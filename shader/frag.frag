#version 430 compatibility

uniform sampler3D uSparseNoiseSampler;
uniform sampler3D uVectorData;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;
in vec3 fDir;

uniform mat4 uModelViewMatrix;
uniform float uNumSteps;
uniform float uBaseAlpha;
uniform float uRainbowValMin;
uniform float uRainbowValMax;
uniform float uMinMagnitude;
uniform float uMaxMagnitude;
uniform float uColorIntensity;

uniform float uNumStepsLIC;
uniform float uVelocityScale;
uniform float uDT;

const float SQRT3 = 1.732;

vec3 GetScaledVelocityData(vec3 stp) {
	vec3 returned;
	returned.x = -3.0 + 6.0*stp.s - 4.0*stp.s*(stp.t+1.0) - 4.0*stp.p;
	returned.y = 12.0*stp.s - 4.0*stp.s*stp.s - 12.0*stp.p + 4.0*stp.p*stp.p;
	returned.z = 3.0 + 4.0*stp.s - 4.0*stp.s*(stp.t+1.0) + 6.0*stp.p + 4.0*(stp.t+1.0)*stp.p;
	return normalize(returned);
}

vec3 GetVelocityTextureData(vec3 stp) {
	// data in texture is already normalized.
	return texture(uVectorData, stp).rgb;
}

vec3 Rainbow( float t ) {
	//t = clamp( t, 0., 1. );
	t = 1.0 - t;

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

float transferFunction(vec3 stp, float accumulated) {
	float magnitude = texture(uVectorData, stp).a;
	if (magnitude < uMinMagnitude || magnitude > uMaxMagnitude) {
		return 0.0;
	}
	return magnitude;
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

vec3 SolveAdvectionEqn(vec3 pos, vec3 vel) {
	return pos + vel*uDT;
}

float FetchSparseNoise(vec3 stp) {
	return texture(uSparseNoiseSampler, stp).r;
}

float ComputeLIC(vec3 stp) {
	float accumulated = FetchSparseNoise(stp);
	vec3 stpIterator = stp;
	for (int i=0; i<uNumStepsLIC; i++) {
		vec3 velocity = GetVelocityTextureData(stpIterator);
		stpIterator = SolveAdvectionEqn(stpIterator, velocity);
		accumulated += FetchSparseNoise(stpIterator);
	}
	for (int i=0; i<uNumStepsLIC; i++) {
		vec3 velocity = GetVelocityTextureData(stpIterator);
		stpIterator = SolveAdvectionEqn(stpIterator, -1.0*velocity);
		accumulated += FetchSparseNoise(stpIterator);
	}
	return accumulated/(2.0*uNumStepsLIC);
}


/**
 * Clamps, and also converts it to rainbow color.
 */
vec3 ClampRainbow(float t, float smin, float smax) {
	return Rainbow(1.0 - (t - smin)/(smax - smin));
}


void main(void) {
	vec3 stp = fTexCoord;
	vec3 forwardDir = GetForwardDir();


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
		float accumulated = ComputeLIC(stp);
		alpha = transferFunction(stp, accumulated) * uBaseAlpha;
		vec3 rgb = uColorIntensity*accumulated*
			ClampRainbow(texture(uVectorData, stp).a, uRainbowValMin, uRainbowValMax);
		cstar += astar * alpha * rgb;
		astar *= ( 1. - alpha );
		stp += forwardDir; // doing front-to-back composition
		if ( astar < 0.05 ) {
			break;
		}
	}
    gl_FragColor = vec4(cstar, 1.0);

}
