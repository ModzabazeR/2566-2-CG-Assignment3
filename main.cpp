#undef GLFW_DLL
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"
#include "Libs/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

float yaw = -90.0f, pitch = 0.0f;
float deltaTime, lastFrame;
glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

//Vertex Shader
static const char *vShader = "Shaders/shader.vert";

//Fragment Shader
static const char *fShader = "Shaders/shader.frag";

void CreateOBJ(char const *path) {
    Mesh *obj1 = new Mesh();
    std::cout << "Creating model " << path << std::endl;
    bool loaded = obj1->CreateMeshFromOBJ(path);
    if (loaded) {
        meshList.push_back(obj1);
        std::cout << "Model loaded" << std::endl;
    } else {
        std::cout << "Failed to load model " << path << std::endl;
    }
}

void CreateShaders() {
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void checkMouse() {
    double xpos, ypos;
    glfwGetCursorPos(mainWindow.getWindow(), &xpos, &ypos);

    static float lastX = xpos;
    static float lastY = ypos;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    pitch += yoffset;
    yaw += xoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

void checkKeyboard(glm::vec3 &cameraPosition, glm::vec3 &cameraDirection, glm::vec3 &cameraRight, glm::vec3 &cameraUp) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(direction);

    float speed = 5.0f;
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += (cameraDirection * deltaTime * speed);
    }
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= (cameraDirection * deltaTime * speed);
    }
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= (cameraRight * deltaTime * speed);
    }
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += (cameraRight * deltaTime * speed);
    }
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPosition += (cameraUp * deltaTime * speed);
    }
    if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPosition -= (cameraUp * deltaTime * speed);
    }
}

unsigned int loadTexture(char const *path, bool isFlipped = true) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(isFlipped);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
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
    } else {
        std::cout << "Failed to load " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

int main() {
    mainWindow = Window(WIDTH, HEIGHT, 3, 3); // 3 คือ OpenGL version 3.3
    mainWindow.initialise();

    CreateOBJ("Models/anime-school.obj");
    CreateOBJ("Models/shiba.obj");
    CreateOBJ("Models/TheCat.obj");
    CreateOBJ("Models/CatPlushie.obj");
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::vec3 cameraPosition = glm::vec3(0.0f, 2.0f, 2.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

    glm::mat4 projection = glm::perspective(
        45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 500.0f);
    // glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);

    glm::vec3 modelPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.8f, 0.5f),
        glm::vec3(-2.3f, 0.5f, -1.0f),
        glm::vec3(3.7f, 1.3f, 4.0f),
    };

    unsigned int modelTextures[] = {
        loadTexture("Textures/bg.jpg"),
        loadTexture("Textures/shiba.png"),
        loadTexture("Textures/TheCat.png"),
        loadTexture("Textures/CatPlushie.png"),
    };

    //Loop until window closed
    while (!mainWindow.getShouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Get + Handle user input events
        glfwPollEvents();

        checkMouse();
        checkKeyboard(cameraPosition, cameraDirection, cameraRight, cameraUp);

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

        //Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view(1.0f);

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);

        glm::mat4 cameraPosMat(1.0f);
        cameraPosMat[3][0] = -cameraPosition.x;
        cameraPosMat[3][1] = -cameraPosition.y;
        cameraPosMat[3][2] = -cameraPosition.z;

        view = cameraRotateMat * cameraPosMat;

        //draw here
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetUniformLocation("model");
        uniformProjection = shaderList[0].GetUniformLocation("projection");
        uniformView = shaderList[0].GetUniformLocation("view");

        //Object
        for (int i = 0; i < meshList.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, modelPositions[i]);

            float modelScale = 1.0f;

            if (i == 1) {
                modelScale = 50.0f; // shiba
            } else if (i == 2) {
                modelScale = 0.02f; // TheCat
            } else if (i == 3) {
                modelScale = 8.0f; // CatPlushie
            }
            model = glm::scale(model, glm::vec3(modelScale, modelScale, modelScale));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, modelTextures[i]);
            meshList[i]->RenderMesh();
        }


        // light
        glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, (GLfloat *) &lightColour);

        glUseProgram(0);
        //end draw

        mainWindow.swapBuffers();
    }

    return 0;
}
