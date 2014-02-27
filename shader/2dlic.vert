#version 430 compatibility

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec3 vTexCoord;
attribute vec3 vColor;

out vec3 fTexCoord;
out vec3 fColor;
out vec3 fNormal;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

uniform float uZ;

void main(void) {
	vec4 modelSpaceCoord = vec4(vPosition.xy, uZ, 1.0);

	fTexCoord = vec3(vTexCoord.st, 0.5*uZ+0.5);
	fColor = vec4(1.0, 1.0, 1.0, 1.0);
	fNormal = vNormal;

    gl_Position = uProjectionMatrix * uModelViewMatrix * modelSpaceCoord;
}


