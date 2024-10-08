#include <GLFW/glfw3.h>
#include <vector>

class Figure
{
  struct Vec2 {
    float x, y;
  };
public:
    Figure();
    ~Figure();
    void drawFigure(); // New method to draw a triangle

private:
  std::vector<Vec2> vertices;
  int num_vertex;
};
