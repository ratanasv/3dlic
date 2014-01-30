#include "StdAfx.h"
#include "glui_presentation_layer.h"
#include "3dlic_model.h"
#include "glui_utility.h"
#include "glui.h"
#include "SliderBundle.h"
#include "CheckboxBundle.h"

GLUIPresentationLayer* GLUIPresentationLayer::INSTANCE = new GLUIPresentationLayer();

void CallbackWrapper(int id) {
	GLUIPresentationLayer::INSTANCE->OnFloatCallback(id);
}

void CallbackWrapper2(int id) {
	GLUIPresentationLayer::INSTANCE->OnBooleanCallback(id);
}

void GLUIPresentationLayer::InsertSlider(GLUI* main_glui, GLUI_Panel* panel, 
	LICFloatParam param, const string& label) 
{
	const int callbackID = static_cast<int>(param);
	const FloatParam floatParam = THREEDLICParameters::INSTANCE->GetFloatParameter(param);
	vec2<> val(floatParam.GetDefaultVal(), floatParam.GetDefaultVal());
	vec2<> bounds(floatParam.GetMinVal(), floatParam.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), CallbackWrapper ));
	_sliders[param] = sliderBundle;
	THREEDLICParameters::INSTANCE->AddObserver(sliderBundle);
}

void GLUIPresentationLayer::InsertDualSlider(GLUI* main_glui, GLUI_Panel* panel, 
	LICFloatParam paramOne, LICFloatParam paramTwo, const string& label)
{
	const int callbackID = GetCallbackID(paramOne, paramTwo);
	const FloatParam floatParamOne = THREEDLICParameters::INSTANCE->
		GetFloatParameter(paramOne);
	const FloatParam floatParamTwo = THREEDLICParameters::INSTANCE->
		GetFloatParameter(paramTwo);
	vec2<> val(floatParamOne.GetDefaultVal(), floatParamTwo.GetDefaultVal());
	vec2<> bounds(floatParamOne.GetMinVal(), floatParamOne.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), CallbackWrapper, true));
	_sliders[paramOne] = sliderBundle;
	_dualSliderIDs[paramOne] = paramTwo;
	THREEDLICParameters::INSTANCE->AddObserver(sliderBundle);
}

void GLUIPresentationLayer::InsertCheckBox(GLUI* main_glui, GLUI_Panel* panel, 
	LICBoolParam param, const string& label)
{
	const int callbackID = static_cast<int>(param);
	const BoolParam boolParam = THREEDLICParameters::INSTANCE->GetBoolParameter(param);
	CheckboxBundle::Builder builder;
	builder.WithMainGLUI(main_glui)
		.WithPanel(panel)
		.WithLabel(label)
		.WithDefaultValue(boolParam.GetDefaultVal())
		.WithCallbackID(callbackID)
		.WithCallback(CallbackWrapper2);
		
	shared_ptr<CheckboxBundle> checkboxBundle(new CheckboxBundle(builder));
	_checkboxes[param] = checkboxBundle;
	THREEDLICParameters::INSTANCE->AddObserver(checkboxBundle);	
}

int GLUIPresentationLayer::GetCallbackID(LICFloatParam pone, LICFloatParam ptwo) {
	return static_cast<int>(pone);
}

void GLUIPresentationLayer::OnFloatCallback(const int callbackID) {
	LICFloatParam which = static_cast<LICFloatParam>(callbackID);
	THREEDLICParameters::INSTANCE->SetFloatParameter(which, 
		_sliders.at(which)->GetValue().x);
	if (_sliders.at(which)->IsDual()) {
		LICFloatParam otherEnd = _dualSliderIDs.at(which);
		THREEDLICParameters::INSTANCE->SetFloatParameter(otherEnd, 
			_sliders.at(which)->GetValue().y);
	}
}

void GLUIPresentationLayer::OnBooleanCallback(const int callbackID) {
	LICBoolParam which = static_cast<LICBoolParam>(callbackID);
	THREEDLICParameters::INSTANCE->SetBoolParameter(which, 
		_checkboxes.at(which)->_val);
}