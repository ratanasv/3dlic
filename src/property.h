#pragma once
#include <cstring>

/**
 * Wrapper around boost::program_options. This is designed to be a singleton.
 * Will throw std::logic_error if methods fail to produce valid values.
 */

using std::string;

enum class Property {WINDOW_SIZE, WINDOW_TITLE, PATH_DATA, PATH_VERTEX_SHADER, 
	PATH_FRAGMENT_SHADER, PATH_NOISE, PATH_GAUSSIAN_COMPUTE_SHADER, PATH_KERNEL,
	PATH_NOISE2D, PATH_VS_2DLIC, PATH_FS_2DLIC};


void InitProgramOptions(int argc, char** argv);
int GetIntProperty(Property prop);
string GetStringProperty(Property prop);
float GetFloatProperty(Property prop);
