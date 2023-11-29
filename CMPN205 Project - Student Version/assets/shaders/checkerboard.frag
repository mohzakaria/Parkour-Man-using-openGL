#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.
// Uniform variable specifying the size of each tile
uniform int size = 32;

// Array holding the colors for the two types of tiles (colors[0] for even indices, colors[1] for odd indices)
uniform vec3 colors[2];

void main(){
    // Calculate the tile indices in both x and y directions based on the fragment's position
    //gl_FragCoord.x and gl_FragCoord.y: These are built-in variables in GLSL representing the screen-space coordinates of the fragment being processed.
    uint tileXindex = uint(gl_FragCoord.x / float(size));
    uint tileXindex = uint(gl_FragCoord.x / float(size));
    uint tileYindex = uint(gl_FragCoord.y / float(size));
    bool isSumOfIndicesEven = mod((tileXindex + tileYindex), 2) == 0;
    if (isSumOfIndicesEven)
        frag_color = vec4(colors[0], 1.0);
    else
        frag_color = vec4(colors[1], 1.0);
        
}