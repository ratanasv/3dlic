#pragma once
#include "model_controller.h"
#include "abstract.h"
#include <map>

using std::map;
using vir::Abstract;
using Angel::vec2;
using std::pair;

class FloatParam {
public:
	FloatParam();
	FloatParam(float a, float b, float c);
	operator float();
	operator const float() const;
	void SetFloat(float in);
	float GetMinVal() const;
	float GetMaxVal() const;
	float GetDefaultVal() const;
	void Reset();
private:
	float _minVal;
	float _maxVal;
	float _defaultVal;
	float _val;
};

enum class LICFloatParam : int {
	XROT, YROT, XTRANSLATE, YTRANSLATE, ZTRANSLATE, 
	NUM_STEPS, BASE_ALPHA, CLAMP_VAL_MIN, CLAMP_VAL_MAX, NUM_STEPS_LIC,
	VELOCITY_SCALE, DT
};

enum class LICBoolParam : int {
	PROJECTION, ISPAUSED
};


class THREEDLICParameters : public Observable {
private: //enforces a singleton property.
	THREEDLICParameters();
	THREEDLICParameters(const THREEDLICParameters& other);
public:
	typedef enum {ORTHO, PERSP} PROJ_TYPE;
private:
	PROJ_TYPE _projection;
	bool _isPaused;
	map<LICFloatParam, FloatParam> _floatParams;
public:
	PROJ_TYPE GetProjection() const;
	bool GetIsPaused() const;
	void SetTranslate(const vec3& in);
	void SetProjection(PROJ_TYPE in);
	void SetIsPaused(bool in);
	FloatParam GetFloatParameter(LICFloatParam param) const;
	void SetFloatParameter(LICFloatParam param, float in);

	static THREEDLICParameters* INSTANCE;
};