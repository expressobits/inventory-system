#include "item_stack.h"

void ItemStack::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item_id", "item_id"), &ItemStack::set_item_id);
	ClassDB::bind_method(D_METHOD("get_item_id"), &ItemStack::get_item_id);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &ItemStack::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &ItemStack::get_amount);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_id"), "set_item_id", "get_item_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "amount"), "set_amount", "get_amount");
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

String ItemStack::serialize() const {
	String data = String();
	if (!item_id.is_empty())
		data += item_id;
	else
		data += "NONE";
	data += " ";
	data += String::num_int64(get_amount());
	return data;
}

void ItemStack::deserialize(String data) {
	PackedStringArray array = data.split(" ");
	ERR_FAIL_COND_MSG(array.size() < 2, "Data to deserialize item_stack is invalid: Does not contain the 'amount' field");
	if (!array[0].is_empty() && array[0] != "NONE")
		set_item_id(array[0]);
	if (!array[1].is_empty())
		set_amount(array[1].to_int());
}
