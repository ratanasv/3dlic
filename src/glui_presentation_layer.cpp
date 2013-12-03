#include "StdAfx.h"
#include "glui_presentation_layer.h"
#include "3dlic_model.h"

GLUIPresentationLayer* GLUIPresentationLayer::INSTANCE = new GLUIPresentationLayer();

void GLUIPresentationLayer::InsertSlider(GLUI* main_glui, GLUI_Panel* panel, 
										 LICFloatParam param, const string& label) 
{
	const int callbackID = static_cast<int>(param);
	const FloatParam floatParam = THREEDLICParameters::INSTANCE->GetFloatParameter(param);
	vec2 val(floatParam.GetDefaultVal(), floatParam.GetDefaultVal());
	vec2 bounds(floatParam.GetMinVal(), floatParam.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), false));
	_sliders[param] = sliderBundle;
}

void GLUIPresentationLayer::InsertDualSlider(GLUI* main_glui, GLUI_Panel* panel, 
											 LICFloatParam paramOne, LICFloatParam paramTwo, const string& label)
{
	const int callbackID = GetCallbackID(paramOne, paramTwo);
	const FloatParam floatParamOne = THREEDLICParameters::INSTANCE->
		GetFloatParameter(paramOne);
	const FloatParam floatParamTwo = THREEDLICParameters::INSTANCE->
		GetFloatParameter(paramTwo);
	vec2 val(floatParamOne.GetDefaultVal(), floatParamTwo.GetDefaultVal());
	vec2 bounds(floatParamOne.GetMinVal(), floatParamOne.GetMaxVal());

	shared_ptr<SliderBundle> sliderBundle(new SliderBundle(main_glui, panel,
		val, bounds, callbackID, label.c_str(), false));
	_sliders[paramOne] = sliderBundle;
}

int GLUIPresentationLayer::GetCallbackID(LICFloatParam pone, LICFloatParam ptwo) {
	return static_cast<int>(pone);
}