#version 330 core

uniform sampler2D screen_texture;

in vec2 v_UV;

out vec4 FragColor;

void main()
{    
    vec3 col = texture(screen_texture, v_UV).xyz;
    FragColor = vec4(col.xyz, 1.0);
}