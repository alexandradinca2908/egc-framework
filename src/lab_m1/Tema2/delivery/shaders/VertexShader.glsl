#version 330

//  Input
//  Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_textcoord;
layout(location = 3) in vec3 v_color;

//  Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

//  Output
//  Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_textcoord;
out vec3 frag_color;

//  2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 129.5);
}

//  2D Noise
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    //  Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    //  Smooth Interpolation

    //  Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f * f * (3.0 - 2.0 * f);

    //  Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a) * u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
    //  Send output to fragment shader
    frag_normal = v_normal;
    frag_color = vec3(1.0, 0.5, 0);

    //  Add noise to position
    vec3 world_position = vec3(Model * vec4(v_position, 1.0)).xyz;

    vec2 pos = vec2(world_position.x, world_position.z);
    float noise = noise(pos * 0.1) * 4 + 0.06;

    world_position.y = noise;

    //  Compute gl_Position
    gl_Position = Projection * View * vec4(world_position, 1.0);
}