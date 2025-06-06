<div id="top"></div>

<!-- PROJECT SHIELDS -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links-->
<div align="center">

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

</div>

<!-- PROJECT LOGO -->
<br />
<div align="center">

  <h3 align="center">Ripple Simulation</h3>

  <p align="center">
    <i>A real-time water ripple simulation using OpenGL with an interactive GUI control panel</i>
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
<summary>Table of Contents</summary>

- [About The Project](#about-the-project)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Contact](#contact)
  - [Maintainer(s)](#maintainers)
  - [Creator(s)](#creators)
- [Additional documentation](#additional-documentation)

</details>


<!-- ABOUT THE PROJECT -->
## About The Project
<div align="center">
  <a href="https://github.com/devniteshkumar/ripple-simulation">
    <img width="80%" alt="ripple-simulation-demo" src="https://media.githubusercontent.com/media/devniteshkumar/ripple-simulation/master/assets/ReadmeGIF.gif">
  </a>
</div>

Ripple Simulation is a real-time water ripple simulation built with OpenGL featuring advanced shader-based rendering and an interactive GUI control panel. The project demonstrates realistic physics-based water ripple effects with multiple simultaneous ripples and comprehensive parameter controls.

**Key Features:**
- Real-time ripple effects with improved realism
- Interactive water surface
- **Multiple simultaneous ripples** (up to 10 ripples)
- Shader-based rendering with enhanced physics
- **Comprehensive GUI Control Panel** with adjustable parameters:
  - Height Scale slider (0.0 - 0.1)
  - Time Scale slider (0.1 - 2.0)
  - Decay Scale slider (0.0 - 5.0)
  - Number of ripples input (customizable limit)
  - Frequency input (wave frequency control)
  - Speed input (ripple propagation speed)
  - **Reset Ripples button** (clear all active ripples)
- Hot-reload shader support
- Memory-safe implementation with Address Sanitizer support

**Dependencies:**
This project uses the following external libraries as git submodules:
- **GLAD** - OpenGL loader library
- **GLFW** - Window and input handling
- **GLM** - OpenGL Mathematics library
- **ImGui** - Immediate Mode GUI library

<p align="right">(<a href="#top">back to top</a>)</p>

## Getting Started

To set up a local instance of the application, follow the steps below.

### Prerequisites
The following dependencies are required to be installed for the project to function properly:
* CMake 3.10 or higher
* OpenGL 3.3 compatible graphics card
* C++17 compatible compiler

<p align="right">(<a href="#top">back to top</a>)</p>

### Installation

_Now that the environment has been set up and configured to properly compile and run the project, the next step is to install and configure the project locally on your system._

1. Clone the repository with submodules
   ```sh
   git clone https://github.com/devniteshkumar/ripple-simulation.git --recurse-submodules
   ```
2. Update submodules if needed
   ```sh
   cd ripple-simulation
   git submodule update --init --recursive
   ```
3. Build the project
   ```sh
   cmake .
   ```
4. Run the application
    ```sh
   cmake --build .
   ```
   ```sh
   ./build/ogt
   ```

**Alternative:** If you are using VSCode, you can just run the build task (generally, `Ctrl + Shift + B`).

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

The Ripple Simulation provides an interactive water surface where you can create and manipulate realistic ripple effects. Use the comprehensive GUI control panel to fine-tune the simulation parameters in real-time.

**Controls:**
- **Left Click**: Create ripples at cursor position (multiple ripples supported)
- **ESC**: Exit application
- **GUI Controls**: Use the Control Panel window to adjust:
  - **Height Scale**: Controls the amplitude of the ripple effect (0.0 - 0.1)
  - **Time Scale**: Controls the speed of the ripple animation (0.1 - 2.0)
  - **Decay Scale**: Controls how quickly ripples fade over time (0.0 - 5.0)
  - **Number of ripples**: Set the maximum number of simultaneous ripples
  - **Frequency**: Adjust the wave frequency for ripple density
  - **Speed**: Control the propagation speed of ripples
  - **Reset Ripples**: Clear all active ripples from the simulation

**Project Structure:**
```
.
├── assets
│   ├── gradient.jpg
│   └── water_texture.jpg
├── build
│   └── ogt
├── external/
│   ├── glad/
│   ├── glfw/
│   ├── glm/
│   └── imgui/
├── CMakeLists.txt
└── src
    ├── main.cpp
    ├── renderer
    │   ├── buffers.cpp
    │   ├── buffers.hpp
    │   ├── renderer.cpp
    │   ├── renderer.hpp
    │   ├── shader.cpp
    │   ├── shader.hpp
    │   ├── texture.cpp
    │   └── texture.hpp
    ├── shader
    │   ├── ripple.frag
    │   └── ripple.vert
    └── utils
        ├── fileio.cpp
        ├── fileio.hpp
        ├── FileWatch.hpp
        └── stb_image.h
```

<p align="right">(<a href="#top">back to top</a>)</p>

## Contact

### Maintainer(s)

The currently active maintainer(s) of this project.

- [Nitesh Kumar](https://github.com/devniteshkumar)

### Creator(s)

Honoring the original creator(s) and ideator(s) of this project.

- [Nitesh Kumar](https://github.com/devniteshkumar)

<p align="right">(<a href="#top">back to top</a>)</p>

## Additional documentation

  - [License](/LICENSE)
  - [DeepWiki Documentation](https://deepwiki.com/devniteshkumar/ripple-simulation)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->

[contributors-shield]: https://img.shields.io/github/contributors/devniteshkumar/ripple-simulation.svg?style=for-the-badge
[contributors-url]: https://github.com/devniteshkumar/ripple-simulation/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/devniteshkumar/ripple-simulation.svg?style=for-the-badge
[forks-url]: https://github.com/devniteshkumar/ripple-simulation/network/members
[stars-shield]: https://img.shields.io/github/stars/devniteshkumar/ripple-simulation.svg?style=for-the-badge
[stars-url]: https://github.com/devniteshkumar/ripple-simulation/stargazers
[issues-shield]: https://img.shields.io/github/issues/devniteshkumar/ripple-simulation.svg?style=for-the-badge
[issues-url]: https://github.com/devniteshkumar/ripple-simulation/issues
[license-shield]: https://img.shields.io/github/license/devniteshkumar/ripple-simulation.svg?style=for-the-badge
[license-url]: https://github.com/devniteshkumar/ripple-simulation/blob/master/LICENSE

