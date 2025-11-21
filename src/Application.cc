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

  while(!glfwWindowShouldClose(window)){

    //glclear在OpenGL库里
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
