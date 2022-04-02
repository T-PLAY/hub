
struct Transform {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 mvp;
    mat4 modelView;
    mat4 worldNormal;
    mat4 viewNormal;
};


layout (location = 0) in vec3 in_position;
layout (location = 4) in vec3 in_texcoord;
layout (location = 0) out vec3 out_pos;
layout (location = 2) out vec3 out_texcoord;
uniform Transform transform;
void main(void)
{
	mat4 mvp    = transform.proj * transform.view * transform.model;
	out_pos     = in_position;
	gl_Position = mvp*vec4(in_position.xyz, 1.0);
	out_texcoord = in_texcoord;
}

