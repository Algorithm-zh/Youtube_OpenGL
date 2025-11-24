#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource{
  std::string VertexSource;
  std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string &filePath){
  
  std::fstream stream(filePath);
  enum class ShaderType{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  std::string line;
  ShaderType type = ShaderType::NONE;
  std::stringstream ss[2];

  while(std::getline(stream, line)){
    if(line.find("#shader") != std::string::npos){
      if(line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if(line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }else{
      ss[(int)type] << line << "\n"; 
    }
  }
  return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source){

  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  //Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
    //获取错误信息
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    //在栈上分配一个内存区域(少见的写法， 一般人可能直接就写个在堆上的了malloc)
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " <<
          (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){

  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}


int main (int argc, char *argv[]) {

  GLFWwindow *window;
  if(!glfwInit()){
    return -1;
  }

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

  float positions[6] = {
    -0.5, -0.5,
    0, 0.5,
    0.5, -0.5
  };

  //将顶点数据存储到gpu
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  //启用顶点属性0并告诉opengl如何解析顶点数据
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  glBindBuffer(GL_ARRAY_BUFFER, 0);


  while(!glfwWindowShouldClose(window)){

    //glclear在OpenGL库里
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteShader(shader);

  glfwTerminate();
  return 0;
}
