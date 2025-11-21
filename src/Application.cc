#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main (int argc, char *argv[]) {

  GLFWwindow *window;
  if(!glfwInit()){
    return -1;
  }

  window = glfwCreateWindow(1920, 1080, "Hello world", nullptr, nullptr);
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

  float positions[6] = {
    -0.5, -0.5,
    0.5, 0.5,
    0.5, -0.5
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  while(!glfwWindowShouldClose(window)){

    //glclear在OpenGL库里
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
