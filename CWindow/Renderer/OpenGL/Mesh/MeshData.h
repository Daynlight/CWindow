#pragma once

#include <vector>
#include <unordered_map>


namespace CW::Renderer{
class MeshData{
private:
  std::vector<char> data;
  unsigned int dimension = 0;
  unsigned int size_of_element = 0;
  GLenum type;

public:
  MeshData(){};
  MeshData(std::vector<char> data, unsigned int dimension, unsigned int size_of_element, GLenum type)
  : data(data), dimension(dimension), size_of_element(size_of_element), type(type){};

  unsigned int getSize(){
    return data.size();
  };

  unsigned int getSizeOfElement(){
    return size_of_element;
  };

  unsigned int getDimension(){
    return dimension;
  };

  char operator[](size_t index){
    return data[index]; 
  };

  GLenum getType(){
    return type;
  };

};
};
