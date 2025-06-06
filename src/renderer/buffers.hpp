#pragma once

#include <vector>
#include <glad/glad.h>

class vertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    vertexBuffer(const void *data, unsigned int size);
    ~vertexBuffer();
    void bind() const;
    void unbind() const;
};

class indexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    indexBuffer(const unsigned int *data, unsigned int count);
    ~indexBuffer();
    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return m_Count; }
};

struct vertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(float);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
        default:
            return 0;
        }
    }
};

class vertexBufferLayout
{
private:
    std::vector<vertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    vertexBufferLayout();

    template <typename T>
    void push(unsigned int count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for vertexBufferLayout::push()");
    }

    inline const std::vector<vertexBufferElement> getElements() const { return m_Elements; }
    inline unsigned int getStride() const { return m_Stride; }
};

class vertexArray
{
private:
    unsigned int m_RendererID;

public:
    vertexArray();
    ~vertexArray();
    void AddBuffer(const vertexBuffer &vb, const vertexBufferLayout &layout);
    void bind() const;
    void unbind() const;
};

template <>
void vertexBufferLayout::push<float>(unsigned int count);

template <>
void vertexBufferLayout::push<unsigned int>(unsigned int count);

template <>
void vertexBufferLayout::push<unsigned char>(unsigned int count);
