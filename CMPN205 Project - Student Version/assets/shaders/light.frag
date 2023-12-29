#version 330 core

#define DIRECTIONAL 0
#define POINT 2
#define SPOT 2

in Varyings {

        vec4 color;

        vec2 tex_coord;

        vec3 world;

        vec3 view;

        vec3 normal;

} fsin;


struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color; // amibent w specular w el diffusion 
    vec3 attenuation;// constant, linear, quadreatic
    vec2 cone_angles; // spot light
};

uniform Material material;

#define MAX_LIGHT_COUNT 16

uniform Light lights[MAX_LIGHT_COUNT];

uniform int light_count;
ut vec4 frag_color;

struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};


void main() {

        vec3 normal = normalize(fsin.normal);
        vec3 view = normalize(fsin.view);
        int count = min(light_count, MAX_LIGHT_COUNT);
        vec3 accumulated_light = vec3(0.0);
        for(int index = 0; index < count; index++){

        Light light = lights[index];

        vec3 light_direction;

        float attenuation = 1;

        if(light.type == TYPE_DIRECTIONAL)

        light_direction = light.direction;

else {

        light_direction = fsin.world - light.position;

        float distance = length(light_direction);

        light_direction /= distance;

        attenuation *= 1.0f / (light.attenuation_constant +

        light.attenuation_linear * distance +

        light.attenuation_quadratic * distance * distance);

        if(light.type == TYPE_SPOT){
        
        float angle = acos(dot(light.direction, light_direction));

        attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);

 }

  }

    vec3 reflected = reflect(light_direction, normal);
    
    float lambert = max(0.0f, dot(normal, -light_direction));
    
    float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);
    
    vec3 diffuse = material.diffuse * light.diffuse * lambert;
    
    vec3 specular = material.specular * light.specular * phong;
    
    vec3 ambient = material.ambient * light.ambient;
    
    accumulated_light += (diffuse + specular) * attenuation + ambient;

   }

    frag_color= fsin.color * vec4(accumulated_light, 1.0f);

}