#include "shader.hpp"


std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if(in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
}

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    // read files and store in strings
    std::string vertex_code = get_file_contents(vertex_path);
    std::string fragment_code = get_file_contents(fragment_path);

    const char* vertex_source = vertex_code.c_str();
    const char* fragment_source = fragment_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    compile_errors(vertex_shader, "VERTEX");

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    compile_errors(fragment_shader, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    compile_errors(ID, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::activate_shader() {
    glUseProgram(ID);
}

void Shader::delete_shader() {
    glDeleteProgram(ID);
}

void Shader::compile_errors(unsigned int shader, const char* type) {
    GLint compiled;
    char log[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if(!compiled) {
            glGetShaderInfoLog(shader, 1024, NULL, log);
            std::cout << "SHADER COMPILATION ERROR for: " << type << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &compiled);
        if(!compiled) {
            glGetProgramInfoLog(shader, 1024, NULL, log);
            std::cout << "SHADER_LINKING_ERROR for: " << type << std::endl;
        }
    }
}
