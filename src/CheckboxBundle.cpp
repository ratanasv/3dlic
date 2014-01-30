#include "StdAfx.h"
#include "CheckboxBundle.h"
#include "glui.h"

void CheckboxBundle::reset() {
	_val = _defVal;
}

void CheckboxBundle::refresh() {

}

void CheckboxBundle::Update(const Observable* const observable) {

}

CheckboxBundle::CheckboxBundle(const Builder& b) : 
	_val(b._defVal), _defVal(b._defVal), _callbackID(b._callbackID), 
	_label(b._label)
{
	_checkbox = b._mainGlui->add_checkbox_to_panel(b._panel, 
		const_cast<char*>(_label.c_str()), &_val, _callbackID, b._callback);
}

CheckboxBundle CheckboxBundle::Builder::Build() {
	return CheckboxBundle(*this);
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithMainGLUI(GLUI* main_glui) {
	_mainGlui = main_glui;
	return *this;
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithPanel(GLUI_Panel* panel) {
	_panel = panel;
	return *this;
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithDefaultValue(int val) {
	_defVal = val;
	return *this;
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithLabel(const std::string& label) {
	_label = label;
	return *this;
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithCallbackID(int id) {
	_callbackID = id;
	return *this;
}

CheckboxBundle::Builder& CheckboxBundle::Builder::WithCallback(void (*callback)(int)) {
	_callback = callback;
	return *this;
}
