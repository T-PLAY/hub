#include "Scan.glsl"
//#include "DefaultLight.glsl"
#include "VertexAttribInterface.frag.glsl"

out vec4 out_color;

//struct Material {
//    int perVertexColor;
//    vec4 color;
//    ScanTextures tex;
//};


void main() {
    vec4 bc = getBaseColor( material, getPerVertexTexCoord() );
    if ( toDiscard( material, bc ) ) discard;

    //out_color = vec4( bc.rgb, 1 );
//    out_color = vec4( bc.rrr, 1 );
//    return;

//    float color = (bc.b + bc.g * 255.0) / 255.0;
//    float color = bc.b;
    float color = clamp(bc.g, 0.0, 1.0);
    float alpha = color;
//    float alpha = 1.0 - color;

//    color = 0.9;
//    out_color = vec4(vec3(color), alpha);
    out_color = vec4(vec3(color), 1.0);
//    out_color = vec4(0.0, 1.0, 0.0, 0.5);
    return;

//    float alpha = 1.0 - color;
//    float alpha = color;
//    float threshold = material.color.r;
//    if (alpha < threshold) discard;
////    out_color = vec4(vec3(color * material.color.g), material.color.g);
////    out_color = vec4(vec3(color * material.color.g), 0.0);
//    out_color = vec4(vec3(color), 1.0);
//    out_color = vec4(vec3(color * 0.8), );
}
