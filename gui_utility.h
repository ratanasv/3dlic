#pragma once
#include "StdAfx.h"



struct SliderBundle{
	GLUI_HSlider* slider;
	GLUI_StaticText* label;
	vir::vec2 vals;
	vir::vec2 def_vals;
	vir::vec2 bounds;
	int id;
	const char* format;
	bool two_sided;
	SliderBundle(GLUI_HSlider* slider, GLUI_StaticText* label, vir::vec2 vals, 
		vir::vec2 def_vals, vir::vec2 bounds, int id, const char* format, 
		bool is_two_sided = false): slider(slider), 
		label(label), vals(vals), def_vals(def_vals), bounds(bounds), 
		id(id), format(format), two_sided(is_two_sided){};
	SliderBundle(): slider(NULL), label(NULL), vals(vir::vec2()), 
		def_vals(vir::vec2()), bounds(vir::vec2(-1.0,1.0)), id(-1), format(NULL){};
	void refresh()
	{
		char string[128];
		if(two_sided){
			sprintf( string, format, vals.x, vals.y);
			label->set_text(string);
			slider->set_slider_val(vals.x, vals.y);
		}else{
			sprintf( string, format, vals.x);
			label->set_text(string);
			slider->set_slider_val(vals.x);
		}
	}
	void reset()
	{	
		vals = def_vals;
		refresh();
	}
};
struct CheckboxBundle{
	GLUI_Checkbox* checkbox;
	int val;
	int def_val;
	int id;
	CheckboxBundle(GLUI_Checkbox* checkbox, int val, int def_val, int id):
	checkbox(checkbox), val(val), def_val(def_val), id(id){};
	CheckboxBundle(): checkbox(NULL), val(0), def_val(0), id(-1){};
	void refresh()
	{

	}
	void reset()
	{
		val = def_val;
	}
};

enum{T_CLAMP, ALPHA};
void callback(int id);
void slider45_factory(GLUI* main_glui, GLUI_Panel* panel, 
struct SliderBundle& sb_in);
void cbox45_factory(GLUI* main_glui, GLUI_Panel* panel, char* name, 
struct CheckboxBundle& cb_in);

class RadioBundle{
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

extern std::map<int, struct SliderBundle> Bunch_Of_Sliders;
extern std::map<int, struct CheckboxBundle> Bunch_Of_Checkboxes;
extern std::map<int, RadioBundle> Bunch_Of_Radios;
