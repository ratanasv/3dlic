#pragma once
#include "model_controller.h"
#include "abstract.h"
#include <map>
#include "LICFloatParam.h"
#include <boost/thread/shared_mutex.hpp>

using std::map;
using vir::Abstract;
using Angel::vec2;
using std::pair;

class FloatParam {
public:
	FloatParam();
	FloatParam(float a, float b, float c);
	void SetFloat(float in);
	float GetMinVal() const;
	float GetMaxVal() const;
	float GetDefaultVal() const;
	void Reset();
	float GetFloat() const;
private:
	const float _minVal;
	const float _maxVal;
	const float _defaultVal;
	float _val;
};

class BoolParam {
public:
	BoolParam(bool defaultVal);
	virtual ~BoolParam();
private:
	const bool _defaultVal;
	bool _val;
public:
	bool GetDefaultVal() const;
	bool GetBool() const;
	void SetBool(bool in);
};

// enum class LICFloatParam : int {
// 	XROT, YROT, XTRANSLATE, YTRANSLATE, ZTRANSLATE, 
// 	NUM_STEPS, BASE_ALPHA, RAINBOW_VAL_MIN, RAINBOW_VAL_MAX, NUM_STEPS_LIC,
// 	VELOCITY_SCALE, DT, MAGNITUDE_MIN, MAGNITUDE_MAX, COLOR_INTENSITY
// };

enum class LICBoolParam : int {
	PROJECTION, ISPAUSED, CUTTING_PLANE, SHOW_FORWARD_VECTOR_DIFF
};

enum class PROJ_TYPE : int {
	ORTHO, PERSP
};


class THREEDLICParameters : public Observable {
private: //enforces a singleton property.
	THREEDLICParameters();
	THREEDLICParameters(const THREEDLICParameters& other);
private:
	PROJ_TYPE _projection;
	map<LICFloatParam, FloatParam> _floatParams;
	map<LICBoolParam, BoolParam> _boolParams;
	mutable boost::shared_mutex _mutex;
public:
	PROJ_TYPE GetProjection() const;
	void SetTranslate(const vec3<>& in);
	void SetProjection(PROJ_TYPE in);
	FloatParam GetFloatParameter(const LICFloatParam& param) const;
	BoolParam GetBoolParameter(LICBoolParam param) const;
	void SetFloatParameter(const LICFloatParam& param, float in);
	void SetBoolParameter(LICBoolParam param, bool in);

	friend THREEDLICParameters& GetTDLPInstance();
};

THREEDLICParameters& GetTDLPInstance();