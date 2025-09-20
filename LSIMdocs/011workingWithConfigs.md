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