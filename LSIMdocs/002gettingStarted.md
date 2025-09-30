## Getting Started

To begin using the [L-SIM Engine](https://github.com/Haletas033/LSIMENGINE) first download the latest version containing both the executable and source code (v1.0 as of writing this) .
Now while you can technically load the engine at this point; I would advise not to.
This is because it will use the engine directory as the working directory which will lead to every project's files being in the same place. Instead, you should download the [L-SIM Launcher](https://github.com/Haletas033/LSIMLAUNCHER) then set the main working directory and finally use the add project button to create a new project in that directory.
This in turn will lead to every project being in its own directory in the main working directory.

**EDITOR'S NOTE:** _Ensure that your main working directory is not a directory containing any non-L-SIM projects.
Otherwise, these directories will still appear when selecting a project to load making it harder to find what you are looking for._

**EXTRA INFO:** _For a directory to work as an L-SIM project it must–at least–include a resources directory (for textures) and a Scripts directory for scripts.
A shaders directory containing a `default.vert` `default.frag`, `default.geom`, and `instance.vert` which can most easily be found in the [L-SIM Engine](https://github.com/Haletas033/LSIMENGINE) repo or [L-SIM Launcher](https://github.com/Haletas033/LSIMLAUNCHER) repo.
Finally, you need a config directory containing a `config.json` for engine defaults which can be found in the repos previously listed._