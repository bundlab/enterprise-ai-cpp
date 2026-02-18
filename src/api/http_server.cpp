#include "api/http_server.h"
#include "../third_party/crow_all.h"


namespace enterprise_ai::api {

    void HttpServer::run(int port) {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/")
        ([]() {
            return "Enterprise AI running";
        });

        CROW_ROUTE(app, "/health")
        ([]() {
            return crow::response(200, "OK");
        });

        app.port(port).multithreaded().run();
    }

} // namespace enterprise_ai::api
