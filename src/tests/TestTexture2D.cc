#include "TestTexture2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <memory>
namespace test{

  TestTexture2D::TestTexture2D()
    :m_Proj(glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f)), 
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f))), 
    m_TranslationA(0.5f, 0.5f, 0.0f), m_TranslationB(1.2f, 1.2f, 0.0f)
  {

    float positions[] = {
      -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, 1.0f, 0.0f, 
      0.5f,  0.5f,1.0f,1.0f,
      -0.5f, 0.5f,0.0f,1.0f
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    m_VAO->AddBuffer(*m_VertexBuffer, layout);//里面已经bind m_VAO
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    m_Shader->Bind();
    m_Texture = std::make_unique<Texture>("res/textures/a.png");
    m_Shader->SetUniform1i("u_Texture", 0);
  }

  void TestTexture2D::OnImGuiRender()  {
    ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  void TestTexture2D::OnUpdate(float deltaTime)  {
   
  }
   
  void TestTexture2D::OnRender()  {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f),m_TranslationA);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f),m_TranslationB);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }
  }

  TestTexture2D::~TestTexture2D()  {
   
  }
}


 

 

