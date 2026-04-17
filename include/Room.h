#ifndef ROOM_H
#define ROOM_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Physics.h"
#include "PalaceObject.h"

class Room {
public:
    glm::vec3 Position;
    glm::vec3 Size;
    std::vector<std::unique_ptr<PalaceObject>> Objects;

    Room(glm::vec3 position, glm::vec3 size) : Position(position), Size(size) {}
    virtual ~Room() = default;

    virtual void Draw() = 0;
    virtual AABB GetAABB() const {
        return {Position - Size * 0.5f, Position + Size * 0.5f};
    }

    void AddObject(std::unique_ptr<PalaceObject> obj) {
        Objects.push_back(std::move(obj));
    }
};

class SquareRoom : public Room {
public:
    SquareRoom(glm::vec3 position, glm::vec3 size);
    void Draw() override;

private:
    std::unique_ptr<Mesh> mesh;
    void setupMesh();
};

class RoundRoom : public Room {
public:
    RoundRoom(glm::vec3 position, glm::vec3 size, int segments = 32);
    void Draw() override;

private:
    std::unique_ptr<Mesh> mesh;
    int segments;
    void setupMesh();
};

class HexRoom : public Room {
public:
    HexRoom(glm::vec3 position, glm::vec3 size);
    void Draw() override;

private:
    std::unique_ptr<Mesh> mesh;
    void setupMesh();
};

#endif
