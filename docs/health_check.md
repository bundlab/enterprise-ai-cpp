# Health Check Component

A reusable, dependency-free health-check component that reports whether core services are operational. It is designed for monitoring, container orchestration (Kubernetes/Docker health probes), and production readiness.

## Structure

```text
HealthCheck (aggregator)
    ├── ApplicationHealthCheck   – application liveness
    ├── AiEngineHealthCheck      – AI engine / model readiness
    └── InfrastructureHealthCheck– infrastructure (placeholder for future DB/network checks)
```

## Health States

| State       | Meaning                                   |
|-------------|-------------------------------------------|
| `HEALTHY`   | Component fully operational               |
| `DEGRADED`  | Operational but with reduced capability   |
| `UNHEALTHY` | Component not operational                 |

Overall status rules (`HealthStatus::overall_state()`):

- Any `UNHEALTHY` component → overall **UNHEALTHY**
- Otherwise any `DEGRADED` component → overall **DEGRADED**
- Otherwise → overall **HEALTHY**

## Usage Example

```cpp
#include "core/health_check.hpp"
#include <iostream>

int main() {
    using namespace enterprise_ai::core;

    HealthCheck health_check;
    health_check.register_check(std::make_shared<ApplicationHealthCheck>());
    health_check.register_check(std::make_shared<AiEngineHealthCheck>(true));
    health_check.register_check(std::make_shared<InfrastructureHealthCheck>());

    auto status = health_check.run();

    if (status.is_healthy()) {
        std::cout << "System is healthy\n";
    }

    std::cout << status.to_json() << "\n";
}
```

Sample JSON output (served by the `/health` endpoint):

```json
{
  "status": "HEALTHY",
  "components": [
    {"name": "application", "state": "HEALTHY", "details": "application is running"}
  ]
}
```

The `/health` HTTP endpoint returns `200 OK` when healthy/degraded and `503 Service Unavailable` when unhealthy — suitable as a container liveness/readiness probe.

## Extending the Health Check

Any new subsystem (database, network, external service) can plug in without modifying existing code:

1. Implement the `IHealthCheck` interface:

   ```cpp
   class DatabaseHealthCheck : public enterprise_ai::core::IHealthCheck {
   public:
       enterprise_ai::core::ComponentHealth check() const override {
           // Ping the database, run SELECT 1, etc.
           return {"database", enterprise_ai::core::HealthState::HEALTHY,
                   "connection pool ready"};
       }
   };
   ```

2. Register it with the aggregator (e.g. in `HttpServer::run()` in `src/api/http_server.cpp`):

   ```cpp
   health_check_.register_check(std::make_shared<DatabaseHealthCheck>());
   ```

3. Rebuild — the new component automatically appears in `run()` results and the `/health` JSON payload.

Use `DEGRADED` for soft failures (e.g. high latency, fallback mode) and `UNHEALTHY` for hard failures so orchestrators can restart the instance.

## Running Tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Unit tests live in `tests/test_health_check.cpp` and cover all three health states, aggregation rules, built-in checks, and JSON serialization.
