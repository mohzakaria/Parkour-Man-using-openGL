#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            //  Generate and bind the Vertex Array Object (VAO)

    
            glGenVertexArrays(1, &VAO); // generate single vertex array object with name (identifier) VAO
            glGenBuffers(1, &VBO);      // generate single vertex buffer with name VBO to store vertex data: pos/normals/colors
            glGenBuffers(1, &EBO);      // generate element array buffer that holds indices that reference vertices in vertex buffer

            glBindVertexArray(VAO); // bind the generated array object to be the active container fo any susequent
            // configurations making it the current vao which means that all subsequent vertex-related configurations
            // and operations will be applied to this specific VAO until it's unbound or another VAO is bound

            glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                // binds it to opengl target GL_ARRAY_BUFFER that typically stores positions, colors, texture coordinates
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); // allocate memory for data : gl_static means that data will be used for drawing and not frequently changed

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // binds buffer pbject to target will be used as the element array buffer for subsequent operations
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

            elementCount = elements.size(); 

            glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // enables the vertex attribute array specified by ATTRIB_LOC_POSITION
            // which is constant that represents the index of the vertex attr in the shader program.
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
            // specifies the structure of the vertex attribute arrays
            //  3--> three components per vertex attribute x y z
            // GL_FLOAT --> datatype is float
            // false--> data WILL NOT be normalizsed
            // sizef0f(vertex)--> the  byte offset between consecutive veretx sttribute in the array(the sixe of each vertex)
            //(void*)0--> the byte offset within vertex buffer object to locate the first component of the attribute
            //  is set to the begining of the vertex struct

            glEnableVertexAttribArray(ATTRIB_LOC_COLOR); // enables the vertex attr array specified by ATTRIB_LOC_COLOR
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
            // specifies the structure of the vertex attribute arrays
            //  4--> four components per vertex attribute R G B A
            // GL_UNSIGNED_BYTE--> datatype is unsigned integer
            // true--> data WILL be normalizsed
            // sizef0f(vertex)--> the  byte offset between consecutive veretx sttribute in the array(the sixe of each vertex)
            //(void *)offsetof(Vertex, color)--> the byte offset within vertex buffer object to locate the first component of the attribute
            //  is set to the begining of the second attr : color  

            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);// enables the vertex attr array specified by ATTRIB_LOC_TEXCOORD
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
            // specifies the structure of the vertex attribute arrays
            //  2--> two components per vertex attribute x y 
            // GL_FLOAT --> datatype is float
            // false--> data WILL NOT be normalizsed
            // sizef0f(vertex)--> the  byte offset between consecutive veretx sttribute in the array(the sixe of each vertex)
            //(void *)offsetof(Vertex, tex_coord)--> the byte offset within vertex buffer object to locate the first component of the attribute
            //  is set to the begining of the third attr : texcood

            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);// enables the vertex attr array specified by ATTRIB_LOC_NORMAL
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
            // specifies the structure of the vertex attribute arrays
            //  3--> three components per vertex attribute x y Z
            // GL_FLOAT --> datatype is float
            // false--> data WILL NOT be normalizsed
            // sizef0f(vertex)--> the  byte offset between consecutive veretx sttribute in the array(the sixe of each vertex)
            //(void *)offsetof(Vertex, normal)--> the byte offset within vertex buffer object to locate the first component of the attribute
            //  is set to the begining of the fourth attr : normal
            glBindVertexArray(0); 
        }

        // this function should render the mesh
        void draw()
        {
            // TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);//(void *)0 since EBO is already bound
            glBindVertexArray(0); // Unbind VAO after drawing
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}