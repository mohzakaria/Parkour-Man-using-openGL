#include <shader.hpp>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //TODO: Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
// an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

    //We return true if the compilation succeeded

     // Create a shader object
     //type here is the file type Like:GL_VERTEX_SHADER ,GL_FRAGMENT_SHADER
    GLuint shaderID = glCreateShader(type);

    // Attach the shader source code to the shader object
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);//nullptr: This parameter represents an array of string lengths.
    // Since modern OpenGL expects null-terminated strings for shader source code, the lengths are not explicitly provided (hence, nullptr).
    //it provides length of the string
    glCompileShader(shaderID);
    if (checkForShaderCompilationErrors(shaderID) != "") //check if there's error on compilation
    {
        std::cout << checkForShaderCompilationErrors(shaderID); // if there's error , print the message
        return false;
    }
    
    glAttachShader(program, shaderID); // attach sahder 
    return true;
}


//
bool our::ShaderProgram::link() const {
    
    glLinkProgram(program); // to link the program with vertex shader or geomtry shader or fragment shader, they will be used to create an executable
    if (checkForLinkingErrors(program) != "")
{
        std::cout << checkForLinkingErrors(program);
        return false;
    }
    return true;
}
////////////////////////
////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}