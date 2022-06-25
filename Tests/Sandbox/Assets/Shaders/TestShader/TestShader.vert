#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_UV;
layout (location = 2) in vec3 a_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_ModelMatrix;

out vec2 v_UV;
out vec3 v_Normal;

void main()
{
   gl_Position = u_Projection * u_View * u_ModelMatrix * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
   v_UV = a_UV;
   v_Normal = a_Normal;
}