#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();
    void Draw();

private:
    unsigned int VBO, EBO;
    void setupMesh();
};

#endif
