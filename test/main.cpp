#include "lilGUI.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main()
{
  if (!glfwInit())
    return -1;
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LilWindow", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGL())
    return -1;
  
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwSwapBuffers(window);
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
}
