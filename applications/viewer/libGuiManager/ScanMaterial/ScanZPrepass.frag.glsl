
#include "Scan.glsl"
#include "TransformStructs.glsl"

layout( location = 0 ) out vec4 out_ambient;
layout( location = 1 ) out vec4 out_normal;
layout( location = 2 ) out vec4 out_diffuse;
layout( location = 3 ) out vec4 out_specular;

#include "VertexAttribInterface.frag.glsl"

out vec4 out_color;

//------------------- main ---------------------
void main() {
//    discard;
//    return;

    // Discard non fully opaque fragments
    vec4 bc = getBaseColor( material, getPerVertexTexCoord() );
    if ( toDiscard( material, bc ) ) discard;

    float color = clamp(bc.g, 0.0, 1.0);
//    if (color < 0.5) discard;


    vec3 binormal    = getWorldSpaceBiTangent();
    vec3 normalWorld = getNormal(
        material, getPerVertexTexCoord(), getWorldSpaceNormal(), getWorldSpaceTangent(), binormal );

    // TODO : use BSDF separable interface ?
//    out_ambient = vec4( vec3(color) * 0.01 + getEmissiveColor( material, getPerVertexTexCoord() ), 1.0 );
    out_ambient = vec4( vec3(color) + getEmissiveColor( material, getPerVertexTexCoord() ), 1.0 );
    out_normal  = vec4( normalWorld * 0.5 + 0.5, 1.0 );
//    out_diffuse = vec4( bc.xyz, 1.0 );
    out_diffuse = vec4( vec3(color), 1.0 );
    out_specular = vec4( getSpecularColor( material, getPerVertexTexCoord() ), 0.5 );

    discard;
}
