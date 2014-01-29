#include "StdAfx.h"
#include "CheckboxBundle.h"
#include "glui.h"

void CheckboxBundle::reset() {
	_val = _defVal;
}

void CheckboxBundle::refresh() {

}

CheckboxBundle::CheckboxBundle(GLUI* main_glui, GLUI_Panel* panel, int val, int def_val, 
	int _callbackID, void (*callback)(int id)) : 
	_val(val), _defVal(def_val), _callbackID(_callbackID) 
{
	_checkbox = main_glui->add_checkbox_to_panel(panel, "unused name", &_val, 
		_callbackID, callback);
}

void CheckboxBundle::Update(const Observable* const observable) {

}

