#include "figure.hpp"

Figure::Figure() : offset({ 0.0f, 0.0f })  // Initialize the offset
{
    vertices.push_back({ -0.5f, -0.5f });
    vertices.push_back({ 0.5f, -0.5f });
    vertices.push_back({ 0.0f, 0.5f });
    num_vertex = 3;
}

Figure::Figure(std::vector<Vec2>& customVertices) : offset({ 0.0f, 0.0f })
{
    setVertices(customVertices);
}

Figure::~Figure()
{
    vertices.clear();
}

void Figure::setVertices(std::vector<Vec2>& customVertices)
{
    vertices = customVertices;
    num_vertex = vertices.size();
}

void Figure::setPosition(Vec2 position)
{
    pos = position;  // Base position for the figure
}

void Figure::setOffset(Vec2 newOffset)
{
    offset = newOffset;  // Set the offset for moving the figure
}

Figure::Vec2 Figure::getOffset()
{
    return offset;
}

void Figure::drawFigure()
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < num_vertex; i++) {
        // Apply the offset when drawing each vertex
        glVertex2f(vertices[i].x + offset.x, vertices[i].y + offset.y);
    }
    glEnd();
}


