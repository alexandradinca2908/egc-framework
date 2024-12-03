#version 330

// Input
in vec3 frag_color;
in vec3 frag_normal;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;

void main()
{
    //  Write pixel out color
    out_color = vec4(frag_color, 1.0);
}
