## Working With Configs
After mastering the basics the next thing you should learn is how configs work.
In L-SIM every project has its own config file.
A config in L-SIM is a single `.json` file (**as of v1.0**) that contains information regarding engine defaults.
Here we will take a look at a `config.json` and how we can modify it to meet our needs.

**Below is the default config filel that we will be working from**
````json
{
    "shader-constants": {
      "MAX_LIGHTS": 8
    },
        "defaults": {
            "defaultWindowWidth": 1920,
            "defaultWindowHeight": 1080,
        
            "size": 256,
            "gridScale": 2.0,
            "heightScale": 80.0,
            "scale": 15.0,
            "octaves": 8,
            "persistence":0.5,
            "lacunarity": 2.0,
        
            "sphereStacks": 20,
            "sphereSlices": 20,
        
            "torusRingSegments": 40,
            "torusTubeSegments": 20,
            "torusRingRadius": 1.0,
            "torusTubeRadius": 0.3,
        
            "FOVdeg": 45.0,
            "nearPlane": 0.1,
            "farPlane": 10000.0,
            "sensitivity": 100.0,
            "speed": 1.0,
            "speedMultiplier": 50.0,
        
            "transformSpeed": 0.1
        },
        "loggers": {
            "stdInfo": {
                "hasTimeStamp": true,
                "colour": "INFO_COLOUR",
                "type": "INFO",
                "level": 0
            },
            "stdWarn": {
                "hasTimeStamp": true,
                "colour": "WARNING_COLOUR",
                "type": "WARNING",
                "level": 1
            },
            "stdError": {
                "hasTimeStamp": true,
                "colour": "ERROR_COLOUR",
                "type": "ERROR",
                "level": 2
            }
        }
}
````
Now if you have worked with before this should be about all you need to get started.
However, if you have not used or even seen a `.json` file before don't worry, by the end of this page you should have a good enough understanding to work with config files in L-SIM and even use them in other applications.
Every `.json` file begins with `{` and ends with `}`. Between these 2 braces are a set of key-value pairs and arrays.
Working from the top down the L-SIM `config.json` file has 3 keys in the first layer.

**Let's take a look at these.**
```json
{
  "shader-constants": {},
  "defaults" : {},
  "loggers" :  {}
}
```
In L-SIM the 3 keys in the first layer are `shader-constants`, `defaults`, and `loggers`.

Let's start by taking a look at `shader-constants` as it is the simplest.
```json
{
  "shader-constants": {
    "MAX_LIGHTS": 8
  }
}
```
The `shader-constants` key contains one other key `MAX_LIGHTS` with a value of 8.
If you had tried to create more than 8 lights, you would have noticed that the engine wouldn't let you.
This is because of the `MAX_LIGHTS` key being set to 8.
If you want you can now try to change it to a different value such as 16 or 4.
Keep in mind that the larger this value the slower the engine will run, although this shouldn't be noticeable so long as the number is not insanely high.

If you want you can also try adding another key-value pair. In this example we will add a `DEFAULT_COLOUR`.
```json
{
  "shader-constants": {
    "MAX_LIGHTS": 8,
    "DEFAULT_RED": 1,
    "DEFAULT_GREEN": 0,
    "DEFAULT_BLUE": 0
  }
}
```
**As of v1.0** we have to add every value separately, but hopefully in newer versions syntax like `DEFAULT_COLOUR: "vec4(1, 0, 0, 1)"` will be supported.
How that we have our new fields we need to  use them in our `default.frag` in the `shaders` directory.
`default.frag` is the fragment shader which handles the colour of every pixel (fragment) on the screen.
Which is why we want to use it here.

Upon opening `default.frag` find this line
```glsl
vec4 baseColor = useTexture ? texture(tex0, texCoord) : meshColor;
```
Now it won't go too into depth about what is happening here. Just know that we are checking to see if we should use a texture or a flat colour.
So in order to use our default colour we should replace the `meshColour` part with `vec4(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, 1.0f)`
How upon loading into the game the default cube and any other mesh without a texture should be your colour (in this case red).

#### So how does this work?
If you have written a shader before you may have noticed the that version part is missing.
Do **not** add it. This is because when the program loads the shaders it first appends the version then for every shader constant you have it does.

```cpp
#define [key] [value]
```

in our case we get

```cpp
#version 330 core
#define MAX_LIGHTS 8
#define DEFAULT_RED 1
#define DEFAULT_GREEN 0
#define DEFAULT_BLUE 0
```
at the top of every shader. We can also use these in engine itself, but we will come back to that later.