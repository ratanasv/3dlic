#pragma once
#include "StdAfx.h"

struct ElevationData {
	float MinLng, MaxLng,  MinLat, MaxLat;
	int NumLngs, NumLats;
	float MinHgt, MaxHgt;
	std::shared_ptr<std::vector<float>> Grid;
};

void create_proj6_panel(GLUI* main_glui);
void draw6();
void reset6();
void animate6(float t);
void init_light();
void init6();

