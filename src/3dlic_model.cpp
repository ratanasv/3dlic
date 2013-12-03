#include "StdAfx.h"
#include "3dlic_model.h"
#include <stdexcept>

using std::invalid_argument;

THREEDLICParameters* THREEDLICParameters::INSTANCE = new THREEDLICParameters();

FloatParam::FloatParam() : _defaultVal(0.5), _minVal(0.0), _maxVal(1.0), 
	_val(_defaultVal) {};

FloatParam::FloatParam( float a, float b, float c ) {
	_minVal = b;
	_maxVal = c;
	_defaultVal = a;
	SetFloat(_defaultVal);
}

FloatParam::operator float() {
	return _val;
}

FloatParam::operator const float() const {
	return _val;
}

void FloatParam::SetFloat(float in) {
	if (in <= _maxVal && in >= _minVal) {
		_val = in;
	} else {
		throw invalid_argument("valid not within bound");
	}
}

float FloatParam::GetMinVal() const {
	return _minVal;
}

float FloatParam::GetMaxVal() const {
	return _maxVal;
}

float FloatParam::GetDefaultVal() const {
	return _defaultVal;
}

void FloatParam::Reset() {
	_val = _defaultVal;
}

THREEDLICParameters::THREEDLICParameters() {
	_floatParams[LICFloatParam::XROT] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[LICFloatParam::YROT] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[LICFloatParam::XTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[LICFloatParam::YTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[LICFloatParam::ZTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[LICFloatParam::NUM_STEPS] =  FloatParam(256.0, 0.0, 1000.0);
	_floatParams[LICFloatParam::BASE_ALPHA] = FloatParam(0.7, 0.0, 1.0);
	_floatParams[LICFloatParam::CLAMP_VAL_MIN] = FloatParam(0.1, 0.0, 1.0);
	_floatParams[LICFloatParam::CLAMP_VAL_MAX] = FloatParam(0.3, 0.0, 1.0);
	_floatParams[LICFloatParam::NUM_STEPS_LIC] = FloatParam(20.0, 0.0, 100.0);
	_floatParams[LICFloatParam::VELOCITY_SCALE] = FloatParam(0.01, 0.001, 1.0);
	_floatParams[LICFloatParam::DT] = FloatParam(0.05, 0.0001, 0.3);
	_projection = PERSP;
	_isPaused = false;
}

THREEDLICParameters::PROJ_TYPE THREEDLICParameters::GetProjection() const {
	return _projection;
}

bool THREEDLICParameters::GetIsPaused() const {
	return _isPaused;
}


void THREEDLICParameters::SetProjection( PROJ_TYPE in ) {
	_projection = in;
	NotifyObservers();
}

void THREEDLICParameters::SetIsPaused( bool in ) {
	_isPaused = in;
	NotifyObservers();
}


FloatParam THREEDLICParameters::GetFloatParameter( LICFloatParam param ) const {
	return _floatParams.at(param);
}

void THREEDLICParameters::SetFloatParameter( LICFloatParam param, float in ) {
	_floatParams.at(param).SetFloat(in);
	NotifyObservers();
}

