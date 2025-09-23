## File System

 Welcome to the section on the file system of L-SIM.
 The file system in contained inside of `fileIO.h` and `fileIO.cpp`.
 In this section we will discuss.
 * The order items are saved in
 * how to add your own fields to be saved

### Order of Items
**The order of items as they are written and read from a file is as following:**
1. versionLen
2. version (string)
3. meshCount
4. 
**for each mesh:**
1. nameLen
2. name (string)
3. verticesLen
4. vertices (array)
5. indicesLen
6. indices (array)
7. useTexture
8. useNormalMap
9. texturePathLen
10. texturePath (string)
11. specMapPathLen
12. specMapPath (string)
13. normalMapPathLen
14. normalMapPath (string)
15. **if loading texIDs are generated based on the paths provided**
16. color
17. meshID
18. parentID
19. position
20. rotation
21. scale
22. modelMatrix
23. **if loading buffers (VAO, VBO, EBO) are created here and the mesh is configured in the hierarchy**

**End of loop**

1. lightCount

**For each light:**
1. lightPos
2. lightColor
3. attenuationScale

**End of loop**

**End**

### How to add a new field
Adding a field to be saved is quite straightforward.
To do this, in `fileIO.cpp` inside the `saveToFile` function add:
```c
safeWrite([value to write to (as reference)], [size of field (in bytes)], [error message if fail]);
```

**EDITOR'S NOTE:**
_Remember that if you are writing a field with dynamic size e.g., name or vertices you will need to save the length as well_

Reading a value is basically the same as writing one.
To do this, in `fileIO.cpp` inside the `loadFromFile` function add:
```c
safeRead([value to write to (as reference)], [size of field (in bytes)], [error message if fail]);
```

**Below is as example of writing a name value:**
```c
int nameLen = name.size();
//Write nameLen
safeWrite(&nameLen, sizeof(nameLen), "Failed to write name length");
//Write name
safeWrite(mesh->name.c_str(), nameLen * sizeof(char), "Failed to write name");
```

**And here's reading:**
```c
int nameLen;
std::string name;
//Read nameLen
safeRead(&nameLen, sizeof(nameLen), "Failed to read name length");

name.resize(nameLen);

//Read name
safeRead(name.data(), nameLen * sizeof(char), "Failed to read name");
```

**EDITOR'S NOTE:**
_Remember to make sure fields are read and written in the same order and the field must be handled in both functions._