# Basic-Emulator

Basic-Emulator is a simple Applesoft-Basic Emulator.

## Installation

Use git to clone the repository:

```bash
git clone https://gitlab.com/average-basic-enjoyers/basic-emulator.git
```

### Linux

On Debian based distros, the following packages are required:

```bash
sudo apt install git cmake ninja-build libx11-dev xorg-dev libglu1-mesa-dev
```

### Windows

Git, CMake and preferably Visual Studio (as it ships with Microsoft's MSVC compiler) must all be installed.

## Building

Basic-Emulator uses the CMake build system.

### Configure wrapper

For quick setup, the configure wrapper takes care of setting up CMake automatically. It must be run from the Basic-Emulator root directory.

#### Linux

```bash
./configure-lin-64 <debug|release>
```

#### Windows

```bash
.\configure-win-64.bat <debug|release>
```

## License
[MIT](https://choosealicense.com/licenses/mit/)