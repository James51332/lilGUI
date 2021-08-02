#pragma once

#include "lilRenderer.h"

#include <glad/glad.h>

class LilRenderer
{
public:
  static void Init();
  static void Terminate();
  
  static void Begin();
  static void End();
  
  static void OnResize(float width, float height);
  
private:
  struct LilRendererData
  {
    GLuint VAO, VBO, IBO, ShaderProgram;
  };
  
  static LilRendererData s_Data;
};
