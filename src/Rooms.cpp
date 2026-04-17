#include "Room.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// SquareRoom Implementation
SquareRoom::SquareRoom(glm::vec3 position, glm::vec3 size) : Room(position, size) {
    setupMesh();
}

void SquareRoom::setupMesh() {
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

    mesh = std::make_unique<Mesh>(vertices, indices);
}

void SquareRoom::Draw() {
    mesh->Draw();
}

// RoundRoom Implementation
RoundRoom::RoundRoom(glm::vec3 position, glm::vec3 size, int segments) : Room(position, size), segments(segments) {
    setupMesh();
}

void RoundRoom::setupMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    float radius = 0.5f;
    float height = 1.0f;

    // Sides
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        vertices.push_back({{x, -height/2, z}, {cos(angle), 0, sin(angle)}, {(float)i/segments, 0}});
        vertices.push_back({{x,  height/2, z}, {cos(angle), 0, sin(angle)}, {(float)i/segments, 1}});
    }
    for (int i = 0; i < segments; ++i) {
        indices.push_back(2*i); indices.push_back(2*i+1); indices.push_back(2*i+2);
        indices.push_back(2*i+1); indices.push_back(2*i+3); indices.push_back(2*i+2);
    }

    // Top & Bottom Caps
    unsigned int topCenterIdx = vertices.size();
    vertices.push_back({{0, height/2, 0}, {0, 1, 0}, {0.5, 0.5}});
    unsigned int bottomCenterIdx = vertices.size();
    vertices.push_back({{0, -height/2, 0}, {0, -1, 0}, {0.5, 0.5}});

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float nextAngle = 2.0f * M_PI * (i+1) / segments;

        unsigned int v1 = vertices.size();
        vertices.push_back({{cos(angle)*radius, height/2, sin(angle)*radius}, {0,1,0}, {0,0}});
        unsigned int v2 = vertices.size();
        vertices.push_back({{cos(nextAngle)*radius, height/2, sin(nextAngle)*radius}, {0,1,0}, {1,0}});
        indices.push_back(topCenterIdx); indices.push_back(v1); indices.push_back(v2);

        unsigned int v3 = vertices.size();
        vertices.push_back({{cos(angle)*radius, -height/2, sin(angle)*radius}, {0,-1,0}, {0,0}});
        unsigned int v4 = vertices.size();
        vertices.push_back({{cos(nextAngle)*radius, -height/2, sin(nextAngle)*radius}, {0,-1,0}, {1,0}});
        indices.push_back(bottomCenterIdx); indices.push_back(v4); indices.push_back(v3);
    }

    mesh = std::make_unique<Mesh>(vertices, indices);
}

void RoundRoom::Draw() {
    mesh->Draw();
}

// HexRoom Implementation
HexRoom::HexRoom(glm::vec3 position, glm::vec3 size) : Room(position, size) {
    setupMesh();
}

void HexRoom::setupMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    float radius = 0.5f;
    float height = 1.0f;

    // Sides
    for (int i = 0; i < 6; ++i) {
        float angle = 2.0f * M_PI * i / 6.0f;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        vertices.push_back({{x, -height/2, z}, {cos(angle), 0, sin(angle)}, {(float)i/6.0f, 0}});
        vertices.push_back({{x,  height/2, z}, {cos(angle), 0, sin(angle)}, {(float)i/6.0f, 1}});
    }
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        indices.push_back(2*i); indices.push_back(2*i+1); indices.push_back(2*next);
        indices.push_back(2*i+1); indices.push_back(2*next+1); indices.push_back(2*next);
    }

    // Caps
    unsigned int topCenterIdx = vertices.size();
    vertices.push_back({{0, height/2, 0}, {0, 1, 0}, {0.5, 0.5}});
    unsigned int bottomCenterIdx = vertices.size();
    vertices.push_back({{0, -height/2, 0}, {0, -1, 0}, {0.5, 0.5}});

    for (int i = 0; i < 6; ++i) {
        float angle = 2.0f * M_PI * i / 6.0f;
        float nextAngle = 2.0f * M_PI * (i+1) / 6.0f;

        unsigned int v1 = vertices.size();
        vertices.push_back({{cos(angle)*radius, height/2, sin(angle)*radius}, {0,1,0}, {0,0}});
        unsigned int v2 = vertices.size();
        vertices.push_back({{cos(nextAngle)*radius, height/2, sin(nextAngle)*radius}, {0,1,0}, {1,0}});
        indices.push_back(topCenterIdx); indices.push_back(v1); indices.push_back(v2);

        unsigned int v3 = vertices.size();
        vertices.push_back({{cos(angle)*radius, -height/2, sin(angle)*radius}, {0,-1,0}, {0,0}});
        unsigned int v4 = vertices.size();
        vertices.push_back({{cos(nextAngle)*radius, -height/2, sin(nextAngle)*radius}, {0,-1,0}, {1,0}});
        indices.push_back(bottomCenterIdx); indices.push_back(v4); indices.push_back(v3);
    }

    mesh = std::make_unique<Mesh>(vertices, indices);
}

void HexRoom::Draw() {
    mesh->Draw();
}
