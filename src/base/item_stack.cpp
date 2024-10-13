#include "item_stack.h"

void ItemStack::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_id", "item_id"), &ItemStack::set_item_id);
	ClassDB::bind_method(D_METHOD("get_item_id"), &ItemStack::get_item_id);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &ItemStack::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &ItemStack::get_amount);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &ItemStack::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &ItemStack::get_properties);

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
}

String ItemStack::get_item_id() const {
	return item_id;
}

void ItemStack::set_amount(const int &new_amount) {
	amount = new_amount;
}

int ItemStack::get_amount() const {
	return amount;
}

void ItemStack::set_properties(const Dictionary &new_properties) {
	properties = new_properties;
}

Dictionary ItemStack::get_properties() const {
	return properties;
}

String ItemStack::serialize() const {
	String data = String();
	if (!item_id.is_empty())
		data += item_id;
	else
		data += "";
	data += " ";
	data += String::num_int64(get_amount());
	data += " ";
	data += serialize_properties(properties);
	return data;
}

void ItemStack::deserialize(String data) {
	PackedStringArray array = data.split(" ");
	ERR_FAIL_COND_MSG(array.size() < 2, "Data to deserialize item_stack is invalid: Does not contain the 'amount' field");
	if (!array[0].is_empty())
		set_item_id(array[0]);
	if (!array[1].is_empty())
		set_amount(array[1].to_int());
	String properties_data = String();
	for (size_t i = 2; i < array.size(); i++) {
		properties_data += array[i];
		if (i < array.size() - 1)
			properties_data += array[i];
	}
	set_properties(deserialize_properties(properties_data));
}

String ItemStack::serialize_properties(const Dictionary properties) {
	String properties_data = String();
	for (size_t i = 0; i < properties.keys().size(); i++) {
		properties_data += properties.keys()[i];
		properties_data += ":";
		properties_data += properties[properties.keys()[i]];
		if (i < properties.keys().size() - 1)
			properties_data += " ";
	}
	return properties_data;
}

Dictionary ItemStack::deserialize_properties(const String data) {
	Dictionary properties = Dictionary();
	PackedStringArray array = data.split(" ");
	for (size_t i = 0; i < array.size(); i++) {
		PackedStringArray key_value = array[i].split(":");
		if (key_value.size() != 2)
			continue;
		properties[key_value[0]] = key_value[1];
	}
	return properties;
}
