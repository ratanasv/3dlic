#version 430 compatbility

uniform float xyCuttingPlaneZValue;
attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec3 vTexCoord;
attribute vec3 vColor;

out vec3 fTexCoord;
out vec3 fColor;
out vec3 fNormal;

void main(void) {
	vec4 xyPlane = vec4(vPosition, 1.0);
	xyPlane.z = xyCuttingPlaneZValue;

	fTexCoord = vTexCoord;
	fColor = vColor;
	fNormal = vNormal;
    gl_Position = gl_ModelViewProjectionMatrix * xyPlane;
}


