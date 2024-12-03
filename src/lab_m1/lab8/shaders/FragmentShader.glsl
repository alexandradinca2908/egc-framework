#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int is_spot;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 world_normal = normalize(world_normal);
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25 * material_kd;
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);

    int receivesLight;
    if (dot(world_normal, L) > 0) {
        receivesLight = 1;
    } else {
        receivesLight = 0;
    }
    float specular_light = material_ks * receivesLight * pow(max(dot(world_normal, H), 0), material_shininess);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if (is_spot == 1) {
        float cut_off = radians(30.0f);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);

        if (spot_light < cos(cut_off)) {
	        diffuse_light = 0;
            specular_light = 0;
        } else {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);

            diffuse_light *= light_att_factor;
            specular_light *= light_att_factor;
        }
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    float distance = length(light_position - world_position);
    float factor = 1 / (distance * distance + 1);
    float light = ambient_light + factor * (diffuse_light + specular_light);

    // TODO(student): Write pixel out color
    out_color = vec4(vec3(light) * object_color, 1.0);

}
