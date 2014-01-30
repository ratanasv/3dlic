#pragma once
#include "vec.h"
#include "model_controller.h"
#include <cstring>

class GLUI_Checkbox;
class GLUI;
class GLUI_Panel;

class CheckboxBundle : public Observer {
public:
	GLUI_Checkbox* _checkbox;
	int _val;
	const int _defVal;
	const int _callbackID;
	std::string _label;
	void refresh();
	void reset();
	virtual void Update(const Observable* const observable);

	class Builder {
	private:
		GLUI* _mainGlui;
		GLUI_Panel* _panel;
		int _defVal;
		int _callbackID;
		std::string _label;
		void (*_callback)(int);
	public:
		Builder& WithMainGLUI(GLUI* main_glui);
		Builder& WithPanel(GLUI_Panel* panel);
		Builder& WithDefaultValue(int val);
		Builder& WithLabel(const std::string& label);
		Builder& WithCallbackID(int id);
		Builder& WithCallback(void (*callback)(int));
		CheckboxBundle Build();
		friend class CheckboxBundle;
	};

	CheckboxBundle(const Builder& b);
private:
	CheckboxBundle(const CheckboxBundle& other);
	CheckboxBundle& operator=(const CheckboxBundle& other);
};