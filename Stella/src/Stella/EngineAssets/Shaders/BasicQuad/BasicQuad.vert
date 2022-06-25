#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_UV;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_ModelMatrix;

out vec2 v_UV;

void main()
{
    //gl_Position = vec4(a_Position.xyz, 1.0); 
    gl_Position = u_Projection * u_View * u_ModelMatrix * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
    v_UV = a_UV;
}
