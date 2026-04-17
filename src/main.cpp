#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Room.h"
#include "Physics.h"

/**
 * Mnemosyne - 3D Memory Palace Construction Kit
 */

// Settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Lighting
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// World
std::vector<std::unique_ptr<Room>> rooms;
Room* selectedRoom = nullptr;

void performRaycast() {
    // Simple Raycast: just check rooms in front of camera
    selectedRoom = nullptr;
    float minDistance = 10.0f; // Max interaction distance

    for (const auto& room : rooms) {
        glm::vec3 toRoom = room->Position - camera.Position;
        float distance = glm::length(toRoom);
        if (distance < minDistance) {
            glm::vec3 dirToRoom = glm::normalize(toRoom);
            float alignment = glm::dot(camera.Front, dirToRoom);
            if (alignment > 0.95f) { // roughly looking at it
                selectedRoom = room.get();
                minDistance = distance;
            }
        }
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 oldPos = camera.Position;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Basic Collision Detection
    AABB playerBox = {camera.Position - glm::vec3(0.3f), camera.Position + glm::vec3(0.3f)};
    bool collision = false;
    for (const auto& room : rooms) {
        if (playerBox.intersects(room->GetAABB())) {
            collision = true;
            break;
        }
    }
    if (collision) {
        camera.Position = oldPos;
    }

    performRaycast();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (selectedRoom) {
            std::cout << "Interacted with room at (" << selectedRoom->Position.x << ", " << selectedRoom->Position.z << ")" << std::endl;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mnemosyne - Memory Palace Construction Kit", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shader
    Shader ourShader("shaders/vertex.glsl", "shaders/fragment.glsl");

    // Initialize rooms
    rooms.push_back(std::make_unique<SquareRoom>(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    rooms.push_back(std::make_unique<HexRoom>(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    rooms.push_back(std::make_unique<RoundRoom>(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)));

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update matrices and draw
        ourShader.use();
        float aspectRatio = (SCR_HEIGHT > 0) ? (float)SCR_WIDTH / (float)SCR_HEIGHT : 1.0f;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Lighting uniforms
        ourShader.setVec3("lightDir", lightDir);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("lightColor", lightColor);

        for (auto& room : rooms) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), room->Position);
            model = glm::scale(model, room->Size);
            ourShader.setMat4("model", model);

            glm::vec3 color = (room.get() == selectedRoom) ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(0.6f, 0.6f, 0.6f);
            ourShader.setVec3("objectColor", color);
            room->Draw();
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
