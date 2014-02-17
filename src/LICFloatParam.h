#pragma once

#include <vector>
#include <cstring>

/** Warning, static initialization order is undefined. Hence these "enum" instances 
 * might not be properly initialized if used in other instance variables.
 */

class LICFloatParam {
private:
	LICFloatParam(const char* stringRep);
	const std::string _stringRep;
	static std::vector<LICFloatParam> INSTANCES;
	const int _id;
public:
	static LICFloatParam XROT;
	static LICFloatParam YROT; 
	static LICFloatParam XTRANSLATE; 
	static LICFloatParam YTRANSLATE;
	static LICFloatParam ZTRANSLATE;
	static LICFloatParam NUM_STEPS; 
	static LICFloatParam BASE_ALPHA; 
	static LICFloatParam RAINBOW_VAL_MIN; 
	static LICFloatParam RAINBOW_VAL_MAX; 
	static LICFloatParam NUM_STEPS_LIC;
    static LICFloatParam VELOCITY_SCALE; 
	static LICFloatParam DT; 
	static LICFloatParam MAGNITUDE_MIN; 
	static LICFloatParam MAGNITUDE_MAX; 
	static LICFloatParam COLOR_INTENSITY;
	static LICFloatParam NOISE_DENSITY;
	static LICFloatParam NOISE_SIGMA;

	std::string GetStringRep() const;
	int GetID() const;
	static LICFloatParam FromID(const int id);
	bool operator< (const LICFloatParam& other) const;
};