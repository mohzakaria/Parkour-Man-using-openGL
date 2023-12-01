#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    frag_color=texture(tex, tex_coord);
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    float green = frag_color.g;
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    float red = texture(tex, tex_coord - vec2(STRENGTH, 0.0)).r; // Sample the red channel from a pixel to the left
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel
    float blue = texture(tex, tex_coord + vec2(STRENGTH, 0.0)).b; // Sample the blue channel from a pixel to the right
    float alpha=frag_color.a; 
    frag_color = texture(red,green,blue,alpha);
}
