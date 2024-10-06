#include "slot.h"

#include "item_definition.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>

void Slot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &Slot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &Slot::get_item);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &Slot::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &Slot::get_amount);
	ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &Slot::set_max_stack);
	ClassDB::bind_method(D_METHOD("get_max_stack"), &Slot::get_max_stack);
	ClassDB::bind_method(D_METHOD("set_categorized", "categorized"), &Slot::set_categorized);
	ClassDB::bind_method(D_METHOD("is_categorized"), &Slot::is_categorized);
	ClassDB::bind_method(D_METHOD("set_accepted_categories", "accepted_categories"), &Slot::set_accepted_categories);
	ClassDB::bind_method(D_METHOD("get_accepted_categories"), &Slot::get_accepted_categories);
	ClassDB::bind_method(D_METHOD("get_item_id"), &Slot::get_item_id);
	ClassDB::bind_method(D_METHOD("get_max_stack_for_item", "item"), &Slot::get_max_stack_for_item);
	ClassDB::bind_method(D_METHOD("is_full"), &Slot::is_full);
	ClassDB::bind_method(D_METHOD("is_empty"), &Slot::is_empty);
	ClassDB::bind_method(D_METHOD("has_valid"), &Slot::has_valid);
	ClassDB::bind_method(D_METHOD("contains", "item", "amount"), &Slot::contains, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("contains_category", "category"), &Slot::contains_category);

	ADD_SIGNAL(MethodInfo("updated"));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "Item"), "set_item", "get_item");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount"), "set_amount", "get_amount");
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

void Slot::set_item(const Ref<Item> &new_item) {
	item = new_item;
	emit_signal("updated");
}

Ref<Item> Slot::get_item() const {
	return item;
}

void Slot::set_amount(const int &new_amount) {
	amount = new_amount;
	emit_signal("updated");
}

int Slot::get_amount() const {
	return amount;
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

void Slot::set_accepted_categories_code(int new_code) {
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
	return item != nullptr && amount > 0 && item->get_definition() != nullptr;
}

bool Slot::contains(Ref<Item> item, int amount) const {
	if (this->item->get_definition() != item->get_definition()) {
		return false;
	} else {
		return this->amount >= amount;
	}
}

bool Slot::contains_category(Ref<ItemCategory> category) const {
	if (this->item == nullptr || this->item->get_definition() == nullptr) {
		return false;
	} else {
		return this->item->get_definition()->is_in_category(category);
	}
}

int Slot::left_to_fill() {
	if (has_valid()) {
		return get_max_stack() - amount;
	}
	return -1;
}

String Slot::get_item_id() const {
	if (this->item == nullptr || this->item->get_definition() == nullptr) {
		return "";
	} else {
		return this->item->get_definition()->get_id();
	}
}

int Slot::get_max_stack_for_item(Ref<ItemDefinition> item) const {
	if (max_stack == -1 && item != nullptr) {
		return item->get_max_stack();
	} else {
		return max_stack;
	}
}
