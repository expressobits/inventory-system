#include "interact_action.h"

void InteractAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_input", "input"), &InteractAction::set_input);
	ClassDB::bind_method(D_METHOD("get_input"), &InteractAction::get_input);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &InteractAction::set_description);
	ClassDB::bind_method(D_METHOD("get_description"), &InteractAction::get_description);
	ClassDB::bind_method(D_METHOD("set_code", "code"), &InteractAction::set_code);
	ClassDB::bind_method(D_METHOD("get_code"), &InteractAction::get_code);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "input"), "set_input", "get_input");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "code"), "set_code", "get_code");
}

InteractAction::InteractAction() {
}

InteractAction::~InteractAction() {
}

void InteractAction::set_input(const String &new_input) {
	input = new_input;
}

String InteractAction::get_input() const {
	return input;
}

void InteractAction::set_description(const String &new_description) {
	description = new_description;
}

String InteractAction::get_description() const {
	return description;
}

void InteractAction::set_code(const int &new_code) {
	code = new_code;
}

int InteractAction::get_code() const {
	return code;
}
