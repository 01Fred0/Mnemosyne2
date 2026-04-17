#include "PalaceObject.h"
#include <GL/glew.h>

std::unique_ptr<Mesh> FileObject::cubeMesh = nullptr;

FileObject::FileObject(std::string name, glm::vec3 pos) : PalaceObject(name, pos, glm::vec3(0.5f)) {
    ensureMesh();
}

void FileObject::ensureMesh() {
    if (cubeMesh) return;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    auto addFace = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 normal) {
        unsigned int startIdx = static_cast<unsigned int>(vertices.size());
        vertices.push_back({p1, normal, {0.0f, 0.0f}});
        vertices.push_back({p2, normal, {1.0f, 0.0f}});
        vertices.push_back({p3, normal, {1.0f, 1.0f}});
        vertices.push_back({p4, normal, {0.0f, 1.0f}});
        indices.push_back(startIdx); indices.push_back(startIdx + 1); indices.push_back(startIdx + 2);
        indices.push_back(startIdx); indices.push_back(startIdx + 2); indices.push_back(startIdx + 3);
    };

    addFace({-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {0, 0, 1});
    addFace({ 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0, 0, -1});
    addFace({-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-1, 0, 0});
    addFace({ 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, {1, 0, 0});
    addFace({-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0, 1, 0});
    addFace({-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}, {0, -1, 0});

    cubeMesh = std::make_unique<Mesh>(vertices, indices);
}

void FileObject::Draw() {
    cubeMesh->Draw();
}
