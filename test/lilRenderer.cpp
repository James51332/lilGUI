#include "lilRenderer.h"

#include <lilGUI.h>
#include <iostream>

LilRenderer::LilRendererData LilRenderer::s_Data;

void LilRenderer::Init()
{
  
  // 1) Data Sources
  static const char *vertexSource = R"(
  #version 330 core

  layout (location = 0) in vec3 aPos;

  void main()
  {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  })";

  static const char *fragmentSource = R"(
  #version 330 core

  out vec4 FragColor;

  void main()
  {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  })";
  
  static float vertices[] = {
       0.5f,  0.5f, 0.0f,  // top right
       0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left
  };
  
  static unsigned short indices[] = {
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
  };
  
  // 2) Compile Shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);
  
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      std::cout << "Vertex Shader Failed to Compile:" << std::endl << infoLog << std::endl;
  }
  
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Fragment Shader Failed to Compile:" << std::endl << infoLog << std::endl;
  }

  // 3) Link Shaders
  s_Data.ShaderProgram = glCreateProgram();
  glAttachShader(s_Data.ShaderProgram, vertexShader);
  glAttachShader(s_Data.ShaderProgram, fragmentShader);
  glLinkProgram(s_Data.ShaderProgram);

  glGetProgramiv(s_Data.ShaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(s_Data.ShaderProgram, 512, NULL, infoLog);
    std::cout << " " << infoLog << std::endl;
  }
  
  // 4) Clean Up Shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  // 5) Set Up Vertex Array
  glGenVertexArrays(1, &s_Data.VAO);
  glGenBuffers(1, &s_Data.VBO);
  glGenBuffers(1, &s_Data.IBO);
  
  glBindVertexArray(s_Data.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LilRenderer::Terminate()
{
  glDeleteVertexArrays(1, &s_Data.VAO);
  glDeleteBuffers(1, &s_Data.VBO);
  glDeleteBuffers(1, &s_Data.IBO);
  glDeleteProgram(s_Data.ShaderProgram);
}

void LilRenderer::Begin()
{
  glUseProgram(s_Data.ShaderProgram);
  glBindVertexArray(s_Data.VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void LilRenderer::End()
{
  
}

void LilRenderer::OnResize(float width, float height)
{
  glViewport(0, 0, width, height);
}

