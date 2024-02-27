#include "categorized_slot.h"
#include <godot_cpp/classes/engine.hpp>

void CategorizedSlot::_update_categories_code() {
	accepted_categories_code = 0;
	if (!Engine::get_singleton()->is_editor_hint()) {
		for (size_t i = 0; i < accepted_categories.size(); i++) {
			Ref<ItemCategory> c = accepted_categories[i];
			if (c == nullptr)
				continue;
			accepted_categories_code |= c->get_code();
		}
	}
}

bool CategorizedSlot::_is_accept_any_categories(const TypedArray<ItemCategory> &other_list) const {
	for (size_t i = 0; i < other_list.size(); i++) {
		Ref<ItemCategory> c = other_list[i];
		if (c == nullptr)
			continue;
		if ((accepted_categories_code & c->get_code()) > 0) {
			return true;
		}
	}
	return false;
}

void CategorizedSlot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_accepted_categories", "accepted_categories"), &CategorizedSlot::set_accepted_categories);
	ClassDB::bind_method(D_METHOD("get_accepted_categories"), &CategorizedSlot::get_accepted_categories);
	ClassDB::bind_method(D_METHOD("add", "item", "amount"), &CategorizedSlot::add, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("is_accept_any_categories_of_item", "item"), &CategorizedSlot::is_accept_any_categories_of_item);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "accepted_categories", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "ItemCategory")), "set_accepted_categories", "get_accepted_categories");
}

CategorizedSlot::CategorizedSlot() {
}

CategorizedSlot::~CategorizedSlot() {
}

void CategorizedSlot::set_accepted_categories(const TypedArray<ItemCategory> &new_accepted_categories) {
	accepted_categories = new_accepted_categories;
	_update_categories_code();
}

TypedArray<ItemCategory> CategorizedSlot::get_accepted_categories() const {
	return accepted_categories;
}

int CategorizedSlot::add(const Ref<Item> item, const int &amount) {
	_update_categories_code();
	if (!is_accept_any_categories_of_item(item->get_definition())) {
		return amount;
	}
	return Slot::add(item, amount);
}

bool CategorizedSlot::is_accept_any_categories_of_item(const Ref<ItemDefinition> &other_item) const {
	return accepted_categories_code == 0 || _is_accept_any_categories(other_item->get_categories());
}
