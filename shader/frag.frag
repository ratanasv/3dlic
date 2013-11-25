#version 430 compatibility

uniform sampler3D volume;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;
in vec3 fDir;

uniform mat4 uModelViewMatrix;
uniform float uNumSteps;
uniform float uBaseAlpha;
uniform float uValMin;
uniform float uValMax;

const float SQRT3 = 1.732;

vec3 Rainbow( float t ) {
	t = clamp( t, 0., 1. );

	vec3 rgb;

	// b -> c
	rgb.r = 0.;
	rgb.g = 4. * ( t - (0./4.) );
	rgb.b = 1.;

	// c -> g
	if( t >= (1./4.) )
	{
		rgb.r = 0.;
		rgb.g = 1.;
		rgb.b = 1. - 4. * ( t - (1./4.) );
	}

	// g -> y
	if( t >= (2./4.) )
	{
		rgb.r = 4. * ( t - (2./4.) );
		rgb.g = 1.;
		rgb.b = 0.;
	}

	// y -> r
	if( t >= (3./4.) )
	{
		rgb.r = 1.;
		rgb.g = 1. - 4. * ( t - (3./4.) );
		rgb.b = 0.;
	}

	return rgb;
}

vec3 GetForwardDir() {
	return fDir;
}

vec3 GetForwardDirAlternate() {
	vec4 cameraPos = inverse(uModelViewMatrix)[3];
	vec3 adjustedTexCoord = (fTexCoord-0.5)*2.0;
	vec3 dir = adjustedTexCoord - cameraPos.xyz;
	dir = normalize(dir);
	dir = SQRT3*dir/uNumSteps;
	return dir;
}


void main(void) {
	vec3 stp = fTexCoord;
	vec3 forwardDir = GetForwardDirAlternate();



	float astar = 1.;
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
		alpha = texVal * uBaseAlpha; //opacity according to the texel's magnitude.
		vec3 rgb = Rainbow((texVal - uValMin)/(uValMax - uValMin));
		cstar += astar * alpha * rgb;
		astar *= ( 1. - alpha );
		stp += forwardDir; // doing front-to-back composition
		if ( astar == 0.0 ) {
			break;
		}
	}

    gl_FragColor = vec4(cstar, 1.0);

}
