#version 430 compatibility

uniform float xyCuttingPlaneZValue;
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
	vec4 xyPlane = vec4(vPosition, 1.0);

	fTexCoord = (xyPlane.xyz + 1.0) / 2.0;
	fColor = vColor;
	fNormal = vNormal;

	vec4 xyzw = uModelViewMatrix * xyPlane;
	xyzw.stp = ( xyzw.xyz + vec3(1.,1.,1.) ) / 2.;	
	vec3 eye = ( vec3(0.,0.,0.) + vec3( 1., 1., 1. ) ) / 2.;
	xyzw.stp = xyzw.stp - eye;	
	xyzw.w = 0.;
	fDir = SQRT3 * normalize( ( inverse(uModelViewMatrix) * xyzw ).stp ) / float(uNumSteps);

    gl_Position = uProjectionMatrix * uModelViewMatrix * xyPlane;

}


