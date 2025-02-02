# Basic-Emulator

Basic-Emulator is a simple Applesoft-Basic Emulator.

## Prerequisites

In order to build the emulator, you must have a few things installed:

 - [CMake](https://cmake.org)
 - [LLVM](https://llvm.org)
 - [Ninja](https://ninja-build.org)

Make sure that all of the above are installed correctly and that they are set in your `PATH` environment variable.

### Linux

On Debian based distros, the following packages are required:

```bash
sudo apt install git cmake ninja-build libx11-dev xorg-dev libglu1-mesa-dev
```

## Building

### VSCode

In order to build the project using VSCode, you must first configure the CMake project:

1. Press `F1` to open the command palette
2. Type in _CMake: Configure_
3. Press `Enter`

You will probably get asked what preset you want to choose, you can choose from
 - Debug: `<os>-64-debug`
 - Release: `<os>-64-release`

Then you can build the project:

1. Press `F1` to open the command palette
2. Type in _CMake: Build_
3. Press `Enter`

### Command Line

In order to build the project using the commandline, you must first configure the CMake project. There are different presets to choose from, you can list them using:

```bash
cmake --list-presets
```

There will probably be two presets to choose from:
 - Debug: `<os>-64-debug`
 - Release: `<os>-64-release`

You can then go on and choose a preset using the following command:

```bash
cmake --preset=<preset-name>
```

After this, you can build the project. For building, there are also different presets to choose from. It must be noted that the build preset **must** match with the configure preset. You can see all possible build presets using the following command:

```bash
cmake --build --list-presets
```

Now you can build the project using a preset of your choice:
```bash
cmake --build --preset=<preset-name>
```
