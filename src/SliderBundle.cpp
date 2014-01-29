#include "StdAfx.h"
#include "SliderBundle.h"
#include "glui.h"
#include "3dlic_model.h"


SliderBundle::SliderBundle(GLUI* main_glui, GLUI_Panel* panel, vec2<> def_vals,
						   vec2<> bounds, int id, const string format, void (*callback)(int id),
						   bool isDual /*= false*/) : 
	_slider(main_glui->add_slider_to_panel(panel, isDual, GLUI_HSLIDER_FLOAT,
	_vals, id, callback)),
	_label(main_glui->add_statictext_to_panel(panel, "bbbb")),
	_vals(_defVals), 
	_defVals(def_vals), 
	_bounds(bounds), 
	_callbackID(id), 
	_format(format), 
	_twoSided(isDual)
{
	_slider->set_w(200.0);
	_slider->set_float_limits(bounds.x, bounds.y);
	Refresh();
}

/** Data Binding
 * Need to sync between SliderBundle::_vals & THREEDLICParameters::_floatParams[id]
 * & SliderBundle::_label. 
 * The Mechanism
 * i) User interacts with a GLUI element
 * ii) GLUI Internally calls its callback func 
 * iii) In this case THE callback is GLUIPresentationLayer::OnGLUICallback
 * iv) OnGLUICallback then updates the model (THREEDLICParameters).
 * v) THREEDLICParameters then notify all observers of state changes.
 */

void SliderBundle::Update(const Observable* const observable) {
	auto lic = dynamic_cast<const THREEDLICParameters* const>(observable);
	if (lic) {
		Refresh();
	}
}

void SliderBundle::Refresh() {
	char string[128];
	if (_twoSided) {
		sprintf( string, _format.c_str(), _vals.x, _vals.y);
		_label->set_text(string);
		_slider->set_slider_val(_vals.x, _vals.y);
	} else {
		sprintf( string, _format.c_str(), _vals.x);
		_label->set_text(string);
		_slider->set_slider_val(_vals.x);
	}
	float dummy = _slider->float_low;
	dummy = _slider->float_high;
}

void SliderBundle::Reset() {
	_vals = _defVals;
	Refresh();
}

bool SliderBundle::IsDual() const {
	return _twoSided;
}

vec2<> SliderBundle::GetValue() const {
	return _vals;
}
