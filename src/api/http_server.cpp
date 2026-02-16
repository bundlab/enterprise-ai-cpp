#include "api/http_server.h"

#include "crow_all.h"   // ✅ REQUIRED
#include <string>

using namespace crow;   // ✅ REQUIRED
using namespace std;


class HttpServer {
    public:
        void HttpServer::run(int port) {
            crow::SimpleApp app;
            AIService aiService;

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

};
