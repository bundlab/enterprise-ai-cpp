#ifndef ENTERPRISE_AI_CORE_HEALTH_CHECK_HPP
#define ENTERPRISE_AI_CORE_HEALTH_CHECK_HPP

#include <memory>
#include <string>
#include <vector>

namespace enterprise_ai {
namespace core {

/**
 * @brief Basic health states for any component in the system.
 */
enum class HealthState {
    HEALTHY,
    DEGRADED,
    UNHEALTHY
};

/**
 * @brief Convert a HealthState to its string representation.
 */
const char* to_string(HealthState state);

/**
 * @brief Health result reported by a single component check.
 */
struct ComponentHealth {
    std::string name;
    HealthState state = HealthState::UNHEALTHY;
    std::string details;

    ComponentHealth() = default;
    ComponentHealth(std::string n, HealthState s, std::string d = "")
        : name(std::move(n)), state(s), details(std::move(d)) {}
};

/**
 * @brief Structured result of running the full health check.
 *
 * Aggregates per-component results and exposes the overall state.
 */
class HealthStatus {
public:
    HealthStatus() = default;

    void add_component(ComponentHealth component);

    /**
     * @brief Overall state: UNHEALTHY if any component is unhealthy,
     *        otherwise DEGRADED if any is degraded, otherwise HEALTHY.
     */
    HealthState overall_state() const;

    bool is_healthy() const;

    const std::vector<ComponentHealth>& components() const;

    /** @brief JSON serialization, e.g. for the /health endpoint. */
    std::string to_json() const;

private:
    std::vector<ComponentHealth> components_;
};

/**
 * @brief Interface every health-checkable component must implement.
 *
 * Extend the system by implementing this interface and registering the
 * instance with HealthCheck.
 */
class IHealthCheck {
public:
    virtual ~IHealthCheck() = default;

    /** @brief Run the check and report this component's health. */
    virtual ComponentHealth check() const = 0;
};

/**
 * @brief Aggregates registered IHealthCheck instances into one HealthStatus.
 *
 * Usage:
 * @code
 *   enterprise_ai::core::HealthCheck health_check;
 *   health_check.register_check(
 *       std::make_shared<enterprise_ai::core::ApplicationHealthCheck>());
 *
 *   auto status = health_check.run();
 *   if (status.is_healthy()) {
 *       std::cout << "System is healthy\n";
 *   }
 * @endcode
 */
class HealthCheck {
public:
    /** @brief Register a component check. Order defines report order. */
    void register_check(std::shared_ptr<IHealthCheck> check);

    /** @brief Run all registered checks and build the structured status. */
    HealthStatus run() const;

private:
    std::vector<std::shared_ptr<IHealthCheck>> checks_;
};

/**
 * @name Built-in basic checks
 * Placeholder implementations that verify their subsystem is alive.
 * Future issues can replace these with real database/network/service probes.
 */
///@{
class ApplicationHealthCheck : public IHealthCheck {
public:
    ComponentHealth check() const override;
};

class AiEngineHealthCheck : public IHealthCheck {
public:
    explicit AiEngineHealthCheck(bool model_loaded = true)
        : model_loaded_(model_loaded) {}

    ComponentHealth check() const override;

private:
    bool model_loaded_;
};

class InfrastructureHealthCheck : public IHealthCheck {
public:
    ComponentHealth check() const override;
};
///@}

} // namespace core
} // namespace enterprise_ai

#endif // ENTERPRISE_AI_CORE_HEALTH_CHECK_HPP
