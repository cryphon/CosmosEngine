#pragma once
// ==
// Standard Library
// ==
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

// ==
// Third Party
// ==
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// ==
// Cosmos
// ==

namespace cosmos::render {
std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID;
    
    // Constructor
    Shader(const char* vertex_path, const char* fragment_path);

    void activate_shader();
    void delete_shader();

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_mat4(const std::string& name, const glm::mat4& matrix) const;
    void set_vec3(const std::string&name, const glm::vec3& vec) const;
    void set_float(const std::string& name, float value) const;



    void compile_errors(unsigned int shader, const char* type);
};
}
