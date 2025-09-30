## Input System

### Functions:
`bool Inputs::isDown(const int key, const bool onlyOnPress, GLFWwindow* window);`\
`void Inputs::MeshInputs(GLFWwindow* window, const Scene &scene,
    const int &currentMesh, int &selectedMeshType, int &selectedMesh, glm::vec3 Orientation);` _For LSIM_\
`void Inputs::LightInputs(Scene &scene, const int &currentLight, GLFWwindow* window);` _For LSIM_\
`void Inputs::IOInputs(GLFWwindow *window, Scene &scene, const std::string & workingDir);` _For LSIM_\
`void Inputs::InputHandler(GLFWwindow* window, Scene &scene, const std::string &workingDir,
    const int &currentMesh, const int &currentLight, int &selectedMeshType, int &selectedMesh, glm::vec3 Orientation);` _For LSIM main function groups others_

### Examples:
#### Checking if Key is Down:
```c
if (Inputs::isDown(GLFW_KEY_ANY, false, window)
    //Do something when key is down
    
//Or if you only want when pressed

if (Inputs::isDown(GLFW_KEY_ANY, true, window)
    //Do something when key is down
```