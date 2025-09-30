#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/camera.hpp"
#include "pdb/model.hpp"
#include <vector>
#include <iostream>
#include "utils/fileio.hpp"

// Mouse state
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
Camera *g_camera = nullptr;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;
    if (g_camera)
        g_camera->ProcessMouseMovement(xoffset, yoffset);
}

// Model to Mesh
Mesh modelToMesh(const pdb::Model &model)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<int, unsigned int> serialToIndex; // atom.serial -> vertex index

    // 1. Atoms → vertices
    for (const auto &atom : model.atoms)
    {
        Vertex v;
        v.Position = glm::vec3(atom->x, atom->y, atom->z);

        // Normal placeholder: if drawing as points/lines, normals don't matter
        // If later you do spheres/surfaces, you’ll want real normals
        v.Normal = glm::normalize(v.Position);

        serialToIndex[atom->serial] = static_cast<unsigned int>(vertices.size());
        vertices.push_back(v);
    }

    // 2. Connections → indices
    for (const auto &conn : model.connections)
    {
        auto it1 = serialToIndex.find(conn->serial1);
        auto it2 = serialToIndex.find(conn->serial2);

        if (it1 != serialToIndex.end() && it2 != serialToIndex.end())
        {
            indices.push_back(it1->second);
            indices.push_back(it2->second);
        }
    }
    return Mesh(vertices, indices);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard('W', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard('S', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard('A', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard('D', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard('Q', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard('E', deltaTime);
}

void setShaderUniforms(Shader &shader, Camera &camera, glm::vec3 lightPos)
{
    // glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.GetPosition());
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("objectColor", glm::vec3(0.2f, 0.6f, 1.0f));
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "3D Mesh Renderer", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set mouse callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader meshShader(fileio_getpath("shader/mesh.vert", 1), fileio_getpath("shader/mesh.frag", 1));

    // Load PDB
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <pdb_file>\n";
        return 1;
    }

    std::ifstream pdbFile(argv[1]);
    if (!pdbFile)
    {
        std::cerr << "Error: could not open file " << argv[1] << "\n";
        return 1;
    }
    pdb::Reader reader(pdbFile);
    auto model = reader.read();
    if (!model)
    {
        std::cerr << "Error: failed to read PDB file" << std::endl;
        return 1;
    }
    std::cout << "Atoms: " << model->atoms.size() << " Connections: " << model->connections.size() << std::endl;

    Mesh cube = modelToMesh(*model);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    Camera camera;
    g_camera = &camera;
    glfwSetCursorPosCallback(window, mouse_callback);
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        meshShader.autoreload();
        meshShader.use();
        setShaderUniforms(meshShader, camera, lightPos);
        cube.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
