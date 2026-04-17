#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <memory>
#include "Shader.h"

class Skybox {
public:
    Skybox(std::vector<std::string> faces);
    ~Skybox();
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int VAO, VBO, textureID;
    std::unique_ptr<Shader> skyboxShader;
    void setupMesh();
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif
