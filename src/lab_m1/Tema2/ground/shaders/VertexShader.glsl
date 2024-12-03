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
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
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
    frag_position = v_position;
    frag_normal = v_normal;
    frag_textcoord = v_textcoord;

    //  Add noise to position
    vec2 pos = vec2(v_position.x, v_position.z);
    float noise = noise(pos * 0.1) * 3;
    vec3 new_position = vec3(v_position.x, noise, v_position.z);

    //  Add noise to colour
    noise = noise * 0.5 + 0.5;
    frag_color = mix(vec3(0.1, 0.5, 0.2), vec3(0.5, 0.75, 0.4), noise);

    //  Compute gl_Position
    gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
