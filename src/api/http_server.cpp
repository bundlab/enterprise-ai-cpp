// http_server.h
#ifndef ENTERPRISE_AI_API_HTTP_SERVER_H
#define ENTERPRISE_AI_API_HTTP_SERVER_H

#include "services/ai_service.h"


namespace enterprise_ai {


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