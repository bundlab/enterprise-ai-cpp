#include "api/http_server.h"
#include "../third_party/crow_all.h"

namespace enterprise_ai {
namespace api {

    void HttpServer::run(int port) {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/")
        ([]() {
            return "Enterprise AI running";
        });

        // Basic application health check; extend with AI engine and
        // infrastructure checks as those subsystems come online.
        health_check_.register_check(std::make_shared<core::ApplicationHealthCheck>());

        CROW_ROUTE(app, "/health")
        ([this]() {
            auto status = health_check_.run();

            crow::response res(status.to_json());
            res.add_header("Content-Type", "application/json");
            res.code = status.is_healthy() ? 200 : 503;
            return res;
        });

        app.port(port).multithreaded().run();
    }

} // namespace api
} // namespace enterprise_ai
