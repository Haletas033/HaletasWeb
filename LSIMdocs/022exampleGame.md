# Example Game:
Here is an example game made in the LSIM engine. This game is a simple procedural maze generator. Note that the code is not the best are there are some errors like Z-fighting it is mainly there to demonstrate how a game could be made not how a good game could be made **(:**

<details>
  <summary role="button" class="outline secondary"><strong>Maze Game</strong></summary>

  <details>
    <summary role="button" class="outline secondary"><strong>playerMovement.cpp</strong></summary>
    <article>

```c
        #include <include/scene/script.h>
        #include <include/inputs/inputs.h>
        #include <include/geometry/mesh.h>
        #include <external/glfw/include/GLFW/glfw3.h>
        #include <external/glm/glm.hpp>
        #include <external/glm/gtc/matrix_transform.hpp>
        #include <external/glm/gtc/type_ptr.hpp>
        #include <include/scene/scene.h>


        extern Scene scene;
        extern GLFWwindow* window;
        extern Camera camera;
        
        class PlayerMovement final : public Script {
        Inputs inputs;
        float speed = 5.f;
    
        double lastMouseX = 0.0;
        double lastMouseY = 0.0;
        bool firstMouseMove = true;
    
        float yaw   = -90.f;
        float pitch = 0.f;
    
        float mouseSensitivity = 0.2f;
    
        bool moving = false;
    
        static void CheckCollision(float x, float z, float sx, float sz) {
            auto testAABB = [&](glm::vec3 pos, glm::vec3 scale) {
                if (scale == glm::vec3(0.0f)) return; // safety guard
    
                const float pxMin = x - sx / 2;
                const float pxMax = x + sx / 2;
                const float pzMin = z - sz / 2;
                const float pzMax = z + sz / 2;
    
                const float mxMin = pos.x - scale.x / 2.0f;
                const float mxMax = pos.x + scale.x / 2.0f;
                const float mzMin = pos.z - scale.z / 2.0f;
                const float mzMax = pos.z + scale.z / 2.0f;
    
                float overlapX = std::min(pxMax, mxMax) - std::max(pxMin, mxMin);
                float overlapZ = std::min(pzMax, mzMax) - std::max(pzMin, mzMin);
    
                if (overlapX > 0 && overlapZ > 0) {
                    if (overlapX < overlapZ) {
                        camera.Position.x += (camera.Position.x < pos.x ? -overlapX : overlapX);
                    } else {
                        camera.Position.z += (camera.Position.z < pos.z ? -overlapZ : overlapZ);
                    }
                }
            };
    
            for (const auto &meshPtr : scene.meshes) {
                if (meshPtr->name == "Ground") continue;
                Mesh &mesh = *meshPtr;
                testAABB(mesh.position, mesh.scale);
            }
    
            for (const auto &instanced : scene.instancedMeshes) {
                for (const auto &mat : instanced->instances) {
                    glm::vec3 pos = glm::vec3(mat[3]);
                    glm::vec3 scale = glm::vec3(glm::length(mat[0]),
                                                glm::length(mat[1]),
                                                glm::length(mat[2]));
    
                    testAABB(pos, scale);
                }
            }
        }
    
    
        void HandleInputs(float deltaTime) {
            // local space
            glm::vec3 forward = glm::normalize(glm::vec3(camera.Orientation.x, 0.0f, camera.Orientation.z));
            glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
    
            if (inputs.isDown(GLFW_KEY_W, false, window))
                camera.Position += forward * speed * deltaTime;
            if (inputs.isDown(GLFW_KEY_S, false, window))
                camera.Position -= forward * speed * deltaTime;
            if (inputs.isDown(GLFW_KEY_D, false, window))
                camera.Position += right * speed * deltaTime;
            if (inputs.isDown(GLFW_KEY_A, false, window))
                camera.Position -= right * speed * deltaTime;
            if (inputs.isDown(GLFW_KEY_LEFT_SHIFT, false, window))
                speed = 10.f;
            else
                speed = 5.f;
    
            if (inputs.isDown(GLFW_KEY_W, false, window)
                || inputs.isDown(GLFW_KEY_S, false, window)
                || inputs.isDown(GLFW_KEY_A, false, window)
                || inputs.isDown(GLFW_KEY_D, false, window)) {
                moving = true;
            } else moving = false;
        }
    
        void Start() override {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    
        void Update(float deltaTime) override {
            // Get current mouse position
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
    
            if (firstMouseMove) {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstMouseMove = false;
            }
    
            const double deltaX = mouseX - lastMouseX;
            const double deltaY = lastMouseY - mouseY;
    
            lastMouseX = mouseX;
            lastMouseY = mouseY;
    
            yaw   += static_cast<float>(deltaX) * mouseSensitivity;
            pitch += static_cast<float>(deltaY) * mouseSensitivity;
    
            if (pitch > 89.f) pitch = 89.f;
            if (pitch < -89.f) pitch = -89.f;
    
            glm::vec3 dir;
            dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            dir.y = sin(glm::radians(pitch));
            dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            camera.Orientation = glm::normalize(dir);
    
            HandleInputs(deltaTime);
    
            const float amplitude = moving ? 0.05f : 0.01f;
            camera.Position.y = 1.8 + (sin(glfwGetTime() * speed) * amplitude);
    
            scene.lights[0].lightPos = camera.Position;
    
            CheckCollision(camera.Position.x, camera.Position.z, 1, 1);
        }
    };
    
    REGISTER_SCRIPT(PlayerMovement)
```

  </article>
  </details>

  <details>
    <summary role="button" class="outline secondary"><strong>mazeGen.cpp</strong></summary>
    <article>

```c
    //
    // Created by halet on 9/26/2025.
    //
    
    #include <filesystem>
    #include <iostream>
    #include <random>
    #include <external/glm/vec3.hpp>
    #include <include/scene/script.h>
    #include <include/geometry/primitives.h>
    #include <include/geometry/mesh.h>
    #include <include/scene/scene.h>
    #include <include/utils/texture.h>
    #include <external/glad/include/glad/glad.h>
    #include <external/glm/ext/matrix_transform.hpp>
    
    extern Scene scene;
    extern std::string workingDir;
    class MazeGen final : public Script {
    int mazeSize = 50;
    int cellSize = 5;
    
        struct Cell {
            bool visited = false;
    
            bool top = true;
            bool bottom = true;
            bool left = true;
            bool right = true;
        };
    
        std::vector<std::vector<Cell>> maze;
    
        std::random_device rd;
        std::mt19937 g{rd()};
    
        GLuint brickwall{};
        GLuint brickwallNormal{};
    
        std::unique_ptr<Scene::InstancedMesh> wallBatch;
    
        std::vector<std::pair<int, int>> GetNeighbours(int x, int y) const {
            std::vector<std::pair<int, int>> neighbours;
    
            if (x > 0) neighbours.push_back({x - 1, y});
            if (x < mazeSize - 1) neighbours.push_back({x + 1, y});
            if (y > 0) neighbours.push_back({x, y - 1});
            if (y < mazeSize - 1) neighbours.push_back({x, y + 1});
    
            return neighbours;
        }
    
        void RemoveWallsBetween (const int x, const int y, const int nx, const int ny) {
            if (nx == x + 1) maze[x][y].right = false, maze[nx][ny].left = false;
            else if (nx == x - 1) maze[x][y].left = false, maze[nx][ny].right = false;
            else if (ny == y + 1) maze[x][y].bottom = false, maze[nx][ny].top = false;
            else if (ny == y - 1) maze[x][y].top = false, maze[nx][ny].bottom = false;
        }
    
        void DFS (const int x, const int y) {
            maze[x][y].visited = true;
    
            auto neighbours = GetNeighbours(x, y);
            std::shuffle(neighbours.begin(), neighbours.end(), g);
    
            for (auto [nx, ny] : neighbours) {
                if (!maze[nx][ny].visited) {
                    RemoveWallsBetween(x, y, nx, ny);
                    DFS(nx, ny);
                }
            }
        }
    
        void BuildWall(const float x, const float z, float sx, float sz, const bool vertical) {
            glm::mat4 model(1.0);
    
            if (vertical)
                sz += sx;
    
            glm::vec3 position(x, 2.5f, z);
            glm::vec3 scale(sx, 5.0f, sz);
    
            model = glm::translate(model, position);
            model = glm::scale(model, scale);
    
            wallBatch->instances.push_back(model);
        }
    
        void GenerateMaze() {
            for (int x = 0; x < mazeSize; ++x) {
                for (int y = 0; y < mazeSize; ++y) {
                    constexpr float wallThickness = 1.f;
                    const float baseX = x * cellSize;
                    const float baseZ = y * cellSize;
                    const auto& cell = maze[x][y];
    
                    const float halfCell = cellSize / 2.0f;
    
                    if (cell.top)    BuildWall(baseX, baseZ - halfCell, cellSize, wallThickness, false);
                    if (cell.bottom) BuildWall(baseX, baseZ + halfCell, cellSize, wallThickness, false);
                    if (cell.left)  BuildWall(baseX - halfCell, baseZ, wallThickness, cellSize, true);
                    if (cell.right) BuildWall(baseX + halfCell, baseZ, wallThickness, cellSize, true);
                }
            }
    
        }
    
        void Start() override {
            brickwall = Texture::GetTexId((workingDir + "/resources/brickwall.jpg").c_str());
            brickwallNormal = Texture::GetTexId((workingDir + "/resources/brickwall_normal.jpg").c_str());
    
            wallBatch = std::make_unique<Scene::InstancedMesh>();
            wallBatch->mesh = std::make_unique<Mesh>(primitives::GenerateCube(1));
            wallBatch->mesh->useTexture = true;
            wallBatch->mesh->useNormalMap = true;
            wallBatch->mesh->texId = brickwall;
            wallBatch->mesh->normalMapId = brickwallNormal;
    
            //Make ground plane
            auto ground = std::make_unique<Mesh>(primitives::GeneratePlane(mazeSize));
            ground->name = "Ground";
            ground->scale *= mazeSize * cellSize;
            ground->position = glm::vec3(mazeSize * cellSize / 2 - 2.5, 0, mazeSize * cellSize / 2 - 2.5);
            ground->useTexture = true;
            ground->texId = Texture::GetTexId((workingDir + "/resources/grass.jpg").c_str());
            scene.meshes.push_back(std::move(ground));
    
            maze = std::vector(mazeSize, std::vector<Cell>(mazeSize));
            DFS(1, 1);
            maze[0][0].top = false;
            maze[mazeSize-1][mazeSize-1].bottom = false;
            GenerateMaze();
    
            wallBatch->UploadInstances();
    
            scene.instancedMeshes.push_back(std::move(wallBatch));
        }
    };
    REGISTER_SCRIPT(MazeGen)
```

  </article>
  </details>

</details>

