#include "StdAfx.h"
#include "3dlic_model.h"
#include <stdexcept>
#include "LICFloatParam.h"
#include <mutex>
#include <boost/thread/locks.hpp>

using std::invalid_argument;

FloatParam::FloatParam() : _defaultVal(0.5), _minVal(0.0), _maxVal(1.0), 
	_val(_defaultVal) {};

FloatParam::FloatParam( float a, float b, float c ) : 
	_minVal(b), _maxVal(c), _defaultVal(a)
{
	SetFloat(_defaultVal);
}

float FloatParam::GetFloat() const {
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
	InsertHelper(_floatParams, LICFloatParam::XROT, FloatParam(0.0, -1000.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::YROT, FloatParam(0.0, -1000.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::XTRANSLATE, FloatParam(0.0, -1000.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::YTRANSLATE, FloatParam(0.0, -1000.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::ZTRANSLATE, FloatParam(0.0, -1000.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::NUM_STEPS, FloatParam(100.0, 0.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::BASE_ALPHA, FloatParam(0.7, 0.0, 1.0));
	InsertHelper(_floatParams, LICFloatParam::RAINBOW_VAL_MIN, FloatParam(0.1, 0.0, 1.0));
	InsertHelper(_floatParams, LICFloatParam::RAINBOW_VAL_MAX, FloatParam(0.3, 0.0, 1.0));
	InsertHelper(_floatParams, LICFloatParam::NUM_STEPS_LIC, FloatParam(20.0, 0.0, 100.0));
	InsertHelper(_floatParams, LICFloatParam::VELOCITY_SCALE, FloatParam(0.01, 0.001, 1.0));
	InsertHelper(_floatParams, LICFloatParam::DT, FloatParam(0.01, 0.0, 0.3));
	InsertHelper(_floatParams, LICFloatParam::MAGNITUDE_MIN, FloatParam(0.01, 0.0, 1.0));
	InsertHelper(_floatParams, LICFloatParam::MAGNITUDE_MAX, FloatParam(0.3, 0.0, 1.0));
	InsertHelper(_floatParams, LICFloatParam::COLOR_INTENSITY, FloatParam(100.0, 1.0, 1000.0));
	InsertHelper(_floatParams, LICFloatParam::NOISE_DENSITY, FloatParam(500.0, 1.0, 2000.0));
	InsertHelper(_floatParams, LICFloatParam::NOISE_SIGMA, FloatParam(1.0, 0.0, 8.0));
	InsertHelper(_boolParams, LICBoolParam::CUTTING_PLANE, BoolParam(false));
	InsertHelper(_floatParams, LICFloatParam::Z_CUTTING_PLANE, FloatParam(0.0, -1.0, 1.0));
	InsertHelper(_boolParams, LICBoolParam::SHOW_FORWARD_VECTOR_DIFF, BoolParam(false));
	InsertHelper(_boolParams, LICBoolParam::SHOW_NUM_ITERATIONS, BoolParam(false));
	InsertHelper(_floatParams, LICFloatParam::TERMINATING_ALPHA, FloatParam(0.9, 0.0, 1.0));
	_projection = PROJ_TYPE::PERSP;
}

PROJ_TYPE THREEDLICParameters::GetProjection() const {
	boost::shared_lock<boost::shared_mutex> synchronous(_mutex);
	return _projection;
}



void THREEDLICParameters::SetProjection( PROJ_TYPE in ) {
	{
		std::lock_guard<boost::shared_mutex> synchronous(_mutex);
		_projection = in;
	}
	NotifyObservers();
}



FloatParam THREEDLICParameters::GetFloatParameter(const LICFloatParam& param) const {
	boost::shared_lock<boost::shared_mutex> synchronous(_mutex);
	return _floatParams.at(param);
}

void THREEDLICParameters::SetFloatParameter(const LICFloatParam& param, float in) {
	{
		std::lock_guard<boost::shared_mutex> synchronous(_mutex);
		_floatParams.at(param).SetFloat(in);
	}
	NotifyObservers();
}

BoolParam THREEDLICParameters::GetBoolParameter(LICBoolParam param) const {
	boost::shared_lock<boost::shared_mutex> synchronous(_mutex);
	return _boolParams.at(param);
}

void THREEDLICParameters::SetBoolParameter(LICBoolParam param, bool in) {
	{
		std::lock_guard<boost::shared_mutex> synchronous(_mutex);
		_boolParams.at(param).SetBool(in);
	}
	NotifyObservers();
}


BoolParam::BoolParam(bool defaultVal) : _defaultVal(defaultVal) {
	SetBool(_defaultVal);
}

BoolParam::~BoolParam() {

}

bool BoolParam::GetDefaultVal() const {
	return _defaultVal;
}

bool BoolParam::GetBool() const {
	return _val;
}

void BoolParam::SetBool(bool in) {
	_val = in;
}

THREEDLICParameters& GetTDLPInstance() {
	static THREEDLICParameters INSTANCE;
	return INSTANCE;
}
