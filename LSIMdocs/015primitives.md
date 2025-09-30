## Primitives

### Functions:
`Mesh primitives::GeneratePlane(const float tileScale);`\
`Mesh primitives::GenerateCube(const float tileScale);`\
`Mesh primitives::GeneratePyramid(const float tileScale);`\
`Mesh primitives::GenerateSphere(const int stacks = 20, const int slices = 30, const float tileScale = 1);`\
`Mesh primitives::GenerateTorus(const int ringSegments = 30, const int tubeSegments = 20, const float ringRadius = 0.3f, const float tubeRadius = 0.1f, const float tileScale = 1);`

_tileScale is the size of the texture. A tileScale of 10 will mean the texture will be repeated 10 times in each direction._

### Adding primitive to a Scene:
```c
Mesh mesh{GenerateSphere()};
scene.meshes.pushback(mesh);
```