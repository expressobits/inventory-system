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
	TypedArray<NodePath> opened_stations;

protected:
	static void _bind_methods();

public:
	Crafter();
	~Crafter();
	void set_main_station(const NodePath &new_main_station);
	NodePath get_main_station() const;
	void set_opened_stations(const TypedArray<NodePath> &new_opened_stations);
	TypedArray<NodePath> get_opened_stations() const;
	bool open(CraftStation *craft_station);
	bool close(CraftStation *craft_station);
	bool is_open_main_craft_station() const;
	bool is_open_any_station() const;
	bool is_open(const CraftStation *craft_station) const;
	bool open_main_craft_station();
	bool close_main_craft_station();
	void close_all_craft_stations();
	CraftStation *get_craft_station(const NodePath craft_station_path) const;
};

#endif // CRAFTER_CLASS_H