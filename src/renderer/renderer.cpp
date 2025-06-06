#include "renderer.hpp"

void renderer::draw(const vertexArray &va, const indexBuffer &ib, Shader &shader)
{
    shader.use();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}