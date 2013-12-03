#pragma once
#include "vec.h"
#include "glui.h"
#include "model_controller.h"


using Angel::vec2;
using Angel::vec3;

enum class LICFloatParam : int;
enum class LICBoolParam : int;

class SliderBundle : public Observer {
private:
	const vec2 _defVals;
	const vec2 _bounds;
	const bool _twoSided;
	/** glui directly writes to this value. data binding between this and the 
	 * value in 3dlicmodel is established via gluipresentationlayer::OnGLUICallback.
	 */
	vec2 _vals;
	const int _callbackID;
	const string _format;
	GLUI_HSlider* const slider;
	GLUI_StaticText* const _label;
public:
	SliderBundle(GLUI* main_glui, GLUI_Panel* panel, vec2 def_vals,
		vec2 bounds, int id, const string format, void (*callback)(int id),
		bool is_two_sided = false);
	void Refresh();
	void Reset();
	bool IsDual() const;
	vec2 GetValue() const;
	virtual void Update(const Observable* const observable);
private: //non-copyable, non-assignable
	SliderBundle(const SliderBundle& other);
	SliderBundle& operator=(const SliderBundle& other);
};

class CheckboxBundle {
public:
	GLUI_Checkbox* checkbox;
	int val;
	int def_val;
	LICBoolParam id;
	CheckboxBundle(GLUI_Checkbox* checkbox, int val, int def_val, 
		LICBoolParam id ):
		checkbox(checkbox), val(val), def_val(def_val), id(id) {};
	void refresh() {

	}
	void reset() {
		val = def_val;
	}
};

class RadioBundle {
public:
	int _id;
	RadioBundle(){};
	RadioBundle( GLUI* main_glui, GLUI_Panel* panel, int id, char** labels, 
		int num_label, void (*callback)(int));
	void refresh();
	void reset();
	int _counter;
	void (*_callback)(int);
};

void create_proj6_panel(GLUI* main_glui);
void cbox45_factory(GLUI* main_glui, GLUI_Panel* panel, char* name, 
	const CheckboxBundle& cb_in);
