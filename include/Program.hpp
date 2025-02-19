#ifndef __PROGRAM__
#define __PROGRAM__

#include <GL/glew.h>
#include "Shader.hpp"
#include <string>
#include "unordered_map"
#include "glm/glm.hpp"

class Program {
public:
	/** Miembros de clase especiales */
	Program();
	~Program();

	/** Métodos de clase */
	void attach(Shader* shader);
	bool link();
	GLuint get_id();
	void setUniform(const std::string& name, int value);
	void use() const;
	void unuse();
	static bool createProgram(Program& program, Shader* vertex_shader, Shader* fragment_shader);
	void setVec3(const std::string &name, const glm::vec3 &value);
	void setInt(const std::string& name, const int value);
	void setFloat(const std::string& name, const float value);
private:
	/** Atributos */
	std::unordered_map<std::string, int> uniform_map;
	int MapUniformLocation(const std::string &name);
	int GetUniformLocation(const std::string &location);
	GLuint id;
};
#endif // !__PROGRAM__
