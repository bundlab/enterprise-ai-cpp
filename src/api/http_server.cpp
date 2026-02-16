#include "http_server.h"
#include "../services/ai_service.h"
#include "crow_all.h"

#include <string>

namespace enterprise_ai {
namespace api {

void HttpServer::run(int port) {
    crow::SimpleApp app;
    services::AIService aiService;

    CROW_ROUTE(app, "/health")
    ([] {
        return crow::response(200, "OK");
    });

    CROW_ROUTE(app, "/ai/predict")
        .methods(crow::HTTPMethod::Post)
    ([&aiService](const crow::request& req) {
        auto result = aiService.predict(req.body);
        return crow::response(200, result);
    });

    app.port(port).multithreaded().run();
}

} // namespace api
} // namespace enterprise_ai
