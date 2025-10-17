#include "OpenGL_Renderer.h"

CW::Renderer::Renderer::Renderer() {};

CW::Renderer::Renderer::~Renderer()
{
  windowData.should_close = false;
  if (compiledShader) glDeleteProgram(compiledShader);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteBuffers(1, &VAO);
  if (EBO) glDeleteBuffers(1, &EBO);
  if (pointsSSBO) glDeleteBuffers(1, &pointsSSBO);
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
};

void CW::Renderer::Renderer::windowMovement(std::pair<float, float> move) {

};

void CW::Renderer::Renderer::windowZoom(float zoom) {
};

void CW::Renderer::Renderer::windowEvents()
{
  glfwPollEvents();
  if(glfwWindowShouldClose(window)) windowData.should_close = false;


  // Update Window Info
  int width, height, x, y;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, windowData.width, windowData.height);
  glfwGetWindowPos(window, &x, &y);

  // Save Data
  windowData.width = width;
  windowData.height = height;
  windowData.x = x;
  windowData.y = y;
}

const CW::Renderer::WindowData *CW::Renderer::Renderer::getWindowData() {
  return &windowData;
}

void CW::Renderer::Renderer::createWindow()
{

  if(!glfwInit()) {
    windowData.should_close = false;
    glfwTerminate();
    throw std::runtime_error("Can't initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  window = glfwCreateWindow(800, 600, "Curve", nullptr, nullptr);
  if(!window) {
    windowData.should_close = false;
    glfwTerminate();
    throw std::runtime_error("Can't Create Window");
  };

  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(0);

  windowData.should_close = true;
}

APIWindow* CW::Renderer::Renderer::getWindow()
{
  return window;
}

void CW::Renderer::Renderer::createRenderer()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    windowData.should_close = false;
    return;
  };

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLfloat quadVertices[] = {
    // Positions
    -1.0f,  1.0f,  // Top-left
    -1.0f, -1.0f,  // Bottom-left
    1.0f,  1.0f,  // Top-right
    1.0f, -1.0f,  // Bottom-right
  };

  GLuint indices[] = {
    0, 1, 2,  // First triangle
    1, 3, 2   // Second triangle
  };

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
}

void CW::Renderer::Renderer::renderFrame() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
  glUseProgram(compiledShader);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void CW::Renderer::Renderer::swapBuffer()
{
  glfwSwapBuffers(window);
}

void CW::Renderer::Renderer::compileShaders() {
  GLuint vertexShaderPart = glCreateShader(GL_VERTEX_SHADER);
  const char* vertexShaderData = vertexShader.c_str();
  glShaderSource(vertexShaderPart, 1, &vertexShaderData, nullptr);
  glCompileShader(vertexShaderPart);
  GLuint fragmentShaderPart = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragmentShaderData = fragmentShader.c_str();
  glShaderSource(fragmentShaderPart, 1, &fragmentShaderData, nullptr);
  glCompileShader(fragmentShaderPart);
  compiledShader = glCreateProgram();
  glAttachShader(compiledShader, vertexShaderPart);
  glAttachShader(compiledShader, fragmentShaderPart);
  glLinkProgram(compiledShader);
  glDeleteShader(vertexShaderPart);
  glDeleteShader(fragmentShaderPart);
}

void CW::Renderer::Renderer::bindComputeShader(std::string shader) {
  computeShader = shader;
}

void CW::Renderer::Renderer::runComputeShader(std::vector<float> data)
{
  GLuint computeShaderPart = glCreateShader(GL_COMPUTE_SHADER);
  const char* computeShaderData = computeShader.c_str();
  glShaderSource(computeShaderPart, 1, &computeShaderData, nullptr);
  glCompileShader(computeShaderPart);
  GLuint program = glCreateProgram();
  glAttachShader(program, computeShaderPart);
  glLinkProgram(program);
  glUseProgram(program);
    
  glGenBuffers(1, &pointsSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointsSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);

  glDispatchCompute(data.size(), 1, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  glDeleteShader(computeShaderPart);
  glDeleteProgram(program);
}

void CW::Renderer::Renderer::bindVertexShader(std::string shader)
{
  vertexShader = shader;
}

void CW::Renderer::Renderer::bindFragmentShader(std::string shader) {
  fragmentShader = shader;
};