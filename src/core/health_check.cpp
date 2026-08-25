#include "core/health_check.hpp"

#include <algorithm>

namespace enterprise_ai {
namespace core {

const char* to_string(HealthState state) {
    switch (state) {
        case HealthState::HEALTHY:   return "HEALTHY";
        case HealthState::DEGRADED:  return "DEGRADED";
        case HealthState::UNHEALTHY: return "UNHEALTHY";
    }
    return "UNKNOWN";
}

void HealthStatus::add_component(ComponentHealth component) {
    components_.push_back(std::move(component));
}

HealthState HealthStatus::overall_state() const {
    if (components_.empty()) {
        return HealthState::HEALTHY;
    }

    bool degraded = false;
    for (const auto& component : components_) {
        if (component.state == HealthState::UNHEALTHY) {
            return HealthState::UNHEALTHY;
        }
        if (component.state == HealthState::DEGRADED) {
            degraded = true;
        }
    }
    return degraded ? HealthState::DEGRADED : HealthState::HEALTHY;
}

bool HealthStatus::is_healthy() const {
    return overall_state() == HealthState::HEALTHY;
}

const std::vector<ComponentHealth>& HealthStatus::components() const {
    return components_;
}

std::string HealthStatus::to_json() const {
    std::string json = "{\"status\":\"";
    json += to_string(overall_state());
    json += "\",\"components\":[";

    for (std::size_t i = 0; i < components_.size(); ++i) {
        const auto& component = components_[i];
        if (i > 0) {
            json += ",";
        }
        json += "{\"name\":\"" + component.name + "\",\"state\":\"";
        json += to_string(component.state);
        json += "\",\"details\":\"" + component.details + "\"}";
    }

    json += "]}";
    return json;
}

void HealthCheck::register_check(std::shared_ptr<IHealthCheck> check) {
    checks_.push_back(std::move(check));
}

HealthStatus HealthCheck::run() const {
    HealthStatus status;
    for (const auto& check : checks_) {
        status.add_component(check->check());
    }
    return status;
}

ComponentHealth ApplicationHealthCheck::check() const {
    // Basic liveness probe: the application is running if this code executes.
    return ComponentHealth("application", HealthState::HEALTHY, "application is running");
}

ComponentHealth AiEngineHealthCheck::check() const {
    if (!model_loaded_) {
        return ComponentHealth("ai_engine", HealthState::UNHEALTHY, "no model loaded");
    }
    return ComponentHealth("ai_engine", HealthState::HEALTHY, "model ready");
}

ComponentHealth InfrastructureHealthCheck::check() const {
    // Placeholder until real database/network checks are introduced.
    return ComponentHealth("infrastructure", HealthState::HEALTHY,
                           "core infrastructure operational");
}

} // namespace core
} // namespace enterprise_ai
