#version 330 core

out vec4 color;

in vec4 v_Color;
in vec2 v_UV;
in float v_TexId;

uniform sampler2D u_Textures[8];

void main()
{    
    if (v_TexId > 0.0){    
        int tid = int(v_TexId);
        color = texture(u_Textures[tid], v_UV) * v_Color;
    }
    else {
        color = v_Color;
    }
}

/*

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[16];

void main()
{
    color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}
*/