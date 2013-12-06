#include "StdAfx.h"
#include "glui_utility.h"
#include "property.h"
#include "3dlic_model.h"
#include "glui_presentation_layer.h"

using std::map;
using std::pair;
using namespace vir;

map<LICBoolParam, CheckboxBundle> BunchOfCheckboxes;
map<int, RadioBundle> Bunch_Of_Radios;

SliderBundle::SliderBundle(GLUI* main_glui, GLUI_Panel* panel, vec2<> def_vals,
	vec2<> bounds, int id, const string format, void (*callback)(int id),
	bool isDual /*= false*/) : 
	slider(main_glui->add_slider_to_panel(panel, isDual, GLUI_HSLIDER_FLOAT,
		_vals, id, callback)),
	_label(main_glui->add_statictext_to_panel(panel, "bbbb")),
	_vals(_defVals), 
	_defVals(def_vals), 
	_bounds(bounds), 
	_callbackID(id), 
	_format(format), 
	_twoSided(isDual)
{
	slider->set_w(200.0);
	slider->set_float_limits(bounds.x, bounds.y);
	Refresh();
}

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
		slider->set_slider_val(_vals.x, _vals.y);
	} else {
		sprintf( string, _format.c_str(), _vals.x);
		_label->set_text(string);
		slider->set_slider_val(_vals.x);
	}
	float dummy = slider->float_low;
	dummy = slider->float_high;
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

// void cbox45_factory(GLUI* main_glui, GLUI_Panel* panel, char* name, 
// 	const CheckboxBundle& cb_in) 
// {
// 	const int irrelavent = 0;
// 	BunchOfCheckboxes.insert(
// 		pair<LICBoolParam, CheckboxBundle>(
// 		cb_in.id, cb_in));
// 	auto& cb = BunchOfCheckboxes.at(cb_in.id);
// 	cb.checkbox = main_glui->add_checkbox_to_panel(panel, name, &cb.val, 
// 		irrelavent, (GLUI_Update_CB)callback);
// 	cb.refresh();
// }

void hsv_to_rgb(float hsv[3], float rgb[3])
{
	float tmp1 = hsv[2] * (1-hsv[1]);
	float tmp2 = hsv[2] * (1-hsv[1] * (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) ));
	float tmp3 = hsv[2] * (1-hsv[1] * (1 - (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) )));
	switch((int)(hsv[0] / 60)) {
	case 0:
		rgb[0] = hsv[2] ;
		rgb[1] = tmp3 ;
		rgb[2] = tmp1 ;
		break;
	case 1:
		rgb[0] = tmp2 ;
		rgb[1] = hsv[2] ;
		rgb[2] = tmp1 ;
		break;
	case 2:
		rgb[0] = tmp1 ;
		rgb[1] = hsv[2] ;
		rgb[2] = tmp3 ;
		break;
	case 3:
		rgb[0] = tmp1 ;
		rgb[1] = tmp2 ;
		rgb[2] = hsv[2] ;
		break;
	case 4:
		rgb[0] = tmp3 ;
		rgb[1] = tmp1 ;
		rgb[2] = hsv[2] ;
		break;
	case 5:
		rgb[0] = hsv[2] ;
		rgb[1] = tmp1 ;
		rgb[2] = tmp2 ;
		break;
	default:
		fprintf(stderr,  "What!? Inconceivable!\n");
	}
}


void RadioBundle::reset() {
	_counter = 0;
}

void RadioBundle::refresh() {
	_callback(_counter);
}


RadioBundle::RadioBundle( GLUI* main_glui, GLUI_Panel* panel, int id, 
char** labels, int num_label, void (*callback)(int)) {
	Bunch_Of_Radios[id] = *this;
	RadioBundle& rad = Bunch_Of_Radios[id];
	rad._id = id;
	rad._counter = 0;
	rad._callback = callback;
	GLUI_Panel* tiny_panel = main_glui->add_panel_to_panel(panel, labels[0]);
	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(tiny_panel, 
		&rad._counter,_id, callback);
	for (int i=1; i<num_label; i++) {
		main_glui->add_radiobutton_to_group(group, labels[i]);
	}
}



void create_proj6_panel(GLUI* main_glui) {
	GLUI_Panel* panel = main_glui->add_rollout("vir's project", 1);
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::NUM_STEPS, "NUM_STEPS = %3.2f");
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::BASE_ALPHA, "ALPHA = %0.4f");
	GLUIPresentationLayer::INSTANCE->InsertDualSlider(main_glui, panel, 
		LICFloatParam::RAINBOW_VAL_MIN, LICFloatParam::RAINBOW_VAL_MAX,
		"rainbow clamp = %0.4f - %0.4f");
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::NUM_STEPS_LIC, "num_steps_lic = %3.1f");
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::VELOCITY_SCALE, "velocity_scale = %0.4f");
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::DT, "dt = %0.4f");
	GLUIPresentationLayer::INSTANCE->InsertDualSlider(main_glui, panel, 
		LICFloatParam::MAGNITUDE_MIN, LICFloatParam::MAGNITUDE_MAX, 
		"magnitude = %4.4f - %4.4f");
	GLUIPresentationLayer::INSTANCE->InsertSlider(main_glui, panel, 
		LICFloatParam::COLOR_INTENSITY, "color intensity = %4.4f");
		
	// 	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(panel, &blah, -1, callback);
	// 	char* labels[] = {"texture enviroment","no texture", "modulate", "replace"};
	// 	RadioBundle(main_glui, panel, TEX_ENV, labels, 4, &env_radio_callback);
	// 	char* labels2[] = {"texture sampling", "nearest", "linear"};

}

static GLUI* Glui;				// instance of glui window
