#include "api/http_server.h"

int main() {
    enterprise_ai::api::HttpServer server;  // fully qualified
    server.run(8080);
    return 0;
}