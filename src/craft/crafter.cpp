#include "crafter.h"

void Crafter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_main_station"), &Crafter::get_main_station);
	ClassDB::bind_method(D_METHOD("set_main_station", "main_station"), &Crafter::set_main_station);
	ClassDB::bind_method(D_METHOD("get_inventories_for_stations"), &Crafter::get_inventories_for_stations);
	ClassDB::bind_method(D_METHOD("set_inventories_for_stations", "inventories_for_stations"), &Crafter::set_inventories_for_stations);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "main_station", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CraftStation"), "set_main_station", "get_main_station");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "inventories_for_stations", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Inventory")), "set_inventories_for_stations", "get_inventories_for_stations");
}

Crafter::Crafter() {
}

Crafter::~Crafter() {
}

void Crafter::set_main_station(const NodePath &new_main_station) {
	main_station = new_main_station;
}

NodePath Crafter::get_main_station() const {
	return main_station;
}

TypedArray<NodePath> Crafter::get_inventories_for_stations() const {
	return inventories_for_stations;
}

void Crafter::set_inventories_for_stations(const TypedArray<NodePath> &new_inventories_for_stations) {
	inventories_for_stations = new_inventories_for_stations;
}

CraftStation *Crafter::get_craft_station(const NodePath craft_station_path) const {
	Node *node_station = get_node_or_null(craft_station_path);
	CraftStation *craft_station = Object::cast_to<CraftStation>(node_station);
	if (craft_station == nullptr) {
		ERR_PRINT("Get Craft Station is null!");
	}
	return craft_station;
}

Inventory *Crafter::get_input_inventory_for_stations(const NodePath inventory_path) const {
	Node *node_inventory = get_node_or_null(inventory_path);
	Inventory *inventory = Object::cast_to<Inventory>(node_inventory);
	if (inventory == nullptr) {
		ERR_PRINT("Get Input Inventory is null!");
	}
	return inventory;
}
