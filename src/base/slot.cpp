#include "slot.h"

#include "item_definition.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>

void Slot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_id", "item_id"), &Slot::set_item_id);
	ClassDB::bind_method(D_METHOD("get_item_id"), &Slot::get_item_id);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &Slot::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &Slot::get_amount);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &Slot::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &Slot::get_properties);
	ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &Slot::set_max_stack);
	ClassDB::bind_method(D_METHOD("get_max_stack"), &Slot::get_max_stack);
	ClassDB::bind_method(D_METHOD("set_categorized", "categorized"), &Slot::set_categorized);
	ClassDB::bind_method(D_METHOD("is_categorized"), &Slot::is_categorized);
	ClassDB::bind_method(D_METHOD("set_accepted_categories", "accepted_categories"), &Slot::set_accepted_categories);
	ClassDB::bind_method(D_METHOD("get_accepted_categories"), &Slot::get_accepted_categories);
	ClassDB::bind_method(D_METHOD("is_full"), &Slot::is_full);
	ClassDB::bind_method(D_METHOD("is_empty"), &Slot::is_empty);
	ClassDB::bind_method(D_METHOD("has_valid"), &Slot::has_valid);
	ClassDB::bind_method(D_METHOD("contains", "item", "amount"), &Slot::contains, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("can_stack", "item_id", "properties"), &Slot::can_stack);

	ADD_SIGNAL(MethodInfo("updated"));

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_id"), "set_item_id", "get_item_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount"), "set_amount", "get_amount");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_stack"), "set_max_stack", "get_max_stack");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "categorized"), "set_categorized", "is_categorized");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "accepted_categories", PROPERTY_HINT_ARRAY_TYPE, "String"), "set_accepted_categories", "get_accepted_categories");
}

void Slot::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == StringName("accepted_categories")) {
		if (!categorized) {
			p_property.usage = PROPERTY_USAGE_NO_EDITOR;
		}
	}
}

Slot::Slot() {
}

Slot::~Slot() {
}

void Slot::set_item_id(const String &new_item_id) {
	item_id = new_item_id;
	emit_signal("updated");
}

String Slot::get_item_id() const {
	return item_id;
}

void Slot::set_amount(const int &new_amount) {
	amount = new_amount;
	emit_signal("updated");
}

int Slot::get_amount() const {
	return amount;
}

void Slot::set_properties(const Dictionary &new_properties) {
	properties = new_properties;
	emit_signal("updated");
}

Dictionary Slot::get_properties() const {
	return properties;
}

void Slot::set_max_stack(const int &new_max_stack) {
	max_stack = new_max_stack;
}

int Slot::get_max_stack() const {
	return max_stack;
}

void Slot::set_categorized(const bool &new_categorized) {
	categorized = new_categorized;
	notify_property_list_changed();
}

int Slot::is_categorized() const {
	return categorized;
}

void Slot::set_accepted_categories(const TypedArray<String> &new_accepted_categories) {
	accepted_categories = new_accepted_categories;
}

void Slot::set_accepted_categories_code(const int new_code) {
	accepted_categories_code = new_code;
}

int Slot::get_accepted_categories_code() const {
	return accepted_categories_code;
}

TypedArray<String> Slot::get_accepted_categories() const {
	return accepted_categories;
}

bool Slot::is_full() const {
	if (has_valid()) {
		return amount >= get_max_stack();
	}
	return false;
}

bool Slot::is_empty() const {
	return amount <= 0;
}

bool Slot::has_valid() const {
	return item_id != "" && amount > 0;
}

bool Slot::contains(const String &item_id, const int amount) const {
	if (this->item_id != item_id) {
		return false;
	} else {
		return this->amount >= amount;
	}
}

int Slot::left_to_fill() const {
	if (has_valid()) {
		return get_max_stack() - amount;
	}
	return -1;
}

bool Slot::can_stack(const String &item_id, const Dictionary &properties) const {
	if (get_item_id() == "" || get_item_id() == item_id) {
		return get_properties().is_empty() && properties.is_empty();
	}
	return false;
}