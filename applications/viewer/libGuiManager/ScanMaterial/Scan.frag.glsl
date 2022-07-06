#include "DefaultLight.glsl"
#include "Scan.glsl"
#include "VertexAttribInterface.frag.glsl"

out vec4 out_color;

void main() {
    vec4 bc = getBaseColor( material, getPerVertexTexCoord() );
    if ( toDiscard( material, bc ) ) discard;
    //out_color = vec4( bc.rgb, 1 );
    out_color = vec4( bc.rrr, 1 );
}
