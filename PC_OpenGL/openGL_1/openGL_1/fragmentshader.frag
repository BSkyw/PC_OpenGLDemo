#version  330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
//    vec4 temp = texture(ourTexture2, TexCoord);
//    float factor = temp.a * 1.0;
//    vec4 org = texture(ourTexture, TexCoord);
//    temp.r = texture( ourTexture2, vec2(temp.r, org.r)).r;
//    temp.g = texture( ourTexture2, vec2(temp.g, org.g)).g;
//    temp.b = texture( ourTexture2, vec2(temp.b, org.b)).b;
////    color = vec4(.0,1.,0.1,1.);
    color = texture(ourTexture, vec2(1.0 - TexCoord.x, TexCoord.y));

//    color = vec4(temp.rgb, factor);
}
