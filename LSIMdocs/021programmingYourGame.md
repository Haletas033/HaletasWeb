# Programming Your Own Game
How that you have a solid understanding of LSIM and have made your scene you will want to program your own game.
To do this, First, in the `scripts/` directory of your project
we recommend to create a CMakeLists.txt this way your ide will know about the LSIM files.

**Here is a Simple CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.31)
project(scripts)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    Path to the LSIM source code (not the executable)
)

add_library(scripts STATIC
        Your Files
)
```

To add a new script go into `scripts/` and make a new `.cpp` file. Then add:
```c
#include <include/scene/scripts.h>

class YourClasName final : public Script {
    //Your code
}
REGISTER_SCRIPT(YourClassName) //Remember this!!!
```

Doing this creates your script and registers it with LSIM if you forget to register your script nothing will happen, and you will become very confused as to why. So remember it!

The LSIM Engine offers 2 default functions for scripts these being:
* `void Start() override;` _Called at the start of LSIM_
* `void Update(const float deltaTime) override;` _Called every frame_

How you can go ahead and write your code, but before you do here are some useful `extern`s you will probably need:

`extern Scene scene;` _The scene used by LSIM remember to include `include/scene/scene.h` first_\
`extern Camera camera;` _To work with the camera remember to include `include/scene/camera.h` first_\
`extern GLFWwindow* window` _To work with the window and needed for inputs remember to include `external/glfw/include/GLFW/glfw3.h` first_\
`extern json config` _To work with the config.json file remember to include `include/utils/json.h` first_\
`extern Logger logs` _To work with the logging system remember to include `include/utils/log.h` first_

There are also a few external libraries LSIM uses that you can to these are:

**FastNoiseLite** _`external/FastNoiseLite/FastNoiseLite.h`_\
**Glad** _`external/glad/include/glad/glad.h`_\
**GLFW** _`external/glfw/include/GLFW/glfw3.h`_\
**GLM** _`external/glm/...`_\
**ImGui** _`external/imgui/imgui.h`_\
**NlohmannJSON** _`external/nlohmann/json.hpp`_\
**STB** _`external/stb/stb_image.h`_

Finally, To build your game, you can use the "Run" button next to your project in the launcher. Note: this will assume that your LSIM source code in the parent directory of the LSIM executable.
