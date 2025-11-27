#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define Debug


int main (int argc, char *argv[]) {

  GLFWwindow *window;
  if(!glfwInit()){
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  window = glfwCreateWindow(640, 480, "Hello world", nullptr, nullptr);
  if(!window){
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  //必须在创建opengl渲染上下文之后再初始化glew才可以
  if(glewInit() != GLEW_OK){
    std::cout << "Error init glew" << std::endl;
  }
  std::cout << glGetString(GL_VERSION) << std::endl;
  {
    float positions[] = {
      -0.5, -0.5,
      0.5, -0.5,
      0.5, 0.5,
      -0.5, 0.5
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    //使用核心模式之后，必须创建VAO进行绑定，否则会报错
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);//里面已经bind va

    //创建索引缓冲区，记录顶点绘制顺序
    IndexBuffer ib(indices, 6);

    //创建着色器程序
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    shader.Unbind();
    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;

    while(!glfwWindowShouldClose(window)){

      renderer.Clear();
      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.5f, 0.3f, 1.0f);

      renderer.Draw(va, ib, shader);

      if(r > 1.0f)
        increment = -0.05f;
      else if(r < 0.0f)
        increment = 0.05f;

      r += increment;

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}
