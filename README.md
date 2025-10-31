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

  <h3 align="center">FoldGL</h3>

  <p align="center">
    <i>A high-performance 3D protein structure visualization tool built with OpenGL</i>
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
- [Project Structure](#project-structure)
- [TODO](#todo)
- [Contact](#contact)
  - [Maintainer(s)](#maintainers)
  - [Creator(s)](#creators)
- [Additional documentation](#additional-documentation)

</details>


<!-- ABOUT THE PROJECT -->
## About The Project

FoldGL is a high-performance 3D protein structure visualization tool built with modern OpenGL. It provides real-time rendering of Protein Data Bank (PDB) files with smooth tube-based visualization and an intuitive camera system for interactive exploration of molecular structures.

**Current Features:**
- **PDB File Loading**: Functional support for standard PDB format files
  - Command-line PDB file input
  - Atomic coordinate parsing
  - Residue and chain information extraction
  - CA (Carbon Alpha) backbone extraction
- **3D Tube Visualization**: Real-time OpenGL rendering
  - Modern shader-based rendering pipeline
  - Smooth tube rendering along protein backbone
  - Multi-colored chain segments for visual distinction
  - Advanced lighting system with proper shading
- **Interactive Navigation**:
  - Smooth first-person camera controls
  - Mouse look and WASD movement
  - Fullscreen rendering mode
  - Real-time camera-based lighting
- **Development Features**:
  - Hot-reload shader support for real-time development
  - Memory-safe implementation with Address Sanitizer support
  - Modular architecture with clean separation of concerns

**Dependencies:**
This project uses the following external libraries as git submodules:
- **GLAD** - OpenGL loader library for modern OpenGL functions
- **GLFW** - Cross-platform window and input handling
- **GLM** - OpenGL Mathematics library for vector/matrix operations
- **Bullet Physics** - Rigid-body engine powering bond-constrained unfolding (included as submodule)

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
   git clone https://github.com/devniteshkumar/FoldGL.git --recurse-submodules
   ```
2. Navigate to the project directory
   ```sh
   cd FoldGL
   ```
3. Update submodules if needed
   ```sh
   git submodule update --init --recursive
   ```
4. Configure the project with CMake
   ```sh
   cmake .
   ```
5. Build the project
   ```sh
   cmake --build .
   ```
6. Run the application with a PDB file
   ```sh
   ./build/ogt path/to/your/protein.pdb
   ```

**Note:** The application requires a PDB file as a command-line argument to run. You can download sample PDB files from the [Protein Data Bank](https://www.rcsb.org/).

**Alternative:** If you are using VSCode, you can use the CMake extension and run the build task (generally, `Ctrl + Shift + B`).

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

FoldGL provides real-time 3D visualization of protein structures from PDB files with smooth tube-based rendering. The application loads PDB files via command-line arguments and renders them as colorful tube structures representing the protein backbone.

**Camera Controls:**
- **W/A/S/D**: Move camera forward/left/backward/right
- **Q/E**: Move camera up/down
- **Mouse Movement**: Look around (first-person camera)
- **ESC**: Exit application

**Getting Started:**
1. Build the application (see Installation section below)
2. Launch with a PDB file: `./build/ogt path/to/your/protein.pdb`
3. The application will load the protein structure and display it as colored tubes
4. Use the camera controls to navigate around the 3D protein structure
5. Each chain segment is rendered with distinct colors for easy identification

**Example Usage:**
```bash
# Download a sample PDB file (e.g., 1BNA - a DNA structure)
wget https://files.rcsb.org/download/1BNA.pdb

# Run FoldGL with the PDB file
./build/ogt 1BNA.pdb
```

### Unfolding Simulation (Bullet)
- The CA backbone is simulated with rigid bodies connected by constraints that lock bond lengths and angles; only torsion is free.
- Unfolding runs automatically by applying a gentle end-to-end pull each frame.
- Tuning:
  - Pull strength: edit `sim.applyPulling(...)` in `src/main.cpp`.
  - Mass/damping and CA sphere radius: see `src/physics/unfold.cpp`.

<p align="right">(<a href="#top">back to top</a>)</p>

## Project Structure

```
.
├── build/                      # Build output directory (generated)
│   └── ogt                     # Compiled executable
├── external/                   # External dependencies (git submodules)
│   ├── glad/                   # OpenGL loader library
│   ├── glfw/                   # Window and input handling
│   ├── bullet/                 # Bullet Physics engine
│   └── glm/                    # OpenGL Mathematics library
├── CMakeLists.txt              # CMake build configuration
└── src/                        # Source code
    ├── main.cpp                # Application entry point
    ├── pdb/                    # PDB file parsing and data structures
    │   ├── common.hpp/cpp      # Common utilities and definitions
    │   ├── atom.hpp/cpp        # Atomic data structure and parsing
    │   ├── residue.hpp/cpp     # Amino acid residue representation
    │   ├── chain.hpp/cpp       # Protein chain organization
    │   ├── model.hpp/cpp       # PDB model container
    │   ├── secondary_structure.hpp/cpp  # Helix/strand/coil structures
    │   └── pdb.hpp             # Main PDB package header
    ├── renderer/               # OpenGL rendering system
    │   ├── shader.hpp/cpp      # Shader compilation and management
    │   ├── mesh.hpp/cpp        # 3D mesh representation
    │   ├── camera.hpp/cpp      # Camera system and controls
    │   ├── buffers.hpp/cpp     # OpenGL buffer management
    │   ├── texture.hpp/cpp     # Texture loading and handling
    │   └── renderer.hpp/cpp    # Main rendering pipeline
    ├── shader/                 # GLSL shader files
    │   ├── mesh.vert           # Vertex shader for 3D meshes
    │   └── mesh.frag           # Fragment shader for lighting
    ├── physics/               # Bullet-based unfolding simulation
    │   └── unfold.hpp/cpp
    └── utils/                  # Utility functions
        ├── fileio.hpp/cpp      # File I/O operations
        ├── FileWatch.hpp       # Hot-reload file watching
        └── stb_image.h         # Image loading library
```

<p align="right">(<a href="#top">back to top</a>)</p>

## Completed Features 
* **PDB File Loading and Integration**
  * Command-line argument support for PDB file input
  * Integration of PDB parser with rendering system
  * Error handling for malformed PDB files
  * CA (Carbon Alpha) backbone extraction

* **Basic Protein Visualization**
  * Tube-based backbone rendering
  * Multi-colored chain segments
  * Advanced lighting system with proper shading
* Real-time camera-based lighting
* Smooth tube geometry generation

* **Physics-based Unfolding**
  * Bond-length and bond-angle preservation (twist-only) via Bullet constraints
  * Real-time mesh updates from physics

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
  - [FoldGL Documentation](https://github.com/devniteshkumar/FoldGL/wiki) (Coming Soon)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- Note: Update these URLs to point to your actual FoldGL repository -->

[contributors-shield]: https://img.shields.io/github/contributors/devniteshkumar/FoldGL.svg?style=for-the-badge
[contributors-url]: https://github.com/devniteshkumar/FoldGL/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/devniteshkumar/FoldGL.svg?style=for-the-badge
[forks-url]: https://github.com/devniteshkumar/FoldGL/network/members
[stars-shield]: https://img.shields.io/github/stars/devniteshkumar/FoldGL.svg?style=for-the-badge
[stars-url]: https://github.com/devniteshkumar/FoldGL/stargazers
[issues-shield]: https://img.shields.io/github/issues/devniteshkumar/FoldGL.svg?style=for-the-badge
[issues-url]: https://github.com/devniteshkumar/FoldGL/issues
[license-shield]: https://img.shields.io/github/license/devniteshkumar/FoldGL.svg?style=for-the-badge
[license-url]: https://github.com/devniteshkumar/FoldGL/blob/main/LICENSE

