#ifndef ROOM_H
#define ROOM_H

#include <glm/glm.hpp>
#include <vector>

class Room {
public:
    glm::vec3 Position;
    glm::vec3 Size;

    Room(glm::vec3 position, glm::vec3 size) : Position(position), Size(size) {}
    virtual ~Room() = default;

    virtual void Draw() = 0;
};

class SquareRoom : public Room {
public:
    SquareRoom(glm::vec3 position, glm::vec3 size);
    ~SquareRoom() override;
    void Draw() override;

private:
    unsigned int VAO, VBO;
    void setupMesh();
};

#endif
