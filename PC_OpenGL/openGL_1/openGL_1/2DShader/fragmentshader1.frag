#version  330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = vec4(1.0,0.1,0.1,1.);
//    color = texture(ourTexture, vec2(1.0 - TexCoord.x, TexCoord.y));
//    color = texture(ourTexture, TexCoord);
}
