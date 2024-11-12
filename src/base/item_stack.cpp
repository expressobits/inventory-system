#include "item_stack.h"
#include <godot_cpp/classes/json.hpp>

void ItemStack::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_id", "item_id"), &ItemStack::set_item_id);
	ClassDB::bind_method(D_METHOD("get_item_id"), &ItemStack::get_item_id);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &ItemStack::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &ItemStack::get_amount);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &ItemStack::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &ItemStack::get_properties);
	ClassDB::bind_method(D_METHOD("contains", "item", "amount"), &ItemStack::contains, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("has_valid"), &ItemStack::has_valid);
	ClassDB::bind_method(D_METHOD("serialize"), &ItemStack::serialize);
	ClassDB::bind_method(D_METHOD("deserialize", "data"), &ItemStack::deserialize);

	ADD_SIGNAL(MethodInfo("updated"));

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_id"), "set_item_id", "get_item_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount"), "set_amount", "get_amount");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
}

ItemStack::ItemStack() {
}

ItemStack::~ItemStack() {
}

void ItemStack::set_item_id(const String &new_item_id) {
	item_id = new_item_id;
	emit_signal("updated");
}

String ItemStack::get_item_id() const {
	return item_id;
}

void ItemStack::set_amount(const int &new_amount) {
	amount = new_amount;
	emit_signal("updated");
}

int ItemStack::get_amount() const {
	return amount;
}

void ItemStack::set_properties(const Dictionary &new_properties) {
	properties = new_properties;
	emit_signal("updated");
}

Dictionary ItemStack::get_properties() const {
	return properties;
}

Array ItemStack::serialize() const {
	Array data = Array();
	data.append(item_id);
	data.append(amount);
	if (!properties.is_empty()) {
		data.append(properties);
	}
	return data;
}

void ItemStack::deserialize(Array data) {
	ERR_FAIL_COND_MSG(data.size() < 2, "Data to deserialize item_stack is invalid: Does not contain the 'amount' field");
	set_item_id(data[0]);
	set_amount(data[1]);
	if (data.size() > 2) {
		set_properties(data[2]);
	}
}

bool ItemStack::contains(const String &item_id, const int amount) const {
	if (this->item_id != item_id) {
		return false;
	} else {
		return this->amount >= amount;
	}
}

bool ItemStack::has_valid() const {
	return item_id != "" && amount > 0;
}

String ItemStack::serialize_properties(const Dictionary properties) {
	return JSON::stringify(properties);
}

Dictionary ItemStack::deserialize_properties(const String data) {
	return JSON::parse_string(data);
}
