#ifndef ENTERPRISE_AI_HTTP_SERVER_H
#define ENTERPRISE_AI_HTTP_SERVER_H

#include <string>  // or whatever you need
#include "services/ai_service.h"

namespace enterprise_ai {


namespace api {

class HttpServer {
public:
    void run(int port);

private:
    services::AIService ai_service_;   // ← lives as long as the HttpServer object
};

} // api
} // enterprise_ai

#endif