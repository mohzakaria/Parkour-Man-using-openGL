#ifndef SHADER_HPP
#define SHADER_HPP
//Abdullah's code
#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //this function will return -1 if it didnt find the locaation
            return glGetUniformLocation(program, name.c_str());// Uniform variables in shaders are used to pass data from the application (CPU) to the shader programs (GPU)
        }
       //////////////////////////////////////////////
       //now we will use getUniformLocation() function

       // and these functions will just 
        void set(const std::string &uniform, GLfloat value) {
             glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
        }

        void set(const std::string &uniform, glm::vec4 value) {
             glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
          this->use();
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, &matrix[0][0]);
        }
        ShaderProgram(const ShaderProgram&) = delete; // to delete copy constrcutor and assignment operator
        ShaderProgram& operator=(ShaderProgram&) = delete; //

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        //Question: Why do we delete the copy constructor and assignment operator?
    };

}

#endif