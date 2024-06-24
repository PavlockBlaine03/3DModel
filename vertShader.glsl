#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float tf;

layout(binding = 0) uniform sampler2D samp;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{
    float i = gl_InstanceID + tf;
    float rotationSpeed = 0.5f;
    mat4 localRotY = buildRotateY(rotationSpeed * i);

    mat4 newM_matrix = localRotY;
    mat4 v_matrix = mv_matrix * newM_matrix;

    gl_Position = proj_matrix * v_matrix * vec4(position, 1.0);
    tc = texCoord;
}

mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0, 
                      0.0, 0.0, 1.0, 0.0, 
                      x, y, z, 1.0);
    return trans;
}

mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad), cos(rad), 0.0,
                     0.0, 0.0, 0.0, 1.0);
    return xrot;
}

mat4 buildRotateY(float rad)
{
	mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rad), 0.0, cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0);
	return yrot;
}

mat4 buildRotateZ(float rad)
{
	mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return zrot;
}