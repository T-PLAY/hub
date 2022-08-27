#include "Scan.glsl"
#include "DefaultLight.glsl"
#include "TransformStructs.glsl"

layout( location = 0 ) out vec4 f_Accumulation;
layout( location = 1 ) out vec4 f_Revealage;

#include "VertexAttribInterface.frag.glsl"

//out vec4 out_color;

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
    vec4 bc = getBaseColor( material, getPerVertexTexCoord() );
    if ( toDiscard( material, bc ) ) discard;
    //out_color = vec4( bc.rgb, 1 );
//    out_color = vec4( bc.rrr, 1 );

    float color = (bc.b + bc.g * 255.0) / 255.0;
    float alpha = color - 1.0;
//    out_color = vec4(vec3(color), 0.1);

    float a = 0.5;
    discard;
    if (! toDiscard(material, bc) || a < 0.001 ) discard;

    vec3 contribution = lightContributionFrom( light, getWorldSpacePosition().xyz );

    float w        = weight( gl_FragCoord.z, a );
//    f_Accumulation = vec4( bsdf * contribution * a, a ) * w;
    //f_Accumulation = vec4( contribution * a, a ) * w;
    f_Accumulation = vec4( a ) * w;
    f_Revealage    = vec4( a );
}

