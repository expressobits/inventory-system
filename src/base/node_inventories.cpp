#include "node_inventories.h"

void NodeInventories::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_database", "database"), &NodeInventories::set_database);
	ClassDB::bind_method(D_METHOD("get_database"), &NodeInventories::get_database);
	ClassDB::bind_method(D_METHOD("get_item_from_id", "id"), &NodeInventories::get_item_from_id);
	ClassDB::bind_method(D_METHOD("get_id_from_item", "item"), &NodeInventories::get_id_from_item);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "database", PROPERTY_HINT_RESOURCE_TYPE, "InventoryDatabase"), "set_database", "get_database");
}

void NodeInventories::_ready() {
	if (database == nullptr)
		return;
	for (size_t i = 0; i < database->get_items().size(); i++) {
		Ref<ItemDefinition> item = database->get_items()[i];
		items_cache[item->get_id()] = item;
	}
}

NodeInventories::NodeInventories() {
}

NodeInventories::~NodeInventories() {
}

PackedStringArray NodeInventories::_get_configuration_warnings() const {
	PackedStringArray string_array;
	if (database == nullptr) {
		string_array.append("Database is null!");
	}
	return string_array;
}

void NodeInventories::set_database(const Ref<InventoryDatabase> new_database) {
	database = new_database;
}

Ref<InventoryDatabase> NodeInventories::get_database() const {
	return database;
}

Ref<ItemDefinition> NodeInventories::get_item_from_id(const int id) const {
	return database->get_item(id);
}

int NodeInventories::get_id_from_item(const Ref<ItemDefinition> item) const {
	return item->get_id();
}
