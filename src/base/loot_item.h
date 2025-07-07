#ifndef LOOT_ITEM_CLASS_H
#define LOOT_ITEM_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class LootItem : public Resource {
	GDCLASS(LootItem, Resource);

private:
	String item_id = "";
	float weight = 1.0;
	int min_amount = 1;
	int max_amount = 1;

protected:
	static void _bind_methods();

public:
	LootItem();
	~LootItem();
	void set_item_id(const String &new_item_id);
	String get_item_id() const;
	void set_weight(const float &new_weight);
	float get_weight() const;
	void set_min_amount(const int &new_min_amount);
	int get_min_amount() const;
	void set_max_amount(const int &new_max_amount);
	int get_max_amount() const;
	
	// Serialization methods
	Dictionary serialize() const;
	void deserialize(const Dictionary &data);
};

#endif // LOOT_ITEM_CLASS_H