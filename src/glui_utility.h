#pragma once
#include "vec.h"
#include "glui.h"
#include "3dlic_model.h"


using Angel::vec2;
using Angel::vec3;


class SliderBundle {
public:
	GLUI_HSlider* slider;
	GLUI_StaticText* label;
	vec2 vals;
	vec2 def_vals;
	vec2 bounds;
	THREEDLICParameters::FLOAT_PARAM id;
	const char* format;
	bool two_sided;
	SliderBundle(GLUI_HSlider* slider, GLUI_StaticText* label, vec2 vals, 
		vec2 def_vals, vec2 bounds, THREEDLICParameters::FLOAT_PARAM id, 
		const char* format, bool is_two_sided = false) : slider(slider), 
		label(label), vals(vals), def_vals(def_vals), bounds(bounds), 
		id(id), format(format), two_sided(is_two_sided){};

	void refresh() {
		char string[128];
		if (two_sided) {
			sprintf( string, format, vals.x, vals.y);
			label->set_text(string);
			slider->set_slider_val(vals.x, vals.y);
		} else {
			sprintf( string, format, vals.x);
			label->set_text(string);
			slider->set_slider_val(vals.x);
		}
	}
	void reset() {	
		vals = def_vals;
		refresh();
	}
};

class CheckboxBundle {
public:
	GLUI_Checkbox* checkbox;
	int val;
	int def_val;
	THREEDLICParameters::BOOL_PARAMETER id;
	CheckboxBundle(GLUI_Checkbox* checkbox, int val, int def_val, 
		THREEDLICParameters::BOOL_PARAMETER id ):
		checkbox(checkbox), val(val), def_val(def_val), id(id) {};
	void refresh() {

	}
	void reset() {
		val = def_val;
	}
};

void callback(int id);
void slider45_factory(GLUI* main_glui, GLUI_Panel* panel, 
	const SliderBundle& sb_in);
void cbox45_factory(GLUI* main_glui, GLUI_Panel* panel, char* name, 
	const CheckboxBundle& cb_in);

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

extern map<THREEDLICParameters::FLOAT_PARAM, SliderBundle> BunchOfSliders;
extern map<THREEDLICParameters::BOOL_PARAMETER, CheckboxBundle> BunchOfCheckboxes;
extern map<int, RadioBundle> Bunch_Of_Radios;
