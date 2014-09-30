#pragma once

using Angel::vec4;

class Normalizer {
public:
	Normalizer(float center, float radius) : _center(center), _radius(radius) {};
	vec4<> operator() (const vec3<>& vec) {
		float vecLength = length(vec);
		return vec4<>(vec/vecLength*_radius + _center, vecLength);
	}
private:
	const float _center;
	const float _radius;
};