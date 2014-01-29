#pragma once
#include "vec.h"
#include "model_controller.h"

class GLUI_Checkbox;
class GLUI;
class GLUI_Panel;

class CheckboxBundle : public Observer {
public:
	GLUI_Checkbox* _checkbox;
	int _val;
	const int _defVal;
	const int _callbackID;
	CheckboxBundle(GLUI* main_glui, GLUI_Panel* panel, int val, int def_val, int id, 
		void (*callback)(int id));
	void refresh();
	void reset();
	virtual void Update(const Observable* const observable);
};