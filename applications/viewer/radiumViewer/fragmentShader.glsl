
/*****
 *
 *   Implementation of the interface to acces interpolated attributes from vertex attributes.
 *   This inteface MUST be used ONLY on fragment shaders.
 *
 *****/
//----------------- Supported vertex Attribs ---------------------
layout( location = 0 ) in vec3 in_position;
layout( location = 1 ) in vec3 in_normal;
layout( location = 2 ) in vec3 in_texcoord;
layout( location = 3 ) in vec3 in_vertexcolor;
#ifndef DONT_USE_INPUT_TANGENT
layout( location = 4 ) in vec3 in_tangent;
#endif

//------------------- VertexAttrib interface ---------------------
vec4 getWorldSpacePosition() {
    return vec4( in_position, 1.0 );
}

vec3 getWorldSpaceNormal() {
    if ( length( in_normal.xyz ) < 0.0001 ) { // Spec GLSL : vector not set -> (0, 0, 0, 1)
        vec3 X = dFdx( in_position );
        vec3 Y = dFdy( in_position );
        return normalize( cross( X, Y ) );
    }
    else {
        return normalize( in_normal );
    }
}

#ifndef DONT_USE_INPUT_TANGENT
vec3 getWorldSpaceTangent() {
    if ( length( in_tangent.xyz ) < 0.0001 ) { // Spec GLSL : vector not set -> (0, 0, 0, 1)
        if ( length( in_texcoord.xy ) < 0.0001 ) { return normalize( dFdx( in_position ) ); }
        else {
            vec3 uv_dx = dFdx( in_texcoord );
            vec3 uv_dy = dFdy( in_texcoord );
            vec3 t     = ( uv_dy.t * dFdx( getWorldSpacePosition().xyz ) -
                       uv_dx.t * dFdy( getWorldSpacePosition().xyz ) ) /
                     ( uv_dx.s * uv_dy.t - uv_dy.s * uv_dx.t );
            vec3 ng = getWorldSpaceNormal();
            return normalize( t - ng * dot( ng, t ) );
        }
    }
    else {
        return normalize( in_tangent );
    }
}
#else
vec3 getWorldSpaceTangent() {
    if ( length( in_texcoord.xy ) < 0.0001 ) { return normalize( dFdx( in_position ) ); }
    else {
        vec3 uv_dx = dFdx( in_texcoord );
        vec3 uv_dy = dFdy( in_texcoord );
        vec3 t     = ( uv_dy.t * dFdx( getWorldSpacePosition().xyz ) -
                   uv_dx.t * dFdy( getWorldSpacePosition().xyz ) ) /
                 ( uv_dx.s * uv_dy.t - uv_dy.s * uv_dx.t );
        vec3 ng = getWorldSpaceNormal();
        return normalize( t - ng * dot( ng, t ) );
    }
}
#endif

vec3 getWorldSpaceBiTangent() {
    return normalize( cross( in_normal, getWorldSpaceTangent() ) );
}

vec3 getPerVertexTexCoord() {
    return in_texcoord;
}

vec4 getPerVertexBaseColor() {
    return vec4( in_vertexcolor, 1.0 );
}

struct BlinnPhongTextures {
    int hasKd;
    int hasKs;
    int hasNs;
    int hasNormal;
    int hasAlpha;

    sampler2D kd;
    sampler2D ks;
    sampler2D ns;
    sampler2D normal;
    sampler2D alpha;
};

struct Material {
    vec4 kd;
    vec4 ks;

    float ns;
    float alpha;

    int hasPerVertexKd;
    int renderAsSplat;

    BlinnPhongTextures tex;
};

uniform Material material;


layout (location = 0) out vec4 out_color;
uniform vec4 aColorUniform;
uniform float aScalarUniform;

void main(void)
{
        float gray = texture(material.tex.kd, in_texcoord.xy).r;
        out_color =  vec4(vec3(gray), 1.0);
//        out_color = vec4(texture(material.tex.kd, in_texcoord.xy).r, 0.0, 0.0, 1.0);

//        out_color =  vec4(in_texcoord.xy, 1.0, 1.0);
	// out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;
}

