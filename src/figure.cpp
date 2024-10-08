#include "figure.hpp"

Figure::Figure()
{
  vertices.push_back({ -0.5f, -0.5f });
  vertices.push_back({ 0.5f, -0.5f });
  vertices.push_back({ 0.0f, 0.5f });
  num_vertex = 3;
}

Figure::~Figure()
{
  vertices.clear();
}

void Figure::drawTriangle()
{
  // Define the vertices for the triangle
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < num_vertex; i++) {
    glVertex2f(vertices[i].x, vertices[i].y);
  }
  //glVertex2f(-0.5f, -0.5f); // Bottom left vertex
  //glVertex2f(0.5f, -0.5f);  // Bottom right vertex
  //glVertex2f(0.0f, 0.5f);   // Top vertex
  glEnd();
}
