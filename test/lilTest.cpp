#include "lilRenderer.h"

#include <lilGUI.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
  // 1) Iniitalize GLFW
  if (!glfwInit())
  {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  
  // 2) Create Window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef LIL_MACOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "LilWindow", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  // 3) Set Resize Callback
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
  {
    LilRenderer::OnResize(static_cast<float>(width), static_cast<float>(height));
  });
  
  // 3) Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  
  // 4) Initialize Renderer
  LilRenderer::Init();
  
  // 5) Run Loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
      
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    LilRenderer::Begin();
    
    // Temporary API
    Lil::GetDrawLists()[0].PushRect({-0.75f, -0.5f}, {0.0f, 0.0f});
    Lil::GetDrawLists()[0].PushRect({0.25f, 0.25f}, {0.5f, 0.5f}, Lil::ColorFromRGBA(1.0f, 1.0f, 0.0f, 1.0f));
    Lil::GetDrawLists()[0].PushRect({-0.5f, 0.5f}, {0.0f, 1.0f});
    
    LilRenderer::End();

    glfwSwapBuffers(window);
  }
  
  // 6) Shut Down
  LilRenderer::Terminate();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
