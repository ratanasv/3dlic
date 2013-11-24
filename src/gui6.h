#pragma once
#include "glslprogram.h"
#include "memory"

using std::shared_ptr;

void draw6();
void reset6();
void animate6(float t);
void init6();

extern shared_ptr<GLSLProgram> VolumeTracingShader;