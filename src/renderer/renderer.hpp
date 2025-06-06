#pragma once

#include "renderer/buffers.hpp"
#include "renderer/shader.hpp"

class renderer
{
public:
    static void clear();
    static void draw(const vertexArray &va, const indexBuffer &ib, Shader &shader);
};