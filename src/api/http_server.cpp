// http_server.h
#ifndef ENTERPRISE_AI_API_HTTP_SERVER_H
#define ENTERPRISE_AI_API_HTTP_SERVER_H

        // or "crow_all.h" depending on your setup

namespace enterprise_ai {
namespace services {
    class AIService;       // forward declaration
}

namespace api {

class HttpServer {
public:
    void run(int port = 8080);

private:
    services::AIService aiService_;  // ← must exist as member
};

} // namespace api
} // namespace enterprise_ai

#endif