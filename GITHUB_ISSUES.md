# Identified Issues - Mnemosyne

This document outlines the discrepancies between the intended features described in `DESCRIPTION.md` and the current implementation, along with technical bugs and architectural improvements needed.

## 1. Missing Core Features (Discrepancy with DESCRIPTION.md)

### [Feature] Modular Room System Incomplete
*   **Description**: `DESCRIPTION.md` mentions `RoundRoom`, `HexRoom`, and corridors.
*   **Current State**: Only `SquareRoom` is implemented.
*   **Cause**: Initial skeleton implementation only covers the base `Room` and one derived class.
*   **Suggested Action**: Implement `RoundRoom` and `HexRoom` classes inheriting from `Room`, and create a corridor system for interconnection.

### [Feature] Missing Physics and Collision Detection
*   **Description**: The project claims custom AABB collision detection for realistic movement.
*   **Current State**: No collision detection is implemented. The camera can pass through the `SquareRoom` walls and has no gravity or ground constraints.
*   **Cause**: `Camera::ProcessKeyboard` has the ground constraint commented out, and there is no physics engine or AABB check in the render loop.
*   **Suggested Action**: Implement an AABB collision system and integrate it into the `Camera` movement logic.

### [Feature] Missing Serialization System
*   **Description**: JSON-based save/load system is mentioned as a core system.
*   **Current State**: While `nlohmann_json` is a listed dependency in `CMakeLists.txt`, there is no code for saving or loading room layouts or object positions.
*   **Cause**: Serialization logic has not been implemented in `main.cpp` or the `Room` classes.
*   **Suggested Action**: Implement `toJSON()` and `fromJSON()` methods in the `Room` hierarchy and a `PalaceManager` to handle file I/O.

### [Feature] Lack of Advanced Graphics (Shadows & Post-Processing)
*   **Description**: Features like Shadow Mapping, Bloom, and Color Grading are advertised.
*   **Current State**: The current rendering pipeline is a basic forward pass with a hardcoded gray fragment shader.
*   **Cause**: Shaders and the main rendering loop do not include framebuffers for shadows or post-processing effects.
*   **Suggested Action**: Implement a depth pass for shadow mapping and a post-processing stack using Screen Space Quads.

### [Feature] Missing Spatial Audio
*   **Description**: Integrated spatial audio with miniaudio is listed.
*   **Current State**: No audio initialization or processing code exists in the repository.
*   **Cause**: The audio subsystem is entirely absent from the current codebase.
*   **Suggested Action**: Integrate the `miniaudio` library and implement an `AudioManager` class.

### [Feature] Missing Editor Functionality (Gizmos, Snap, Undo/Redo)
*   **Description**: `DESCRIPTION.md` mentions grid-based snapping, gizmo-based editing, and an undo/redo system.
*   **Current State**: None of these features are implemented. The application is a pure viewer.
*   **Cause**: The editor subsystem has not been developed.
*   **Suggested Action**: Implement a `Command` pattern for undo/redo and integrate an UI library (like ImGui) for gizmos and editor controls.

---

## 2. Technical Bugs and Code Quality Issues

### [Bug] Missing Vertex Attributes for Lighting and Texturing
*   **File**: `src/SquareRoom.cpp`
*   **Description**: The `vertices` array in `setupMesh()` only contains 3D positions (X, Y, Z).
*   **Cause**: Missing normals (for lighting) and texture coordinates (UVs).
*   **Suggested Action**: Update the vertex data to include `vec3` normals and `vec2` UVs, and update `glVertexAttribPointer` calls accordingly.

### [Bug] Hardcoded Fragment Shader Output
*   **File**: `shaders/fragment.glsl`
*   **Description**: `FragColor` is hardcoded to `vec4(0.5, 0.5, 0.5, 1.0)`.
*   **Cause**: Placeholder shader implementation.
*   **Suggested Action**: Implement a proper lighting model (e.g., Phong or PBR) in the fragment shader that uses vertex normals and material properties.

### [Bug] Incomplete Camera Constraints
*   **File**: `src/Camera.cpp`
*   **Description**: `Position.y = 0.0f;` is commented out in `ProcessKeyboard`.
*   **Cause**: Intentional or accidental omission during development.
*   **Suggested Action**: Uncomment or implement a more robust gravity/grounding system if "no flying" is a requirement.

### [Bug] OpenGL Resource Leak in Shader Class
*   **File**: `include/Shader.h` and `src/Shader.cpp`
*   **Description**: The `Shader` class does not delete the OpenGL program ID in a destructor.
*   **Cause**: Missing destructor implementation for resource cleanup.
*   **Suggested Action**: Add a destructor `~Shader()` that calls `glDeleteProgram(ID)`.

### [Bug] Potential Runtime Crash on Missing Shader Files
*   **File**: `src/Shader.cpp`
*   **Description**: If shader files are missing, the error is caught and printed, but the `Shader` object is still created with an invalid or empty program, which might lead to crashes or undefined behavior when `use()` or `setMat4()` is called.
*   **Cause**: Inadequate error propagation after failing to read shader files.
*   **Suggested Action**: Use exceptions or a success flag to prevent the application from proceeding with a broken shader program.

### [Improvement] Lack of Resource Management
*   **Description**: Textures and shaders are loaded directly. `Assimp` is a dependency but not used for model loading.
*   **Cause**: Missing architectural layer for asset handling.
*   **Suggested Action**: Implement a `ResourceManager` to prevent duplicate loading of assets and manage OpenGL resource lifetimes efficiently.

---

## 3. Build and Dependency Issues

### [Build] Missing Dependency Management Configuration
*   **File**: `CMakeLists.txt`
*   **Description**: The build system requires `glfw3`, `glm`, `assimp`, `nlohmann_json`, and `GLEW`.
*   **Current State**: `find_package` calls fail if these are not pre-installed on the system. Although `vcpkg` detection is present, there is no `vcpkg.json` to automate dependency installation.
*   **Cause**: Reliance on manual environment setup.
*   **Suggested Action**: Add a `vcpkg.json` manifest file to the root directory to allow for automatic dependency resolution.
