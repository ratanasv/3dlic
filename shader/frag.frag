#version 430 compatibility

uniform sampler3D volume;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;

uniform mat4 uModelViewMatrix;
uniform float uNumSteps;
uniform float uBaseAlpha;

const float SQRT3 = 1.732;

void main(void) {
	vec3 dir = normalize((inverse(uModelViewMatrix)[3]).xyz);
	dir = dir*(SQRT3/uNumSteps);

	vec3 stp = fTexCoord;
	float astar = 1.0;
	vec3 cstar = vec3(0.0, 0.0, 0.0);
	for (int i=0; i<uNumSteps; i++) {
		float alpha = uBaseAlpha;
		if (any(lessThan(stp, vec3(0.0,0.0,0.0)))) {
			alpha = 0.0;
			continue;
		}
		if (any(greaterThan(stp, vec3(1.,1.,1.)))) {
			alpha = 0.;
			continue;
		}
		float texVal = texture(volume, stp).r;
		cstar += astar * alpha * vec3(texVal, texVal, texVal);
		astar *= (1.0 - alpha);
		stp += dir;
	}

    gl_FragColor = vec4(cstar, 1.0);
}
