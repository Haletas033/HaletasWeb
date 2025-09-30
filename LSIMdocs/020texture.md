## Texture System:

### Functions:
`unsigned int Texture::GetTexId(const char* path);`

### Examples:
#### Getting a TextureID:
```c
unsigned int texture = Texture::GetTexId(pathToTexture);
```

#### Assigning a Texture to a Mesh;
```c
mesh->texId = texture;

//Or if already in scene

scene.meshes[meshIndex]->texId = texture;
```