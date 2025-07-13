#ifndef INVENTORY_ITEM_CLASS_H
#define INVENTORY_ITEM_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include "item_category.h"

using namespace godot;

class ItemDefinition : public Resource {
	GDCLASS(ItemDefinition, Resource);

private:
	String id = "";
	bool can_stack = true;
	int max_stack = 0;
	String name = "";
	Ref<Texture2D> icon;
	float weight = 0.0;
	Vector2i size = Vector2i(1, 1);
	String description = ""; // <-- added
	Dictionary properties;
	TypedArray<String> dynamic_properties;
	TypedArray<ItemCategory> categories;
	void _check_invalid_dynamic_properties();

protected:
	static void _bind_methods();

public:
	ItemDefinition();
	~ItemDefinition();
	void set_id(const String &new_id);
	String get_id() const;
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
	void set_size(const Vector2i &new_size);
	Vector2i get_size() const;
	void set_properties(const Dictionary &new_properties);
	Dictionary get_properties() const;
	void set_dynamic_properties(const TypedArray<String> &new_dynamic_properties);
	TypedArray<String> get_dynamic_properties() const;
	void set_categories(const TypedArray<ItemCategory> &new_categories);
	TypedArray<ItemCategory> get_categories() const;
	bool is_in_category(const Ref<ItemCategory> category) const;
	Vector2i get_rotated_size() const;
	void set_description(const String &new_description);
	String get_description() const;
};

#endif