#include "Figure.hpp"

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
  //color = Color(255, 255, 255);
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

void Figure::moveFigure(Input& input) {

  Vec2 currentOffset = getOffset();  // Obtener el offset actual de la figura

  // Detectar las acciones de movimiento definidas en Input
  if (input.isKeyPressed(input.KEY_W)) {  // Acción "adelante" (W)
    currentOffset.y += 0.01f;
  }
  if (input.isKeyPressed(input.KEY_S)) {  // Opción directa con tecla S (abajo)
    currentOffset.y -= 0.01f;
  }
  if (input.isKeyPressed(input.KEY_A)) {  // Mover izquierda
    currentOffset.x -= 0.01f;
  }
  if (input.isKeyPressed(input.KEY_D)) {  // Mover derecha
    currentOffset.x += 0.01f;
  }

  // Actualizar el offset de la figura
  setOffset(currentOffset);
}

void Figure::setPosition(Vec2 position)
{
  this->position = position;  // Base position for the figure
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
  //glColor3b(color.red, color.green, color.blue);
  for (int i = 0; i < num_vertex; i++) {
    // Apply the offset when drawing each vertex
    glVertex2f(vertices[i].x + offset.x, vertices[i].y + offset.y);
  }
  glEnd();
}


