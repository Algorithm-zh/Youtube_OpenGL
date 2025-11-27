#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include <GL/glew.h>
#include <signal.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#ifdef Debug
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class Renderer{

private:

public:

  void Clear() const ;
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
