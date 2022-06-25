#version 330 core
out vec4 FragColor;

in vec2 v_UV;
in vec3 v_Normal;

void main()
{
   //FragColor = vec4(v_UV.xy, 0.0f, 1.0f);
   FragColor = vec4(v_Normal.xyz, 1.0f);
}