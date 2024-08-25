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
	bool categorized = false;
	TypedArray<ItemCategory> accepted_categories;
	int accepted_categories_code = 0;
	void _update_categories_code();
	bool _is_accept_any_categories(const TypedArray<ItemCategory> &other_list) const;
	void _validate_property(PropertyInfo &p_property) const;

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
	void set_categorized(const bool &new_categorized);
	int is_categorized() const;
	void set_accepted_categories(const TypedArray<ItemCategory> &new_accepted_categories);
	TypedArray<ItemCategory> get_accepted_categories() const;
	int get_item_id() const;
	virtual int add(const Ref<Item> item, const int &amount);
	int remove(const Ref<Item> item, const int &amount);
	int get_max_stack_for_item(Ref<ItemDefinition> item) const;
	bool is_full() const;
	bool is_empty() const;
	bool has_valid() const;
	bool contains(Ref<Item> item, int amount) const;
	bool contains_category(Ref<ItemCategory> category) const;
	bool is_accept_any_categories_of_item(const Ref<ItemDefinition> &other_item) const;
	int left_to_fill();
};

#endif // SLOT_CLASS_H