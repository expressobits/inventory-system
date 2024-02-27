#ifndef INVENTORY_ITEM_CLASS_H
#define INVENTORY_ITEM_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include "item_category.h"

using namespace godot;

class ItemDefinition : public Resource {
	GDCLASS(ItemDefinition, Resource);

private:
	int id = 0;
	bool can_stack = true;
	int max_stack = 0;
	String name;
	Ref<Texture2D> icon;
	float weight;
	Dictionary properties;
	TypedArray<String> dynamic_properties;
	TypedArray<ItemCategory> categories;

protected:
	static void _bind_methods();

public:
	enum {
		NONE = -1
	};

	ItemDefinition();
	~ItemDefinition();
	void set_id(const int &new_id);
	int get_id() const;
	void set_can_stack(const bool &new_can_stack);
	bool get_can_stack() const;
	void set_max_stack(const int &new_max_stack);
	int get_max_stack() const;
	void set_name(const String &new_name);
	String get_name() const;
	void set_icon(const Ref<Texture2D> &new_icon);
	Ref<Texture2D> get_icon() const;
	void set_weight(const float &new_weight);
	float get_weight() const;
	void set_properties(const Dictionary &new_properties);
	Dictionary get_properties() const;
	void set_dynamic_properties(const TypedArray<String> &new_dynamic_properties);
	TypedArray<String> get_dynamic_properties() const;
	void set_categories(const TypedArray<ItemCategory> &new_categories);
	TypedArray<ItemCategory> get_categories() const;
	bool contains_category(const Ref<ItemCategory> category) const;
};

#endif