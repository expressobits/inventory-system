#include "slot.h"

#include "item_definition.h"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>

void Slot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &Slot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &Slot::get_item);
	ClassDB::bind_method(D_METHOD("set_amount", "amount"), &Slot::set_amount);
	ClassDB::bind_method(D_METHOD("get_amount"), &Slot::get_amount);
	ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &Slot::set_max_stack);
	ClassDB::bind_method(D_METHOD("get_max_stack"), &Slot::get_max_stack);
	ClassDB::bind_method(D_METHOD("get_item_id"), &Slot::get_item_id);
	ClassDB::bind_method(D_METHOD("add", "item", "amount"), &Slot::add, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("remove", "item", "amount"), &Slot::remove, DEFVAL(1));
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
}

Slot::Slot() {
}

Slot::~Slot() {
}

void Slot::set_item(const Ref<Item> &new_item) {
	item = new_item;
}

Ref<Item> Slot::get_item() const {
	return item;
}

void Slot::set_amount(const int &new_amount) {
	amount = new_amount;
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
		return this->item->get_definition()->contains_category(category);
	}
}

int Slot::get_item_id() const {
	if (this->item == nullptr || this->item->get_definition() == nullptr) {
		return ItemDefinition::NONE;
	} else {
		return this->item->get_definition()->get_id();
	}
}

int Slot::add(const Ref<Item> item, const int &amount) {
	if (this->item == nullptr) {
		return amount;
	}
	if (amount <= 0 || (has_valid() && this->item->get_definition() != item->get_definition())) {
		return amount;
	}
	int max_stack = get_max_stack_for_item(item->get_definition());
	int amount_to_add = MIN(amount, max_stack - this->amount);
	this->amount = this->amount + amount_to_add;
	if (amount_to_add > 0 && this->item->get_definition() == nullptr) {
		this->item->set_definition(item->get_definition());
		this->item->set_properties(item->get_properties());
		emit_signal("updated");
	}
	return amount - amount_to_add;
}

int Slot::remove(const Ref<Item> item, const int &amount) {
	if (this->item == nullptr) {
		return amount;
	}
	if (amount <= 0 || (has_valid() && this->item->get_definition() != item->get_definition())) {
		return amount;
	}
	int amount_to_remove = MIN(amount, this->amount);
	this->amount = this->amount - amount_to_remove;
	emit_signal("updated");
	if (this->amount <= 0) {
		this->item->set_definition(nullptr);
	}
	return amount - amount_to_remove;
}

int Slot::get_max_stack_for_item(Ref<ItemDefinition> item) const {
	if (max_stack == -1 && item != nullptr) {
		return item->get_max_stack();
	} else {
		return max_stack;
	}
}
