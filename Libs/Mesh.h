#ifndef MESH____H
#define MESH____H

#include <GL/glew.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include <string>
#include <string.h>
#include <unordered_map>
#include "Material.h"

struct Face {
    int vIndex[3], vtIndex[3], vnIndex[3];
    Material material;
};

class Mesh
{
    public:
        Mesh();
        ~Mesh();

        void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
        void RenderMesh();
        void ClearMesh();
        bool CreateMeshFromOBJ(const char * path, bool withMTL);
        void CreateMeshWithTexture(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
        std::unordered_map<std::string, Material> getMaterials();

    private:
        GLuint VAO, VBO, IBO, vertexBuffer, uvBuffer, normalBuffer;
        GLsizei indexCount;
        std::unordered_map<std::string, Material> materials;
        std::vector<Face> faces;
        bool useMTL;
};

#endif