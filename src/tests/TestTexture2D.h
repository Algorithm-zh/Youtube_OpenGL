#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "Test.h"
#include "VertexArray.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace test{
  class TestTexture2D : public Test{
  public:
    TestTexture2D();
    ~TestTexture2D();

    void OnImGuiRender() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

  private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    glm::mat4 m_Proj, m_View;
    glm::vec3 m_TranslationA, m_TranslationB;
  };

}
