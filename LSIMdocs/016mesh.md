## Mesh System

### Properties:
```c
std::string name;

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

bool useTexture = false;
bool useNormalMap = false;

glm::vec4 color = glm::vec4(1.0f);

GLuint texId{};
std::string texturePath;

GLuint specMapId{};
std::string specMapPath;

GLuint normalMapId{};
std::string normalMapPath;

uint16_t meshID = 0;

VAO vao;

glm::vec3 position = glm::vec3(0.0f);
glm::vec3 rotation = glm::vec3(0.0f);
glm::vec3 scale    = glm::vec3(1.0f);

glm::mat4 modelMatrix = glm::mat4(1.0f);
```

### Constructor
`Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);`

### Functions:
`void Mesh::ApplyTransformations();` _Called every frame_\
`void Mesh::Draw(Shader& shader, Camera& camera);` _Called every frame_\
`void Mesh::setupBuffers();`

### Examples:
#### Making a new mesh:
```c
Mesh mesh {someVerticesArray, someIndicesArray};
```
#### Adding Mesh to Scene:
```c
scene.meshes.push_back(mesh);
```
#### Deleting a Mesh From a Scene:
```c
if (Gui::Node* nodeToDelete = Gui::FindNodeByMesh(Gui::root, mesh)) {
    Gui::DeleteNode(nodeToDelete); // This handles reparenting children
}

scene.meshes.erase(scene.meshes.begin() + meshIndex);
```
#### Changing a Property of a Mesh
```c
mesh->property = foo;

//Or if alreay in a Scene

scene.meshes[meshIndex]->property = foo;

//Or if index unknown:

for (auto &mesh : scene.meshes){
    if (mesh->identifierProperty == knownValue){
        mesh->property == newValue
    }
}

//For Example indentifierProperty could be name and knownValue could be "Player"
```

