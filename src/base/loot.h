#ifndef LOOT_CLASS_H
#define LOOT_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include "loot_item.h"

using namespace godot;

class Loot : public Resource {
	GDCLASS(Loot, Resource);

private:
	TypedArray<LootItem> items;
	String id = "";
	String name = "";
	int min_rolls = 1;
	int max_rolls = 1;

protected:
	static void _bind_methods();

public:
	Loot();
	~Loot();
	void set_items(const TypedArray<LootItem> &new_items);
	TypedArray<LootItem> get_items() const;
	void set_id(const String &new_id);
	String get_id() const;
	void set_name(const String &new_name);
	String get_name() const;
	void set_min_rolls(const int &new_min_rolls);
	int get_min_rolls() const;
	void set_max_rolls(const int &new_max_rolls);
	int get_max_rolls() const;
	
	// Helper methods for loot generation
	float get_total_weight() const;
	TypedArray<LootItem> get_random_items(int rolls = -1) const;
	
	// Serialization methods
	Dictionary serialize() const;
	void deserialize(const Dictionary &data);
};

#endif // LOOT_CLASS_H