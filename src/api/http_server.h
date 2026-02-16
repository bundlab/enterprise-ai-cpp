#ifndef ENTERPRISE_AI_HTTP_SERVER_H
#define ENTERPRISE_AI_HTTP_SERVER_H

#include <string>
#include <functional>
#include <memory>

namespace enterprise_ai {
namespace api {

/**
 * @brief Enterprise HTTP Server Interface
 *
 * Responsible for:
 *  - Starting and stopping HTTP service
 *  - Routing incoming requests
 *  - Delegating AI processing to services layer
 */
class HttpServer {
public:
    /**
     * @brief Constructor
     * @param host Server host address (default: 0.0.0.0)
     * @param port Server port (default: 8080)
     */
    HttpServer(const std::string& host = "0.0.0.0", int port = 8080);

    /**
     * @brief Destructor
     */
    ~HttpServer();

    /**
     * @brief Start the HTTP server
     */
    void start();

    /**
     * @brief Stop the HTTP server
     */
    void stop();

    /**
     * @brief Register request handler callback
     *
     * @param handler Function that processes request body
     * @return std::string response
     */
    void register_handler(
        std::function<std::string(const std::string&)> handler
    );

private:
    std::string host_;
    int port_;
    bool running_;

    std::function<std::string(const std::string&)> request_handler_;
};

}  // namespace api
}  // namespace enterprise_ai

#endif // ENTERPRISE_AI_HTTP_SERVER_H
