// Unit tests for the health-check component.
// Self-contained test harness so the component has no external dependencies.

#include "core/health_check.hpp"

#include <iostream>
#include <string>

using namespace enterprise_ai::core;

namespace {

int failures = 0;
int passed = 0;

void expect_true(bool condition, const std::string& what) {
    if (condition) {
        ++passed;
    } else {
        ++failures;
        std::cout << "[FAIL] " << what << "\n";
    }
}

void expect_state(HealthState actual, HealthState expected, const std::string& what) {
    expect_true(actual == expected,
                what + " (expected " + to_string(expected) + ", got " + to_string(actual) + ")");
}

// Configurable fake used to simulate component states.
class FakeCheck : public IHealthCheck {
public:
    FakeCheck(std::string name, HealthState state)
        : name_(std::move(name)), state_(state) {}

    ComponentHealth check() const override {
        return ComponentHealth(name_, state_, "fake");
    }

private:
    std::string name_;
    HealthState state_;
};

void test_health_state_to_string() {
    expect_true(std::string(to_string(HealthState::HEALTHY)) == "HEALTHY", "HEALTHY string");
    expect_true(std::string(to_string(HealthState::DEGRADED)) == "DEGRADED", "DEGRADED string");
    expect_true(std::string(to_string(HealthState::UNHEALTHY)) == "UNHEALTHY", "UNHEALTHY string");
}

void test_empty_status_is_healthy() {
    HealthStatus status;
    expect_state(status.overall_state(), HealthState::HEALTHY, "empty status overall");
    expect_true(status.is_healthy(), "empty status is_healthy");
}

void test_all_healthy_components() {
    HealthStatus status;
    status.add_component({"app", HealthState::HEALTHY, ""});
    status.add_component({"ai", HealthState::HEALTHY, ""});
    status.add_component({"infra", HealthState::HEALTHY, ""});
    expect_true(status.is_healthy(), "all healthy -> is_healthy");
    expect_state(status.overall_state(), HealthState::HEALTHY, "all healthy overall");
}

void test_degraded_component() {
    HealthStatus status;
    status.add_component({"app", HealthState::HEALTHY, ""});
    status.add_component({"ai", HealthState::DEGRADED, "high latency"});
    expect_state(status.overall_state(), HealthState::DEGRADED, "degraded overall");
    expect_true(!status.is_healthy(), "degraded not is_healthy");
}

void test_unhealthy_component_wins_over_degraded() {
    HealthStatus status;
    status.add_component({"ai", HealthState::DEGRADED, ""});
    status.add_component({"db", HealthState::UNHEALTHY, "connection refused"});
    expect_state(status.overall_state(), HealthState::UNHEALTHY, "unhealthy wins overall");
    expect_true(!status.is_healthy(), "unhealthy not is_healthy");
}

void test_health_check_aggregator() {
    HealthCheck health_check;
    health_check.register_check(std::make_shared<FakeCheck>("application", HealthState::HEALTHY));

    auto status = health_check.run();
    expect_true(status.is_healthy(), "aggregator healthy run");

    HealthCheck mixed;
    mixed.register_check(std::make_shared<FakeCheck>("ai_engine", HealthState::UNHEALTHY));
    auto bad = mixed.run();
    expect_state(bad.overall_state(), HealthState::UNHEALTHY, "aggregator unhealthy run");
    expect_true(bad.components().size() == 1, "component count");
    expect_true(bad.components()[0].name == "ai_engine", "component name recorded");
}

void test_builtin_checks() {
    ApplicationHealthCheck app_check;
    auto app_health = app_check.check();
    expect_state(app_health.state, HealthState::HEALTHY, "ApplicationHealthCheck healthy");
    expect_true(app_health.name == "application", "application check name");

    AiEngineHealthCheck ai_ok(true);
    expect_state(ai_ok.check().state, HealthState::HEALTHY, "AI engine with model healthy");

    AiEngineHealthCheck ai_bad(false);
    expect_state(ai_bad.check().state, HealthState::UNHEALTHY, "AI engine without model unhealthy");

    InfrastructureHealthCheck infra_check;
    expect_state(infra_check.check().state, HealthState::HEALTHY, "InfrastructureHealthCheck healthy");
}

void test_json_output() {
    HealthStatus status;
    status.add_component(ComponentHealth("application", HealthState::HEALTHY, "running"));
    auto json = status.to_json();
    expect_true(json.find("\"status\":\"HEALTHY\"") != std::string::npos, "json overall status");
    expect_true(json.find("\"name\":\"application\"") != std::string::npos, "json component name");
    expect_true(json.find("\"state\":\"HEALTHY\"") != std::string::npos, "json component state");
}

} // namespace

int main() {
    test_health_state_to_string();
    test_empty_status_is_healthy();
    test_all_healthy_components();
    test_degraded_component();
    test_unhealthy_component_wins_over_degraded();
    test_health_check_aggregator();
    test_builtin_checks();
    test_json_output();

    std::cout << passed << " checks passed, " << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}
