#include "Renderer.h"
#include "Shaders.h"


int main(){
  CW::Renderer::Renderer window;

  CW::Renderer::TextureLoader loader("../Assets/image.png");
  CW::Renderer::Texture texture;
  texture.create(loader);
  

  CW::Renderer::Mesh viewport;

  std::vector<GLfloat> vertices({
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 
    1.0f, -1.0f, 0.0f, 
  });
  viewport.addVertices(vertices, 3);

  std::vector<GLuint> indicies({
    0, 1, 2,
    1, 3, 2
  });
  viewport.addIndices(indicies);

  std::vector<GLfloat> idtx({
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
  });

  viewport.setData<GLfloat>(idtx, 2, 1, GL_FLOAT);

  CW::Renderer::Shader texture_shader(Texture::vertex, Texture::fragment);
  CW::Renderer::Uniform uniform;
  texture_shader.getUniforms().emplace_back(&uniform);
  uniform["uTexture"]->set<int>(0);


  while(!window.getWindowData()->should_close){
    window.beginFrame();

    texture.bind(0);
    texture_shader.bind();
    viewport.render();
    texture_shader.unbind();
    texture.unbind();

    window.swapBuffer();
    window.windowEvents();
  };

  return 0;
}