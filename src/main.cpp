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
#include "physics/unfold.hpp"

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
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if (g_camera)
        g_camera->ProcessMouseMovement(xoffset, yoffset);
}

// Helper functions for model to mesh conversion
std::vector<glm::vec3> getCAPositions(const pdb::Model &model)
{
    std::vector<glm::vec3> ca_positions;
    for (const auto &chain : model.chains)
    {
        for (const auto &res : chain->residues)
        {
            for (const auto &atom : res->atoms)
            {
                if (atom->name == "CA")
                {
                    ca_positions.push_back(glm::vec3(atom->x, atom->y, atom->z));
                }
            }
        }
    }
    return ca_positions;
}

std::vector<Vertex> generateTubeVertices(const std::vector<glm::vec3> &ca_positions, int segments, float radius, float maxDistance)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < ca_positions.size(); ++i)
    {
        glm::vec3 p = ca_positions[i];
        glm::vec3 dir;
        if (i == 0)
            dir = glm::normalize(ca_positions[i + 1] - p);
        else if (i == ca_positions.size() - 1)
            dir = glm::normalize(p - ca_positions[i - 1]);
        else
            dir = glm::normalize(ca_positions[i + 1] - ca_positions[i - 1]);

        glm::vec3 up = glm::vec3(0, 1, 0);
        if (fabs(glm::dot(dir, up)) > 0.99f)
            up = glm::vec3(1, 0, 0);
        glm::vec3 right = glm::normalize(glm::cross(dir, up));
        glm::vec3 normal = glm::normalize(glm::cross(right, dir));

        for (int j = 0; j < segments; ++j)
        {
            float theta = 2.0f * 3.1415926f * float(j) / float(segments);
            glm::vec3 circ = (right * cosf(theta) * radius) + (normal * sinf(theta) * radius);
            Vertex v;
            v.Position = p + circ;
            v.Normal = glm::normalize(circ);
            vertices.push_back(v);
        }
    }
    return vertices;
}

std::vector<std::vector<unsigned int>> generateTubeIndices(const std::vector<glm::vec3> &ca_positions, int segments, float maxDistance)
{
    std::vector<std::vector<unsigned int>> indices(1);
    int k = 0;
    for (size_t i = 1; i < ca_positions.size(); ++i)
    {
        float dist = glm::distance(ca_positions[i], ca_positions[i - 1]);
        if (dist > maxDistance)
        {
            indices.push_back(std::vector<unsigned int>());
            k++;
            continue;
        }
        for (int j = 0; j < segments; ++j)
        {
            int curr = (i - 1) * segments + j;
            int next = i * segments + j;
            int curr_next = (i - 1) * segments + (j + 1) % segments;
            int next_next = i * segments + (j + 1) % segments;

            indices[k].push_back(curr);
            indices[k].push_back(next);
            indices[k].push_back(curr_next);

            indices[k].push_back(curr_next);
            indices[k].push_back(next);
            indices[k].push_back(next_next);
        }
    }
    return indices;
}

std::vector<Mesh> modelToMesh(const pdb::Model &model)
{
    std::vector<glm::vec3> ca_positions = getCAPositions(model);
    std::vector<Vertex> vertices = generateTubeVertices(ca_positions, 12, 1.0f, 4.5f);
    std::vector<std::vector<unsigned int>> indices = generateTubeIndices(ca_positions, 12, 4.5f);

    std::vector<Mesh> meshes;
    meshes.reserve(indices.size());
    for (size_t i = 0; i < indices.size(); ++i)
    {
        if (indices[i].empty())
            continue;                              // skip empty groups
        meshes.emplace_back(vertices, indices[i]); // construct in-place (no extra copy)
    }

    return meshes;
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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    // Set lightPos to camera position
    shader.setVec3("lightPos", camera.GetPosition());
    shader.setVec3("viewPos", camera.GetPosition());
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    // shader.setVec3("objectColor", glm::vec3(0.2f, 0.6f, 1.0f));
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "3D Mesh Renderer", glfwGetPrimaryMonitor(), NULL);
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

    // Build unfolding simulation on CA trace
    UnfoldSim sim(*model);

    // Initial mesh from CA positions
    std::vector<Mesh> cube = modelToMesh(*model);

    // Assign a distinct color to each mesh
    std::vector<glm::vec3> meshColors;
    meshColors.reserve(cube.size());
    for (size_t i = 0; i < cube.size(); ++i)
    {
        float hue = static_cast<float>(i) / cube.size(); // 0 → 1 across all meshes

        // Convert hue → RGB with smoother, warmer tones
        float r = 0.5f + 0.5f * sinf(6.283f * (hue + 0.0f));
        float g = 0.5f + 0.5f * sinf(6.283f * (hue + 0.33f));
        float b = 0.5f + 0.5f * sinf(6.283f * (hue + 0.67f));

        // Apply slight gamma correction for depth
        r = powf(r, 1.2f);
        g = powf(g, 1.2f);
        b = powf(b, 1.2f);

        // Desaturate slightly to avoid overly bright colors
        glm::vec3 color(r, g, b);
        color = glm::mix(color, glm::vec3(0.7f), 0.15f);

        meshColors.push_back(color);
    }

    // Compute model center
    glm::vec3 center(0.0f);
    if (!model->atoms.empty())
    {
        for (const auto &atom : model->atoms)
        {
            center += glm::vec3(atom->x, atom->y, atom->z);
        }
        center /= static_cast<float>(model->atoms.size());
    }

    glm::vec3 lightPos = center + glm::vec3(1.2f, 1.0f, 2.0f);

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

        // Physics: pull ends a bit to drive unfolding and step
        sim.applyPulling(100.0f);
        sim.step(deltaTime);

        // Update mesh vertices from current CA positions
        std::vector<glm::vec3> ca_positions = sim.getCAPositions();
        std::vector<Vertex> vertices = generateTubeVertices(ca_positions, 12, 1.0f, 4.5f);
        for (auto& m : cube) m.UpdateVertices(vertices);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        meshShader.autoreload();
        meshShader.use();
        setShaderUniforms(meshShader, camera, lightPos);

        for (size_t i = 0; i < cube.size(); ++i)
        {
            meshShader.setVec3("objectColor", meshColors[i]);
            cube[i].Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
