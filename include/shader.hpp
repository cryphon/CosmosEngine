#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID;
    
    // Constructor
    Shader(const char* vertex_path, const char* fragment_path);

    void activate_shader();
    void delete_shader();

    void compile_errors(unsigned int shader, const char* type);
};

#endif
