#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_TexId;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Color;
out vec2 v_UV;
out float v_TexId;

void main()
{
    gl_Position = u_Projection * u_View * vec4(a_Position.xyz, 1.0);
    v_Color = a_Color;
    v_UV = a_UV;
    v_TexId = a_TexId;
}
