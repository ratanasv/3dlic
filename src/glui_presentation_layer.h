#pragma once
#include <map>
#include <memory>
#include <cstring>


using std::map;
using std::shared_ptr;
using std::string;

enum class LICFloatParam : int;
enum class LICBoolParam : int;
class SliderBundle;
class CheckboxBundle;
class GLUI;
class GLUI_Panel;

void CallbackWrapper(int id);
void CallbackWrapper2(int id);

/** Guideline for adding another GLSL uniform variable.
 * i) Add your enum to THREEDLICParameters.
 * ii) Add default values and register it in THREEDLICParameters's default ctor.
 * iii) (optional) Create a GLUI element in glui_utility::create_proj6_panel.
 * iv) Send it to the GPU before the draw call in gui6::draw6.
 */

// class for dealing with GLUI since it writes directly to memory.
class GLUIPresentationLayer {
private:
	map<LICFloatParam, shared_ptr<SliderBundle>> _sliders;
	map<LICFloatParam, LICFloatParam> _dualSliderIDs;
	map<LICBoolParam, shared_ptr<CheckboxBundle>> _checkboxes;
private: //enforces a singleton property.
	GLUIPresentationLayer() {};
	GLUIPresentationLayer(const GLUIPresentationLayer& other);
	GLUIPresentationLayer& operator=(const GLUIPresentationLayer& other);
public:
	void InsertSlider(GLUI* main_glui, GLUI_Panel* panel, LICFloatParam param, 
		const string& label);
	void InsertDualSlider(GLUI* main_glui, GLUI_Panel* panel, 
		LICFloatParam paramOne, LICFloatParam paramTwo, const string& label);
	void InsertCheckBox(GLUI* main_glui, GLUI_Panel* panel, LICBoolParam param, 
		const std::string& label);
	friend void ::CallbackWrapper(int id);
	friend void ::CallbackWrapper2(int id);
private:
	int GetCallbackID(LICFloatParam pone, LICFloatParam ptwo);
	/**
	 * Establishes data binding between the slider and the model.
	 */
	void OnFloatCallback(const int callbackID);
	void OnBooleanCallback(const int callbackID);
public:
	static GLUIPresentationLayer* INSTANCE;
};