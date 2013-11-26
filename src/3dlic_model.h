#pragma once
#include "model_controller.h"
#include "abstract.h"
#include <map>

using std::map;
using vir::Abstract;

class FloatParam {
public:
	FloatParam();
	FloatParam(float a, float b, float c);
	operator float();
	operator const float() const;
	void SetFloat(float in);
private:
	float _val;
	float _minVal;
	float _maxVal;
};


class THREEDLICParameters : public Observable {
private:
	THREEDLICParameters();
	THREEDLICParameters(const THREEDLICParameters& other);
public:
	typedef enum {ORTHO, PERSP} PROJ_TYPE;
	typedef enum {XROT, YROT, XTRANSLATE, YTRANSLATE, ZTRANSLATE, 
		NUM_STEPS, BASE_ALPHA, CLAMP_VAL_MIN, CLAMP_VAL_MAX, NUM_STEPS_LIC,
		VELOCITY_SCALE, DT} FLOAT_PARAM;
	typedef enum { PROJECTION, ISPAUSED} BOOL_PARAMETER;
private:
	PROJ_TYPE _projection;
	bool _isPaused;
	map<FLOAT_PARAM, FloatParam> _floatParams;
public:
	PROJ_TYPE GetProjection() const;
	bool GetIsPaused() const;
	void SetTranslate(const vec3& in);
	void SetProjection(PROJ_TYPE in);
	void SetIsPaused(bool in);
	float GetFloatParameter(FLOAT_PARAM param) const;
	void SetFloatParameter(FLOAT_PARAM param, float in);

	static shared_ptr<THREEDLICParameters> GetInstance();
};


