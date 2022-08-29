
#include "Scan.glsl"
#include "DefaultLight.glsl"
#include "TransformStructs.glsl"
#include "Palettes.glsl"


layout( location = 0 ) out vec4 f_Accumulation;
layout( location = 1 ) out vec4 f_Revealage;

#include "VertexAttribInterface.frag.glsl"

layout( location = 5 ) in vec3 in_viewVector;
layout( location = 6 ) in vec3 in_lightVector;

// implementation of weight functions of the paper
// Weighted Blended Order-Independent Transparency
// 	Morgan McGuire, Louis Bavoil - NVIDIA
// Journal of Computer Graphics Techniques (JCGT), vol. 2, no. 2, 122-141, 2013
// http://jcgt.org/published/0002/02/09/

// remark : manage only non colored transmission. Direct implementation of the above paper without
// the suggested extension :
// ... non-refractive colored transmission can be implemented as a simple extension by processing a
// separate coverage
//  value per color channel

// Note, z range from 0 at the camera to +infinity far away ...

float weight( float z, float alpha ) {

    // pow(alpha, colorResistance) : increase colorResistance if foreground transparent are
    // affecting background transparent color clamp(adjust / f(z), min, max) :
    //     adjust : Range adjustment to avoid saturating at the clamp bounds
    //     clamp bounds : to be tuned to avoid over or underflow of the reveleage texture.
    // f(z) = 1e-5 + pow(z/depthRange, orederingStrength)
    //     defRange : Depth range over which significant ordering discrimination is required. Here,
    //     10 camera space units.
    //         Decrease if high-opacity surfaces seem “too transparent”,
    //         increase if distant transparents are blending together too much.
    //     orderingStrength : Ordering strength. Increase if background is showing through
    //     foreground too much.
    // 1e-5 + ... : avoid dividing by zero !

    return pow( alpha, 0.5 ) * clamp( 10 / ( 1e-5 + pow( z / 10, 6 ) ), 1e-2, 3 * 1e3 );
}

void main() {
    // only render non opaque fragments and not fully transparent fragments
    vec4 bc = getDiffuseColor( material, getPerVertexTexCoord() );
    // compute the transparency factor
//    float a = bc.a;
    float a = bc.a;
//    float a = 0.5;
    if ( !toDiscard( material, bc ) || a < 0.001 ) discard;

    // all vectors are in world space
    vec3 binormal    = getWorldSpaceBiTangent();
    vec3 normalWorld = getNormal(
        material, getPerVertexTexCoord(), getWorldSpaceNormal(), getWorldSpaceTangent(), binormal );
    vec3 binormalWorld = normalize( cross( normalWorld, getWorldSpaceTangent() ) );
    vec3 tangentWorld  = cross( binormalWorld, normalWorld );

    // A material is always evaluated in the fragment local Frame
    // compute matrix from World to local Frame
    mat3 world2local;
    world2local[0] = vec3( tangentWorld.x, binormalWorld.x, normalWorld.x );
    world2local[1] = vec3( tangentWorld.y, binormalWorld.y, normalWorld.y );
    world2local[2] = vec3( tangentWorld.z, binormalWorld.z, normalWorld.z );
    // transform all vectors in local frame so that N = (0, 0, 1);
    vec3 wi = world2local * normalize( in_lightVector ); // incident direction
    vec3 wo = world2local * normalize( in_viewVector );  // outgoing direction

    vec3 bsdf = evaluateBSDF( material, getPerVertexTexCoord(), wi, wo );

    vec3 contribution = lightContributionFrom( light, getWorldSpacePosition().xyz );

    float w        = weight( gl_FragCoord.z, a );
//    f_Accumulation = vec4( bsdf * contribution * a, a ) * w;
//    f_Accumulation = vec4( contribution * a, a ) * w;
//    f_Revealage    = vec4( a );

//    f_Accumulation = vec4( vec3(a), a ) * w;
//    f_Revealage    = vec4( a );


//    float a2 = a * a;
//    float a3 = a2 * a;
//    float a4 = a3 * a;
//    float a5 = a4 * a;
//    int i = int(a * 256);
//    int i = int(a * 9);
//    a = palette[i];
//    vec3 color = vec3(0.0);

//    a *= material.pimp.y;

    vec3 color;
    if (material.iPalette == 0) {
        color = vec3(a);
    } else if (material.iPalette == 1) {
        color = iron_palette[(int(a * 256))] / 256.0;
    } else if (material.iPalette == 2) {
        color = pet_palette[(int(a * 256))] / 256.0;
    } else if (material.iPalette == 3) {
        color = hot_metal_blue_palette[(int(a * 256))] / 256.0;
    } else if (material.iPalette == 4) {
        color = pet20_palette[(int(a * 256))] / 256.0;
    } else if (material.iPalette == 5) {
        color = spring_palette[(int(a * 6))] / 256.0;
    } else if (material.iPalette == 6) {
        color = summer_palette[(int(a * 9))] / 256.0;
    } else if (material.iPalette == 7) {
        color = fall_palette[(int(a * 7))] / 256.0;
    } else if (material.iPalette == 8) {
        color = winter_palette[(int(a * 10))] / 256.0;
    }

    vec3 colorPow = color;
    float aPow = a;
    for (int i = 0; i <int(material.pimp.x * 20); ++i) {
        colorPow *= color;
        aPow *= a;
    }

    color *= material.pimp.y;
    aPow *= material.pimp.y;

//    a = 0.1 * a;
//    a = a5;
//    color = colorPow;
    f_Accumulation = vec4( color, aPow );
    f_Revealage    = vec4( color, aPow );
}
