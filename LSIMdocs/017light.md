## Light System:

### Properties:
```c
glm::vec4 lightColor = glm::vec4(1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f);
    float attenuationScale = 1.0f;
    float invScale = 1.0f / (attenuationScale + 0.001f); // Avoid division by zero
    float linear     = 0.09f * invScale;
    float quadratic  = 0.032f * invScale;
```

### Examples:
#### Making a new light:
```c
Light light;
```
#### Adding a Light to a Scene:
```c
scene.lights.push_back(light);
```
#### Deleting a Light from a Scene:
```c
scene.lights.erase(scene.lights.begin() + lightIndex);
```
#### Changing a property:
```c
light->property = newValue;

//If in a scene

scene.lights[lightIndex]->property = newValue;
```
_Note: As of v1.0 lights don't have names or any identifiers but v1.1 should fix this_