#include "StdAfx.h"
#include "LICFloatParam.h"
#include <atomic>
#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

std::atomic<int> nextID(0);

vector<LICFloatParam> LICFloatParam::INSTANCES;
LICFloatParam LICFloatParam::XROT("x-rotation");
LICFloatParam LICFloatParam::YROT("y-rotation"); 
LICFloatParam LICFloatParam::XTRANSLATE("x-translate"); 
LICFloatParam LICFloatParam::YTRANSLATE("y-translate");
LICFloatParam LICFloatParam::ZTRANSLATE("z-translate");
LICFloatParam LICFloatParam::NUM_STEPS("uNumSteps"); 
LICFloatParam LICFloatParam::BASE_ALPHA("uBaseAlpha"); 
LICFloatParam LICFloatParam::RAINBOW_VAL_MIN("uRainbowValMin"); 
LICFloatParam LICFloatParam::RAINBOW_VAL_MAX("uRainbowValMax"); 
LICFloatParam LICFloatParam::NUM_STEPS_LIC("uNumStepsLIC");
LICFloatParam LICFloatParam::VELOCITY_SCALE("uVelocityScale"); 
LICFloatParam LICFloatParam::DT("uDT"); 
LICFloatParam LICFloatParam::MAGNITUDE_MIN("uMinMagnitude"); 
LICFloatParam LICFloatParam::MAGNITUDE_MAX("uMaxMagnitude"); 
LICFloatParam LICFloatParam::COLOR_INTENSITY("uColorIntensity");
LICFloatParam LICFloatParam::NOISE_DENSITY("noise density");
LICFloatParam LICFloatParam::NOISE_SIGMA("noise sigma");
LICFloatParam LICFloatParam::Z_CUTTING_PLANE("uZ");
LICFloatParam LICFloatParam::TERMINATING_ALPHA("uTerminatingAlpha");

LICFloatParam::LICFloatParam(const char* stringRep) : 
	_stringRep(stringRep), _id(nextID++)
{
	INSTANCES.push_back(*this);
}

std::string LICFloatParam::GetStringRep()const {
	return _stringRep;
}

int LICFloatParam::GetID() const {
	return _id;
}

LICFloatParam LICFloatParam::FromID(const int id) {
	for (auto it=INSTANCES.begin(); it!=INSTANCES.end(); it++) {
 		if (it->GetID() == id) {
 			return *it;
 		}
	}
	throw logic_error("cannot convert int id to LICFloatParam");
}

bool LICFloatParam::operator<(const LICFloatParam& other) const {
	return this->GetID() < other.GetID();
}
