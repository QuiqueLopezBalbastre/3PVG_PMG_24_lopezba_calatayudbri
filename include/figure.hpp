#ifndef DRAW_FIGURE_HPP
#define DRAW_FIGURE_HPP 1

#include <GLFW/glfw3.h>
#include <vector>

class Figure
{
public:
  struct Vec2 {
    float x, y;
  }pos,offset;
  
  Figure();
  Figure(std::vector<Vec2>& customVertices);
  ~Figure();


  void setPosition(Vec2 position);  // Set the base position
  void setOffset(Vec2 offset);      // Set an offset to move the figure
  Vec2 getOffset();
  void drawFigure();                //Draw triangle
  void setVertices(std::vector<Vec2>& customVertices);
 

private:
  std::vector<Vec2> vertices;
  int num_vertex;
};

#endif