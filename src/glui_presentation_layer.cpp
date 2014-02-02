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
	const int callbackID = param.GetID();
	const FloatParam floatParam = GetTDLPInstance().GetFloatParameter(param);
	vec2<> val(floatParam.GetDefaultVal(), floatParam.GetDefaultVal());
	vec2<> bounds(floatParam.GetMinVal(), floatParam.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), CallbackWrapper ));
	_sliders[param] = sliderBundle;
	GetTDLPInstance().AddObserver(sliderBundle);
}

void GLUIPresentationLayer::InsertDualSlider(GLUI* main_glui, GLUI_Panel* panel, 
	LICFloatParam paramOne, LICFloatParam paramTwo, const string& label)
{
	const int callbackID = GetCallbackID(paramOne, paramTwo);
	const FloatParam floatParamOne = GetTDLPInstance().
		GetFloatParameter(paramOne);
	const FloatParam floatParamTwo = GetTDLPInstance().
		GetFloatParameter(paramTwo);
	vec2<> val(floatParamOne.GetDefaultVal(), floatParamTwo.GetDefaultVal());
	vec2<> bounds(floatParamOne.GetMinVal(), floatParamOne.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), CallbackWrapper, true));
	_sliders[paramOne] = sliderBundle;
	InsertHelper(_dualSliderIDs, paramOne, paramTwo);
	GetTDLPInstance().AddObserver(sliderBundle);
}

void GLUIPresentationLayer::InsertCheckBox(GLUI* main_glui, GLUI_Panel* panel, 
	LICBoolParam param, const string& label)
{
	const int callbackID = static_cast<int>(param);
	const BoolParam boolParam = GetTDLPInstance().GetBoolParameter(param);
	CheckboxBundle::Builder builder;
	builder.WithMainGLUI(main_glui)
		.WithPanel(panel)
		.WithLabel(label)
		.WithDefaultValue(boolParam.GetDefaultVal())
		.WithCallbackID(callbackID)
		.WithCallback(CallbackWrapper2);
		
	shared_ptr<CheckboxBundle> checkboxBundle(new CheckboxBundle(builder));
	_checkboxes[param] = checkboxBundle;
	GetTDLPInstance().AddObserver(checkboxBundle);	
}

int GLUIPresentationLayer::GetCallbackID(LICFloatParam pone, LICFloatParam ptwo) {
	return pone.GetID();
}

void GLUIPresentationLayer::OnFloatCallback(const int callbackID) {
	LICFloatParam which = LICFloatParam::FromID(callbackID);
	GetTDLPInstance().SetFloatParameter(which, 
		_sliders.at(which)->GetValue().x);
	if (_sliders.at(which)->IsDual()) {
		LICFloatParam otherEnd = _dualSliderIDs.at(which);
		GetTDLPInstance().SetFloatParameter(otherEnd, 
			_sliders.at(which)->GetValue().y);
	}
}

void GLUIPresentationLayer::OnBooleanCallback(const int callbackID) {
	LICBoolParam which = static_cast<LICBoolParam>(callbackID);
	GetTDLPInstance().SetBoolParameter(which, 
		_checkboxes.at(which)->_val);
}