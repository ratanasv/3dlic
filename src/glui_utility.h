#pragma once
#include "vec.h"
#include "glui.h"
#include "model_controller.h"


using Angel::vec2;
using Angel::vec3;

class LICFloatParam;
enum class LICBoolParam : int;


class RadioBundle {
public:
	int _id;
	RadioBundle(){};
	RadioBundle( GLUI* main_glui, GLUI_Panel* panel, int id, char** labels, 
		int num_label, void (*callback)(int));
	void refresh();
	void reset();
	int _counter;
	void (*_callback)(int);
};

void create_proj6_panel(GLUI* main_glui);
