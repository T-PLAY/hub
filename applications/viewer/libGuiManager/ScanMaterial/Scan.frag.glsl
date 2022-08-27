#include "Scan.glsl"
#include "DefaultLight.glsl"
#include "VertexAttribInterface.frag.glsl"

out vec4 out_color;

void main() {
    vec4 bc = getBaseColor( material, getPerVertexTexCoord() );
    if ( toDiscard( material, bc ) ) discard;
    //out_color = vec4( bc.rgb, 1 );
    out_color = vec4( bc.rrr, 1 );

//    float color = (bc.b + bc.g * 255.0) / 255.0;
    float color = bc.g;
//    color = 0.9;
//    float alpha = 1.0 - color;
    float alpha = color;
    if (alpha < 0.1) discard;
    out_color = vec4(vec3(color), 1.0);
}
