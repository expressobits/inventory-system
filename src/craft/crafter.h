#ifndef CRAFTER_CLASS_H
#define CRAFTER_CLASS_H

#include "base/inventory_database.h"
#include "base/node_inventories.h"
#include "craft_station.h"
#include <godot_cpp/classes/node.hpp>


using namespace godot;

class Crafter : public NodeInventories {
	GDCLASS(Crafter, NodeInventories);

private:
	NodePath main_station;
	TypedArray<NodePath> inventories_for_stations;

protected:
	static void _bind_methods();

public:
	Crafter();
	~Crafter();
	void set_main_station(const NodePath &new_main_station);
	TypedArray<NodePath> get_inventories_for_stations() const;
	void set_inventories_for_stations(const TypedArray<NodePath> &new_inventories_for_stations);
	NodePath get_main_station() const;
	CraftStation *get_craft_station(const NodePath craft_station_path) const;
	Inventory *get_input_inventory_for_stations(const NodePath inventory_path) const;
};

#endif // CRAFTER_CLASS_H