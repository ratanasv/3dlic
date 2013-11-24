#version 430 compatibility

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec3 vTexCoord;
attribute vec3 vColor;

out vec3 fTexCoord;
out vec3 fColor;
out vec3 fNormal;
out vec3 fDir;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform float uNumSteps;

const float SQRT3 = 1.732;

void main(void) {
	vec4 modelSpaceCoord = vec4(vPosition, 1.0);

	fTexCoord = vTexCoord;
	fColor = vColor;
	fNormal = vNormal;

	vec4 worldSpaceCoord = uModelViewMatrix * modelSpaceCoord;
	worldSpaceCoord.stp = ( worldSpaceCoord.xyz + vec3(1.,1.,1.) ) / 2.;	
	vec3 eye = ( vec3(0.,0.,0.) + vec3( 1., 1., 1. ) ) / 2.;
	worldSpaceCoord.stp = worldSpaceCoord.stp - eye;	
	worldSpaceCoord.w = 0.;
	fDir = SQRT3 * normalize( ( inverse(uModelViewMatrix) * worldSpaceCoord ).stp ) / float(uNumSteps);

    gl_Position = uProjectionMatrix * uModelViewMatrix * modelSpaceCoord;

}


