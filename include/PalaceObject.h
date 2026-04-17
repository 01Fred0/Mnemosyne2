#ifndef PALACE_OBJECT_H
#define PALACE_OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Mesh.h"

class PalaceObject {
public:
    std::string Name;
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;

    PalaceObject(std::string name, glm::vec3 pos, glm::vec3 scale = glm::vec3(1.0f))
        : Name(name), Position(pos), Scale(scale), Rotation(glm::vec3(0.0f)) {}

    virtual void Draw() = 0;
};

class FileObject : public PalaceObject {
public:
    FileObject(std::string name, glm::vec3 pos);
    void Draw() override;

private:
    static std::unique_ptr<Mesh> cubeMesh;
    void ensureMesh();
};

#endif
