#include <glad/glad.h>
#include "renderer.hpp"

vertexBuffer::vertexBuffer(const void *data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

vertexBuffer::~vertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

vertexArray::vertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

vertexArray::~vertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void vertexArray::AddBuffer(const vertexBuffer &vb, const vertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const auto &elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void *)(uintptr_t)offset);
        offset += element.count * vertexBufferElement::getSizeOfType(element.type);
    }
}

void vertexArray::bind() const
{
    glBindVertexArray(m_RendererID);
}

void vertexArray::unbind() const
{
    glBindVertexArray(0);
}

void vertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void vertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

indexBuffer::indexBuffer(const unsigned int *data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

indexBuffer::~indexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void indexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void indexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

vertexBufferLayout::vertexBufferLayout() : m_Stride(0) {}

template <>
void vertexBufferLayout::push<float>(unsigned int count)
{
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += count * vertexBufferElement::getSizeOfType(GL_FLOAT);
}

template <>
void vertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * vertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <>
void vertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += count * vertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

