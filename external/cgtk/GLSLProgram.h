//! @file    GLSLProgram.h
//! @author  Johan Nysjo, Fredrik Nysjo
//! @date    <2014-04-30 Wed>
//!
//! @brief Header declaring the GLSLProgram class
//!

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <stdint.h>
#include <string>
#include <map>

namespace cgtk {

//! @class GLSLProgram GLSLProgram.h GLSLProgram.h
//!
//! @brief Wrapper class for a GLSL program.
//!
class GLSLProgram {
public:
    GLSLProgram();

    ~GLSLProgram();

    //! Set GLSL shader source string. Invalidates the wrapper state
    //! until update() is called.
    //!
    //! @param[in] type
    //!   Shader type, for instance, GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
    //! @param[in] source
    //!   Shader source string.
    //!
    void setShaderSource(const GLenum type, const std::string &source);

    //! Set attribute location. Invalidates the wrapper state until
    //! update() is called.
    //!
    //! @param[in] name
    //!   Attribute name, for instance, "a_vertex".
    //! @param[in] location
    //!   An index specifying the location of the attribute
    //!   variable. Valid range is 0 to GL_MAX_VERTEX_ATTRIBS-1.
    //!
    void setAttributeLocation(const std::string &name, const int32_t location);

    //! Get attribute location.
    //!
    //! @param[in] name
    //!   Attribute name, for instance, "a_vertex".
    //! @return
    //!   An index specifying the location of the attribute
    //!   variable. Valid range is 0 to GL_MAX_VERTEX_ATTRIBS-1.
    //!
    int32_t getAttributeLocation(const std::string &name);

    //! Update the program. This method needs to be called at least once
    //! in order to create the program.
    //!
    //! @return
    //!   true if the program was compiled and linked successfully,
    //!   false otherwise.
    //!
    bool update();
    
    //! Set the value of an int uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform1i(const char *name, int value);

    //! Set the value of a ivec2 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform2i(const char *name, glm::ivec2 value);

    //! Set the value of a ivec3 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform3i(const char *name, glm::ivec3 value);

    //! Set the value of a ivec4 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform4i(const char *name, glm::ivec4 value);

    //! Set the value of a float uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform1f(const char *name, float value);

    //! Set the value of a vec2 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform2f(const char *name, glm::vec2 value);

    //! Set the value of a vec3 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform3f(const char *name, glm::vec3 value);

    //! Set the value of a vec4 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniform4f(const char *name, glm::vec4 value);

    //! Set the value of a mat2 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniformMatrix2f(const char *name, glm::mat2 value);

    //! Set the value of a mat3 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniformMatrix3f(const char *name, glm::mat3 value);

    //! Set the value of a mat4 uniform.
    //!
    //! @param[in] name
    //!   The name of the uniform.
    //! @param[in] value
    //!   The value that should be assigned to the uniform.
    //! @return
    //!   true if the value was set successfully, false otherwise.
    //!
    bool setUniformMatrix4f(const char *name, glm::mat4 value);
    
    //! Enable the program.
    //!
    void enable();
    
    //! Disable the program.
    //!
    void disable();

    //! Check program state.
    //!
    //! @return
    //!   True if the program is valid and corresponds to the wrapper state,
    //!   false otherwise.
    bool isValid() const;

    //! Provides access to the program handle. This enables the user
    //! to, for instance, use standard OpenGL calls to set uniforms
    //! and attributes.
    //!
    //! @return
    //!   A non-zero (valid) program handle, or zero if the program is
    //!   invalid or has not yet been created, i.e., if update() has
    //!   not been called.
    //!
    uint32_t getProgramHandle() const;
private:
    // Make instances non-copyable.
    GLSLProgram(const GLSLProgram &);
    const GLSLProgram &operator=(const GLSLProgram &);

    std::map<GLenum, std::string> mShaderSources;
    std::map<std::string, int32_t> mAttributeLocations;
    bool mValid;
    uint32_t mProgram;
};

//! Utility function for reading a GLSL source file into a string.
std::string readGLSLSource(const std::string &filename);

} // namespace cgtk
