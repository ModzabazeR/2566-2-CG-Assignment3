#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    unsigned int textureID;
    float shininess;
    std::string diffuseTexPath; // Path to the diffuse texture
    std::string toString() const {
        return "Meterial[name=" + name + ", diffuseTexPath=" + diffuseTexPath + "]";
    }
};

std::unordered_map<std::string, Material> loadMTL(const std::string& filePath);


#endif //MATERIAL_H
