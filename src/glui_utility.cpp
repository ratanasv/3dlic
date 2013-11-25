#include "StdAfx.h"
#include "glui_utility.h"
#include "property.h"

using std::map;
using std::pair;
using namespace vir;

map<THREEDLICParameters::FLOAT_PARAM, SliderBundle> BunchOfSliders;
map<THREEDLICParameters::BOOL_PARAMETER, CheckboxBundle> BunchOfCheckboxes;
map<int, RadioBundle> Bunch_Of_Radios;

void callback(int id) {
	
	for (auto it : BunchOfSliders) {
		it.second.refresh();
	}
	for (auto it : BunchOfCheckboxes) {
		it.second.refresh();
	}
	for (auto it : Bunch_Of_Radios) {
		it.second.refresh();
	}
	glutPostRedisplay( );//post redisplay
}

void slider45_factory(GLUI* main_glui, GLUI_Panel* panel, const SliderBundle& sb_in) {
	BunchOfSliders.insert(pair<THREEDLICParameters::FLOAT_PARAM, SliderBundle>(
		sb_in.id, sb_in));
	SliderBundle& sb = BunchOfSliders.at(sb_in.id);
	if(sb.two_sided)
		sb.slider = main_glui->add_slider_to_panel(panel, 1, GLUI_HSLIDER_FLOAT, 
			sb.vals, sb.id, (GLUI_Update_CB)callback);
	else
		sb.slider = main_glui->add_slider_to_panel(panel, 0, GLUI_HSLIDER_FLOAT, 
			sb.vals, sb.id, (GLUI_Update_CB)callback);
	sb.slider->set_w(200.0);
	sb.slider->set_float_limits(sb.bounds[0],sb.bounds[1]);
	sb.label = main_glui->add_statictext_to_panel(panel, "bbbb");
	sb.refresh();
	
}

void cbox45_factory(GLUI* main_glui, GLUI_Panel* panel, char* name, 
	const CheckboxBundle& cb_in) 
{
	BunchOfCheckboxes.insert(
		pair<THREEDLICParameters::BOOL_PARAMETER, CheckboxBundle>(
		cb_in.id, cb_in));
	auto& cb = BunchOfCheckboxes.at(cb_in.id);
	cb.checkbox = main_glui->add_checkbox_to_panel(panel, name, &cb.val, 
		cb.id, (GLUI_Update_CB)callback);
	cb.refresh();
}

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
	GLUI_Panel* panel = main_glui->add_rollout("project6", 1);
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(500.0, 500.0), vec2(500.0,500.0), vec2(0.0, 2000.0),
		THREEDLICParameters::NUM_STEPS, 
		"NUM_STEPS = %3.2f", false));
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(0.1, 1.0), vec2(0.1,1.0), vec2(0.0,1.0),
		THREEDLICParameters::BASE_ALPHA, 
		"ALPHA = %0.4f", false));
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(0.0, 1.0), vec2(0.0,1.0), vec2(0.0,1.0),
		THREEDLICParameters::CLAMP_VAL_MIN, 
		"clamp = %0.4f - %0.4f", true));
	// 	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(panel, &blah, -1, callback);
	// 	char* labels[] = {"texture enviroment","no texture", "modulate", "replace"};
	// 	RadioBundle(main_glui, panel, TEX_ENV, labels, 4, &env_radio_callback);
	// 	char* labels2[] = {"texture sampling", "nearest", "linear"};

}

static GLUI* Glui;				// instance of glui window