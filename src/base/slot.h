#ifndef SLOT_CLASS_H
#define SLOT_CLASS_H

#include "item.h"
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Slot : public Resource {
	GDCLASS(Slot, Resource);

private:
	Ref<Item> item;
	int amount = 0;
	int max_stack = -1;

protected:
	static void _bind_methods();

public:
	Slot();
	~Slot();
	void set_item(const Ref<Item> &new_item);
	Ref<Item> get_item() const;
	void set_amount(const int &new_amount);
	int get_amount() const;
	void set_max_stack(const int &new_max_stack);
	int get_max_stack() const;
	int get_item_id() const;
	virtual int add(const Ref<Item> item, const int &amount);
	int remove(const Ref<Item> item, const int &amount);
	int get_max_stack_for_item(Ref<ItemDefinition> item) const;
	bool is_full() const;
	bool is_empty() const;
	bool has_valid() const;
	bool contains(Ref<Item> item, int amount) const;
	bool contains_category(Ref<ItemCategory> category) const;
};

#endif // SLOT_CLASS_H