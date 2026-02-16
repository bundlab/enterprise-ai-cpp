#pragma once
#include "model.hpp"

class InferenceEngine {
private:
    Model model;

public:
    std::string run(const std::string& input) {
        return model.predict(input);
    }
};
