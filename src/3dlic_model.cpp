#include "StdAfx.h"
#include "3dlic_model.h"
#include <stdexcept>

using std::invalid_argument;

FloatParam::operator float() {
	return _val;
}

FloatParam::operator const float() const {
	return _val;
}

FloatParam::FloatParam( float a, float b, float c ) {
	_minVal = b;
	_maxVal = c;
	SetFloat(a);
}

FloatParam::FloatParam() : _val(0.5), _minVal(0.0), _maxVal(1.0) {}

void FloatParam::SetFloat(float in) {
	if (in <= _maxVal && in >= _minVal) {
		_val = in;
	} else {
		throw invalid_argument("valid not within bound");
	}
}



THREEDLICParameters::THREEDLICParameters() {
	_floatParams[XROT] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[YROT] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[XTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[YTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[ZTRANSLATE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[SCALE] = FloatParam(0.0, -1000.0, 1000.0);
	_floatParams[NUM_STEPS] =  FloatParam(500.0, 0.0, 1000.0);
	_floatParams[BASE_ALPHA] = FloatParam(0.1, 0.0, 1.0);
	_floatParams[CLAMP_VAL_MIN] = FloatParam(0.01, 0.0, 1.0);
	_floatParams[CLAMP_VAL_MAX] = FloatParam(1.0, 0.0, 1.0);
	_floatParams[NUM_STEPS_LIC] = FloatParam(20.0, 0.0, 100.0);
	_floatParams[VELOCITY_SCALE] = FloatParam(0.05, 0.0, 2.0);
	_floatParams[DT] = FloatParam(0.05, 0.0001, 0.3);
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


float THREEDLICParameters::GetFloatParameter( FLOAT_PARAM param ) const {
	return _floatParams.at(param);
}

void THREEDLICParameters::SetFloatParameter( FLOAT_PARAM param, float in ) {
	_floatParams.at(param).SetFloat(in);
}


shared_ptr<THREEDLICParameters> THREEDLICParameters::GetInstance() {
	static shared_ptr<THREEDLICParameters> INSTANCE(new THREEDLICParameters());
	return INSTANCE;
}

