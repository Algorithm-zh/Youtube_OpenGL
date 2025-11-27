#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type){
    switch(type){
      case GL_FLOAT:         return 4;
      case GL_UNSIGNED_INT:  return 4;
      case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(false);
    return 0;
  }
};
class VertexBufferLayout{

private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

public:
  VertexBufferLayout()
    :m_Stride(0){}

  //c++ 17语法 if constexpr
  template<typename T>
  void Push(unsigned int count){
    if constexpr (std::is_same_v<T, float>){
      m_Elements.push_back({GL_FLOAT, count, false});
    }
    else if constexpr (std::is_same_v<T, unsigned int>){
      m_Elements.push_back({GL_UNSIGNED_INT, count, false});
    }
    else if constexpr (std::is_same_v<T, unsigned char>){
      m_Elements.push_back({GL_UNSIGNED_BYTE, count, true});
    }
    else{
      static_assert(sizeof(T) == 0, "Unsupport type in Push<T>");
    }
  }

  inline const std::vector<VertexBufferElement>& GetElements() const {return m_Elements;}
  inline unsigned int GetStride() const {return m_Stride;}

};

