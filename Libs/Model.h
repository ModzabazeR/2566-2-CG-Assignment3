#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <glm/vec3.hpp>

struct Model {
    std::string modelPath;
    std::string texturePath;
    glm::vec3 position;
    float scale = 1.0f;
    bool flipTexture = true;
};

#endif //MODEL_H
