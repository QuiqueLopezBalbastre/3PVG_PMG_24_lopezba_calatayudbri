#include "Shader.hpp"
#include <iostream>
#include "fstream"
#include "sstream"

Shader::Shader()
{
	type = kShaderType_Invalid;
	isCompiled = false;
	id = -1;
}

Shader::~Shader()
{
}

bool Shader::loadSource(const ShaderType shader_type, const char* source, const unsigned int source_size)
{
	glewInit();
	type = shader_type;
	switch (shader_type) {
	case kShaderType_Vertex:
		id = glCreateShader(GL_VERTEX_SHADER); break;

	case kShaderType_Fragment:
		id = glCreateShader(GL_FRAGMENT_SHADER); break;

	case kShaderType_Geometry:
		id = glCreateShader(GL_GEOMETRY_SHADER); break;
	default: return false; break;
	}
	GLint size = source_size;
	glShaderSource(id, 1, &source, &size);
	return true;
}

bool Shader::loadFromFile(const ShaderType shader_type, const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
		return false;
	}

	// Lee el contenido del archivo
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();

	// Cierra el archivo
	file.close();

	// Carga el shader desde el contenido del archivo
	return loadSource(shader_type, source.c_str(), source.size());
}

bool Shader::compile()
{
	GLint Length;
	std::string output_log;
	glCompileShader(id);
	GLint params = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &params);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &Length);
	output_log.reserve(Length);
	glGetShaderInfoLog(id, Length, 0, output_log.data());
	if (Length > 0)
	{
		printf("Error %s", output_log.data());
	}

	if (GL_FALSE == params) {
		isCompiled = false;
		std::cout << "Error al compilar el shader: " << std::endl;
	}
	else
		isCompiled = true;
	return isCompiled;
}

unsigned int Shader::get_id()
{
	return id;
}

bool Shader::get_isCompiled() const
{
	return isCompiled;
}
