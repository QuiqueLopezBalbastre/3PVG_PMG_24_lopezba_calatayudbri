#ifndef __FIGURE__
#define __FIGURE__ 1

#include <GLFW/glfw3.h>
#include <vector>
#include "Input.hpp"



class Figure {
public:
  struct Vec2 {
    float x, y;
  }position, offset;

  Figure();
  Figure(std::vector<Vec2>& customVertices);
  ~Figure();


  void setPosition(Vec2 position);  // Set the base position
  void setOffset(Vec2 offset);      // Set an offset to move the figure
  Vec2 getOffset();
  void drawFigure();                //Draw triangle
  void setVertices(std::vector<Vec2>& customVertices);
  void moveFigure(Input& input);

private:
  std::vector<Vec2> vertices;

  int num_vertex;
};

#endif