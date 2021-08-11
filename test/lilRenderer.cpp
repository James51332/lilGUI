#include "lilRenderer.h"

#include <lilGUI.h>
//#include <iostream>

LilRenderer::LilRendererData LilRenderer::s_Data;

void LilRenderer::Init()
{
  
  // 1) Data Sources
  static const char *vertexSource = R"(
  #version 330 core

  layout (location = 0) in vec3 a_Pos;
  layout (location = 1) in vec2 a_UV;
  layout (location = 2) in vec4 a_Color;
  
  out vec4 v_Color;
  out vec2 v_UV;

  void main()
  {
    v_Color = a_Color;
    v_UV = a_UV;
  
    gl_Position = vec4(a_Pos, 1.0);
  })";

  static const char *fragmentSource = R"(
  #version 330 core

  in vec4 v_Color;
  in vec2 v_UV;
  
  out vec4 FragColor;
  
  uniform sampler2D u_Texture;

  void main()
  {
    FragColor = v_Color * texture(u_Texture, v_UV.xy);
  })";

  // 2) Init GL State
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // 3) Compile Shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);
  
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      //std::cout << "Vertex Shader Failed to Compile:" << std:://endl << infoLog << std::endl;
  }
  
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //std::cout << "Fragment Shader Failed to Compile:" << std::endl << infoLog << std::endl;
  }

  // 4) Link Shaders
  s_Data.ShaderProgram = glCreateProgram();
  glAttachShader(s_Data.ShaderProgram, vertexShader);
  glAttachShader(s_Data.ShaderProgram, fragmentShader);
  glLinkProgram(s_Data.ShaderProgram);

  glGetProgramiv(s_Data.ShaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(s_Data.ShaderProgram, 512, NULL, infoLog);
    //std::cout << "Shader Failed to Link" << infoLog << std::endl;
  }
  
  // 5) Clean Up Shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  // 6) Set Up Vertex Array
  glGenVertexArrays(1, &s_Data.VAO);
  glGenBuffers(1, &s_Data.VBO);
  glGenBuffers(1, &s_Data.IBO);
  
  glBindVertexArray(s_Data.VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
  glBufferData(GL_ARRAY_BUFFER, 250 * 1024, nullptr, GL_DYNAMIC_DRAW); // 250kb buffer (if we're full, the cpu is probably already dead...)
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 50 * 1024, nullptr, GL_DYNAMIC_DRAW); // 50kb buffer
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LilVtx), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LilVtx), (void*)sizeof(LilVec3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(LilVtx), (void*)(sizeof(LilVec3) + sizeof(LilVec2)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  // 7) Generate Texture
  glGenTextures(1, &s_Data.TextureID);
  glBindTexture(GL_TEXTURE_2D, s_Data.TextureID);
  
  uint32_t data = 0xffffffff;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
  glGenerateMipmap(GL_TEXTURE_2D); // CRUCIAL LINE ON MACOS
  
  // 8) Create LilContext
  Lil::CreateContext();
}

void LilRenderer::Terminate()
{
  Lil::DestroyContext();
  
  glDeleteVertexArrays(1, &s_Data.VAO);
  glDeleteBuffers(1, &s_Data.VBO);
  glDeleteBuffers(1, &s_Data.IBO);
  glDeleteProgram(s_Data.ShaderProgram);
  glDeleteTextures(1, &s_Data.TextureID);
}

void LilRenderer::Begin()
{
  Lil::BeginFrame();
}

void LilRenderer::End()
{
  Lil::RenderFrame();
  
  for (auto& drawList : Lil::GetDrawLists())
  {
    glUseProgram(s_Data.ShaderProgram);
//    GLint ts = glGetUniformLocation(s_Data.ShaderProgram, "u_Texture");
//    glUniform1i(ts, 0);
    
    
    typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    drawList.VtxArray.GetSize() * sizeof(LilVtx),
                    &drawList.VtxArray[0]);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.IBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                    0,
                    drawList.IdxArray.GetSize() * sizeof(LilIdx),
                    &drawList.IdxArray[0]);
    
    glBindVertexArray(s_Data.VAO);
    
    for (auto& command : drawList.DrawCmds)
    {
      //glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, s_Data.TextureID);
      //if (command.TextureID)
        //glBindTexture(GL_TEXTURE_2D, command.TextureID);
      
      glDrawElements(GL_TRIANGLES, static_cast<int>(command.Size), GL_UNSIGNED_SHORT, (const void*)static_cast<long>(command.IdxOffset));
    }
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

