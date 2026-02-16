#include "ai/inference_engine.hpp"

class AIService {
private:
    InferenceEngine engine;

public:
    std::string predict(const std::string& input) {
        return engine.run(input);
    }
};
