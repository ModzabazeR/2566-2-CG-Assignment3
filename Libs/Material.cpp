// Material.cpp
#include "Material.h"
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

std::unordered_map<std::string, Material> loadMTL(const std::string& filePath) {
    std::unordered_map<std::string, Material> materials;
    std::ifstream file(filePath);
    std::string line;
    Material* currentMaterial = nullptr;

    std::cout << "Loading MTL file " << filePath << std::endl;

    while (getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "newmtl") {
            std::string materialName;
            lineStream >> materialName;
            materials[materialName] = Material();
            currentMaterial = &materials[materialName];
            currentMaterial->name = materialName;
        } else if (prefix == "Ka" && currentMaterial) {
            lineStream >> currentMaterial->ambient.x >> currentMaterial->ambient.y >> currentMaterial->ambient.z;
        } else if (prefix == "Kd" && currentMaterial) {
            lineStream >> currentMaterial->diffuse.x >> currentMaterial->diffuse.y >> currentMaterial->diffuse.z;
        } else if (prefix == "Ks" && currentMaterial) {
            lineStream >> currentMaterial->specular.x >> currentMaterial->specular.y >> currentMaterial->specular.z;
        } else if (prefix == "Ns" && currentMaterial) {
            lineStream >> currentMaterial->shininess;
        } else if (prefix == "map_Kd" && currentMaterial) {
            lineStream >> currentMaterial->diffuseTexPath;

            unsigned int textureID;
            glGenTextures(1, &textureID);
            std::cout << "Loading texture " << currentMaterial->diffuseTexPath << std::endl;

            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(currentMaterial->diffuseTexPath.c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                GLenum format;
                if (nrChannels == 1)
                    format = GL_RED;
                else if (nrChannels == 3)
                    format = GL_RGB;
                else if (nrChannels == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                std::cout << "Texture loaded" << std::endl;
            } else {
                std::cout << "Failed to load " << currentMaterial->diffuseTexPath << std::endl;
            }
            stbi_image_free(data);
            currentMaterial->textureID = textureID;
        }
    }

    return materials;
}
