#ifndef ENTERPRISE_AI_AI_SERVICE_H
#define ENTERPRISE_AI_AI_SERVICE_H

#include <string>

namespace enterprise_ai {
namespace services {

class AIService {
public:
    AIService() = default;
    ~AIService() = default;

    std::string predict(const std::string& input);
};

} // namespace services
} // namespace enterprise_ai

#endif
