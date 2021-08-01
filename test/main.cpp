#include "GLFW/glfw3.h"

int main()
{
  if (!glfwInit())
    return -1;
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LilWindow", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    
    glfwSwapBuffers(window);
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
}
