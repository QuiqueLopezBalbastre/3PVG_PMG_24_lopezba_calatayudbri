#include "figure.hpp"

Figure::Figure()
{
}

Figure::~Figure()
{
}

void Figure::drawTriangle()
{
    // Define the vertices for the triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f); // Bottom left vertex
    glVertex2f(0.5f, -0.5f);  // Bottom right vertex
    glVertex2f(0.0f, 0.5f);   // Top vertex
    glEnd();
}
