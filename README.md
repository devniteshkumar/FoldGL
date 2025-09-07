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

FoldGL is a high-performance 3D protein structure visualization tool built with modern OpenGL. It provides comprehensive support for parsing and rendering Protein Data Bank (PDB) files with an intuitive camera system for interactive exploration of molecular structures.

**Key Features:**
- **PDB File Parsing**: Complete support for standard PDB format files
  - Atomic coordinates and properties
  - Residue and chain information
  - Secondary structure elements (helices, strands, coils)
  - Multiple model support
- **3D Visualization**: Real-time OpenGL rendering
  - Modern shader-based rendering pipeline
  - Smooth camera controls with mouse and keyboard
  - 3D mesh rendering with proper lighting
- **Molecular Structure Support**:
  - Atom-level visualization
  - Residue-based organization
  - Chain and model hierarchy
  - Secondary structure representation
- **Development Features**:
  - Hot-reload shader support for real-time development
  - Memory-safe implementation with Address Sanitizer support
  - Modular architecture with clean separation of concerns

**Dependencies:**
This project uses the following external libraries as git submodules:
- **GLAD** - OpenGL loader library for modern OpenGL functions
- **GLFW** - Cross-platform window and input handling
- **GLM** - OpenGL Mathematics library for vector/matrix operations

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
   git clone <your-foldgl-repository-url> --recurse-submodules
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
6. Run the application
   ```sh
   ./build/ogt
   ```

**Alternative:** If you are using VSCode, you can use the CMake extension and run the build task (generally, `Ctrl + Shift + B`).

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

FoldGL provides an interactive 3D environment for visualizing protein structures from PDB files. Use the intuitive camera controls to explore molecular structures in real-time with smooth navigation and lighting.

**Camera Controls:**
- **W/A/S/D**: Move camera forward/left/backward/right
- **Q/E**: Move camera up/down
- **Mouse Movement**: Look around (first-person camera)
- **ESC**: Exit application

**Getting Started:**
1. Launch the application using `./build/ogt`
2. The application will start with a simple 3D scene
3. Use the camera controls to navigate around the 3D space
4. *Note: PDB file loading functionality is currently in development*

<p align="right">(<a href="#top">back to top</a>)</p>

## Project Structure

```
.
├── build/                      # Build output directory
│   └── ogt                     # Compiled executable
├── external/                   # External dependencies (git submodules)
│   ├── glad/                   # OpenGL loader library
│   ├── glfw/                   # Window and input handling
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
    └── utils/                  # Utility functions
        ├── fileio.hpp/cpp      # File I/O operations
        ├── FileWatch.hpp       # Hot-reload file watching
        └── stb_image.h         # Image loading library
```

<p align="right">(<a href="#top">back to top</a>)</p>

## TODO

### Core Features
- [ ] **PDB File Loading and Integration**
  - [ ] Command-line argument support for PDB file input
  - [ ] File dialog for interactive PDB file selection
  - [ ] Integration of PDB parser with rendering system
  - [ ] Error handling for malformed PDB files
  - [ ] Support for compressed PDB files (.pdb.gz)

- [ ] **Protein Visualization**
  - [ ] Atom-based rendering (spheres/points)
  - [ ] Bond visualization (cylinders/lines)
  - [ ] Multiple representation modes:
    - [ ] Ball-and-stick model
    - [ ] Space-filling model
    - [ ] Ribbon/cartoon representation
    - [ ] Wireframe model
  - [ ] Secondary structure highlighting
    - [ ] Alpha helices (cylinders/ribbons)
    - [ ] Beta strands (arrows/sheets)
    - [ ] Loops and coils

- [ ] **Interactive Features**
  - [ ] Atom/residue selection and highlighting
  - [ ] Information display for selected atoms/residues
  - [ ] Distance and angle measurements
  - [ ] Multiple protein loading and comparison
  - [ ] Animation support for multi-model PDB files

### User Interface
- [ ] **GUI Development**
  - [ ] Integrate ImGui for control panels
  - [ ] File browser widget
  - [ ] Visualization controls:
    - [ ] Representation mode selection
    - [ ] Color scheme options
    - [ ] Visibility toggles for chains/residues
    - [ ] Lighting and material properties
  - [ ] Information panels:
    - [ ] Protein metadata display
    - [ ] Selection details
    - [ ] Statistics and analysis

- [ ] **Camera and Navigation**
  - [ ] Improved camera system:
    - [ ] Orbit/trackball navigation
    - [ ] Zoom to selection
    - [ ] Predefined viewpoints
    - [ ] Smooth camera transitions
  - [ ] View presets (front, back, top, etc.)
  - [ ] Camera position saving/loading

### Performance and Optimization
- [ ] **Rendering Optimization**
  - [ ] Level-of-detail (LOD) system for large proteins
  - [ ] Instanced rendering for repeated elements
  - [ ] Frustum culling for off-screen atoms
  - [ ] GPU-based atom/bond generation
  - [ ] Texture-based color schemes

- [ ] **Memory Management**
  - [ ] Efficient data structures for large proteins
  - [ ] Streaming for extremely large datasets
  - [ ] Memory pool allocation
  - [ ] Garbage collection for unused resources

### Advanced Features
- [ ] **Analysis Tools**
  - [ ] Ramachandran plot visualization
  - [ ] Hydrogen bond detection and display
  - [ ] Surface area calculations
  - [ ] Cavity and pocket detection
  - [ ] Molecular dynamics trajectory support

- [ ] **Export and Sharing**
  - [ ] High-resolution image export
  - [ ] 3D model export (OBJ, STL)
  - [ ] Animation export (video/GIF)
  - [ ] Session save/load functionality
  - [ ] Web-based sharing capabilities

### Technical Improvements
- [ ] **Code Quality**
  - [ ] Comprehensive unit testing
  - [ ] Automated integration tests
  - [ ] Performance benchmarking
  - [ ] Memory leak detection
  - [ ] Code documentation with Doxygen

- [ ] **Platform Support**
  - [ ] Linux support optimization
  - [ ] Windows compatibility
  - [ ] macOS support
  - [ ] Web Assembly port (experimental)

- [ ] **Developer Experience**
  - [ ] Plugin system architecture
  - [ ] Python scripting interface
  - [ ] Automated build system (CI/CD)
  - [ ] Development documentation
  - [ ] Contributing guidelines

### Research and Education Features
- [ ] **Educational Tools**
  - [ ] Interactive tutorials
  - [ ] Guided protein exploration
  - [ ] Annotation system for teaching
  - [ ] Quiz/assessment integration

- [ ] **Research Integration**
  - [ ] PDB database API integration
  - [ ] UniProt data integration
  - [ ] RCSB PDB metadata display
  - [ ] Literature reference linking
  - [ ] Experimental data visualization

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
  - [FoldGL Documentation](https://github.com/your-username/FoldGL/wiki) (Coming Soon)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- Note: Update these URLs to point to your actual FoldGL repository -->

[contributors-shield]: https://img.shields.io/github/contributors/your-username/FoldGL.svg?style=for-the-badge
[contributors-url]: https://github.com/your-username/FoldGL/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/your-username/FoldGL.svg?style=for-the-badge
[forks-url]: https://github.com/your-username/FoldGL/network/members
[stars-shield]: https://img.shields.io/github/stars/your-username/FoldGL.svg?style=for-the-badge
[stars-url]: https://github.com/your-username/FoldGL/stargazers
[issues-shield]: https://img.shields.io/github/issues/your-username/FoldGL.svg?style=for-the-badge
[issues-url]: https://github.com/your-username/FoldGL/issues
[license-shield]: https://img.shields.io/github/license/your-username/FoldGL.svg?style=for-the-badge
[license-url]: https://github.com/your-username/FoldGL/blob/main/LICENSE

