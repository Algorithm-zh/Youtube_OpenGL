#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#define Debug

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#ifdef Debug
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x
#endif

static void GLClearError(){
  while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line){
  while(GLenum error = glGetError()){
    std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

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

  GLCall(unsigned int program = glCreateProgram());
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
  unsigned int VAO;
  GLCall(glGenVertexArrays(1, &VAO));
  GLCall(glBindVertexArray(VAO));

  //将顶点数据存储到gpu
  unsigned int VBO;
  GLCall(glGenBuffers(1, &VBO));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

  //启用顶点属性0并告诉opengl如何解析顶点数据
  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));


  //创建索引缓冲区，记录顶点绘制顺序
  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  // 使用uniform设置颜色
  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  GLCall(glUniform4f(location, 0.8f, 0.2f, 0.7f, 1.0f));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  float r = 0.0f;
  float increment = 0.05f;

  while(!glfwWindowShouldClose(window)){

    //glclear在OpenGL库里
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, r, 0.2f, 0.7f, 1.0f));

    GLCall(glBindVertexArray(VAO));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if(r > 1.0f)
      increment = -0.05f;
    else if(r < 0.0f)
      increment = 0.05f;

    r += increment;

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteShader(shader);

  glfwTerminate();
  return 0;
}
