#version 430 compatibility

uniform sampler3D uNoiseSampler;
uniform sampler3D uVectorData;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;

uniform mat4 uModelViewMatrix;
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


vec3 SolveAdvectionEqn(vec3 pos, vec3 vel) {
	return pos + vel*uDT;
}

float FetchSparseNoise(vec3 stp) {
	return texture(uNoiseSampler, stp).r;
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

	float vectorMagnitude = texture(uVectorData, stp).a;
	vec3 rgb;
	if (vectorMagnitude < uMinMagnitude || vectorMagnitude > uMaxMagnitude) {
		rgb = vec3(0.0, 0.0, 0.0);
	} else {
		float accumulated = ComputeLIC(stp);
		rgb = uColorIntensity*accumulated*
			ClampRainbow(vectorMagnitude, uRainbowValMin, uRainbowValMax);
	}



	gl_FragColor = vec4(rgb, 1.0);

}
