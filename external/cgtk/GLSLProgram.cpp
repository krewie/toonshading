//! @file    GLSLProgram.cpp
//! @author  Johan Nysjo, Fredrik Nysjo
//! @date    <2014-04-30 Wed>
//!
//! @brief Source file with definitions for GLSLProgram.h
//!

#include "GLSLProgram.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Unnamed namespace (for helper functions and constants)
//
namespace {  
void showShaderInfoLog(GLuint shader)
{
    if (!glIsShader(shader)) {
        std::cerr << "Input argument is not a shader." << std::endl;
        return;
    }
  
    GLint infoLogLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1) {
        char* infoLog = (char*)malloc(infoLogLength * sizeof(char));
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
        std::cerr << "Error compiling shader:\n" << infoLog << std::endl;
        free(infoLog);
    }
}

GLuint createShader(GLenum type, const char* shader_source)
{
    // Create shader object
    GLuint shader = glCreateShader(type);
    if (!glIsShader(shader)) {
        return 0;
    }
  
    // Load the shader source
    glShaderSource(shader, 1, &shader_source, NULL);
  
    // Compile the shader
    glCompileShader(shader);

    // Check compile status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        showShaderInfoLog(shader);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void showProgramInfoLog(GLuint program)
{
    if (!glIsProgram(program)) {
        std::cerr << "Input argument is not a program." << std::endl;
        return;
    }
  
    GLint infoLogLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1) {
        char* infoLog = (char*)malloc(infoLogLength * sizeof(char));
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        std::cerr << "Error linking program:\n" << infoLog << std::endl;
        free(infoLog);
    }
}

bool linkProgram(GLuint program)
{
    glLinkProgram(program);
  
    // Check linking status
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        showProgramInfoLog(program);
        glDeleteProgram(program);
        return false;
    }
  
    return true;
}

bool validateProgram(GLuint program)
{
    glValidateProgram(program);
  
    // Check validation status
    GLint validated;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);
    if (!validated) {
        std::cerr << "Invalid program." << std::endl;
        glDeleteProgram(program);
        return false;
    }
  
    return true;
}

void showUniformLocationErrorMessage(const char *name)
{
    std::cerr << "Could not find the location of " << name << std::endl;
}

void showAttributeLocationErrorMessage(const char *name)
{
    std::cerr << "Could not find the location of " << name << std::endl;
}
}

namespace cgtk {

GLSLProgram::GLSLProgram() :
    mShaderSources(),
    mAttributeLocations(),
    mValid(false),
    mProgram(0)
{
    std::cout << "Called GLSLProgram constructor" << std::endl;
}

GLSLProgram::~GLSLProgram()
{
    if (mProgram) {
        glDeleteProgram(mProgram);
    }
}

void GLSLProgram::setShaderSource(const GLenum type, const std::string &source)
{
    mShaderSources[type] = source;
    mValid = false;
}

void GLSLProgram::setAttributeLocation(const std::string &name, const int32_t location)
{
    mAttributeLocations[name] = location;
    mValid = false;
}

int32_t GLSLProgram::getAttributeLocation(const std::string &name)
{
    int32_t location = -1;
    auto it = mAttributeLocations.find(name);
    if (it != mAttributeLocations.end()) {
        location = it->second;
    }
    else {
        location = glGetAttribLocation(mProgram, name.c_str());
        mAttributeLocations[name] = location;
    }
    return location;
}

bool GLSLProgram::update()
{
    // Create program object
    if (mProgram) {
        glDeleteProgram(mProgram);
    }
    mProgram = glCreateProgram();
    if (!mProgram) {
        return false;
    }

    // Create and attach shaders to the program
    for (auto it = mShaderSources.begin(); it != mShaderSources.end(); ++it) {
        GLenum type = it->first;
        const std::string &source = it->second;
        uint32_t shader = createShader(type, source.c_str());
        if (!(shader > 0)) {
            return false;
        }
        glAttachShader(mProgram, shader);
        glDeleteShader(shader); // So that we don't have to call glDetachShader later
    }
  
    // Bind attribute locations
    for (auto it = mAttributeLocations.begin(); it != mAttributeLocations.end(); ++it) {
        const std::string &name = it->first;
        GLint location = it->second;
        glBindAttribLocation(mProgram, location, name.c_str());
    }

    // Link the program
    if (!linkProgram(mProgram)) {
        return false;
    }

    mValid = true;

    return true;
}

bool GLSLProgram::setUniform1i(const char *name, int value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform1i(location, value);
    }
    return true;
}

bool GLSLProgram::setUniform2i(const char *name, glm::ivec2 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform2iv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniform3i(const char *name, glm::ivec3 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform3iv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniform4i(const char *name, glm::ivec4 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform4iv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniform1f(const char *name, float value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform1f(location, value);
    }
    return true;
}

bool GLSLProgram::setUniform2f(const char *name, glm::vec2 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniform3f(const char *name, glm::vec3 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniform4f(const char *name, glm::vec4 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniformMatrix2f(const char *name, glm::mat2 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniformMatrix3f(const char *name, glm::mat3 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    return true;
}

bool GLSLProgram::setUniformMatrix4f(const char *name, glm::mat4 value)
{
    GLint location = glGetUniformLocation(mProgram, name);
    if (location < 0) {
        return false;
    }
    else {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    return true;
}

void GLSLProgram::enable()
{
    glUseProgram(mProgram);
}

void GLSLProgram::disable()
{
    glUseProgram(0);
}

bool GLSLProgram::isValid() const
{
    return mValid;
}

uint32_t GLSLProgram::getProgramHandle() const
{
    return mProgram;
}

std::string readGLSLSource(const std::string &filename)
{
    std::ifstream file(filename);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

} // namespace cgtk
