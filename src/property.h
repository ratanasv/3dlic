#pragma once
#include <cstring>

/**
 * Wrapper around boost::program_options. This is designed to be a singleton.
 * Will throw std::logic_error if methods fail to produce valid values.
 */

using std::string;

typedef enum {WINDOW_SIZE, WINDOW_TITLE} Property;


void InitProgramOptions(int argc, char** argv);
int GetIntProperty(Property prop);
string GetStringProperty(Property prop);
float GetFloatProperty(Property prop);
