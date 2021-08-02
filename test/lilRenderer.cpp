#include "lilRenderer.h"

#include <lilGUI.h>
#include <iostream>

LilRenderer::LilRendererData LilRenderer::s_Data;

void LilRenderer::Init()
{
  
  // 1) Data Sources
  static const char *vertexSource = R"(
  #version 330 core

  layout (location = 0) in vec3 a_Pos;
  layout (location = 1) in vec2 a_UV;
  layout (location = 2) in vec4 a_Color;
  
  out vec4 u_Color;

  void main()
  {
    u_Color = a_Color;
    gl_Position = vec4(a_Pos, 1.0);
  })";

  static const char *fragmentSource = R"(
  #version 330 core

  in vec4 u_Color;
  
  out vec4 FragColor;

  void main()
  {
    FragColor = u_Color;
  })";

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
    std::cout << "Shader Failed to Link" << infoLog << std::endl;
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
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LilVtx), (void*)0);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LilVtx), (void*)sizeof(LilVec3));
  glEnableVertexAttribArray(1);
  
  glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(LilVtx), (void*)(sizeof(LilVec3) + sizeof(LilVec2)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  // 6) Create LilContext
  Lil::CreateContext();
}

void LilRenderer::Terminate()
{
  Lil::DestroyContext();
  
  glDeleteVertexArrays(1, &s_Data.VAO);
  glDeleteBuffers(1, &s_Data.VBO);
  glDeleteBuffers(1, &s_Data.IBO);
  glDeleteProgram(s_Data.ShaderProgram);
}

void LilRenderer::Begin()
{
  for (auto& drawList : Lil::GetDrawLists())
  {
    drawList.Clear();
  }
}

void LilRenderer::End()
{
  
  for (auto& drawList : Lil::GetDrawLists())
  {
  
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 drawList.VtxArray.GetSize() * sizeof(LilVtx),
                 &drawList.VtxArray[0],
                 GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 drawList.IdxArray.GetSize() * sizeof(LilIdx),
                 &drawList.IdxArray[0],
                 GL_STREAM_DRAW);
    
    glUseProgram(s_Data.ShaderProgram);
    glBindVertexArray(s_Data.VAO);
 
    // already bound
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
    
    glDrawElements(GL_TRIANGLES, drawList.IdxArray.GetSize(), GL_UNSIGNED_SHORT, 0);
    
  }
  
  glUseProgram(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LilRenderer::OnResize(float width, float height)
{
  glViewport(0, 0, width, height);
}

