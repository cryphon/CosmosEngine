// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/Shader.hpp>
#include <cosmos/core/Logger.hpp>


namespace cosmos::render {

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

std::string preprocess_shader(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Could not open shader file");

    std::string line;
    std::stringstream result;

    while (std::getline(file, line)) {
        if (line.find("#include") == 0) {
            size_t start = line.find("\"") + 1;
            size_t end = line.rfind("\"");
            std::string include_path = line.substr(start, end - start);
            LOG_DEBUG(include_path);
            std::string full_path = "shaders/" + include_path; // use std::filesystem for clean pathing
            LOG_DEBUG(full_path);
            result << preprocess_shader(full_path); // recursively include
        } else {
            result << line << '\n';
        }
    }
    std::string result_str = result.str();
    LOG_DEBUG("Preprocessed shader [" + path + "]:\n" + result_str);
    return result_str;
}



Shader::Shader(const char* vertex_path, const char* fragment_path) {
    // read files and store in strings
    std::string vertex_code = preprocess_shader(vertex_path);
    std::string fragment_code = preprocess_shader(fragment_path);

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


void Shader::set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::compile_errors(unsigned int shader, const char* type) {
    GLint compiled;
    char log[1024];
    if (type != std::string("PROGRAM")) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            glGetShaderInfoLog(shader, 1024, NULL, log);
            LOG_ERROR(std::string("SHADER COMPILATION ERROR for: ") + type + "\n" + log);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &compiled);  // Fix: use GL_LINK_STATUS for programs
        if (!compiled) {
            glGetProgramInfoLog(shader, 1024, NULL, log);
            LOG_ERROR(std::string("SHADER LINKING ERROR for: ") + type + "\n" + log);
        }
    }
}
}

