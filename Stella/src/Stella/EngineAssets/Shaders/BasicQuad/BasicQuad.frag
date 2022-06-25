#version 330 core

uniform sampler2D s_Texture;

in vec2 v_UV;

out vec4 FragColor;

void main()
{    
    vec3 col = texture(s_Texture, v_UV).xyz;
    //FragColor = vec4(0.5, 0.5, 0.0, 1.0);
    FragColor = vec4(col.xyz, 1.0);
}