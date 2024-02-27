#include "item.h"

void Item::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_definition", "definition"), &Item::set_definition);
	ClassDB::bind_method(D_METHOD("get_definition"), &Item::get_definition);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &Item::set_properties);
	ClassDB::bind_method(D_METHOD("get_properties"), &Item::get_properties);
	ClassDB::bind_method(D_METHOD("is_stack_with", "other_item"), &Item::is_stack_with);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "definition", PROPERTY_HINT_RESOURCE_TYPE, "ItemDefinition"), "set_definition", "get_definition");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
}

Item::Item() {
}

Item::~Item() {
}

void Item::set_definition(const Ref<ItemDefinition> new_definition) {
	definition = new_definition;
}

Ref<ItemDefinition> Item::get_definition() const {
	return definition;
}

void Item::set_properties(const Dictionary &new_properties) {
	properties = new_properties;
}

Dictionary Item::get_properties() const {
	return properties;
}

bool Item::is_stack_with(Ref<Item> other_item) const {
	if (other_item->get_definition() == definition) {
		return properties.is_empty() && other_item->get_properties().is_empty();
	}
	return false;
}
