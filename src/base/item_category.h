#ifndef ITEM_CATEGORY_CLASS_H
#define ITEM_CATEGORY_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class ItemCategory : public Resource {
	GDCLASS(ItemCategory, Resource);

private:
	String id;
	String name;
	Color color;
	Ref<Texture2D> icon;
	int code = 0;
	Dictionary item_properties;
	TypedArray<String> item_dynamic_properties;

protected:
	static void _bind_methods();

public:
	ItemCategory();
	~ItemCategory();
	void set_id(const String &new_id);
	String get_id() const;
	void set_name(const String &new_name);
	String get_name() const;
	void set_color(const Color &new_color);
	Color get_color() const;
	void set_icon(const Ref<Texture2D> &new_icon);
	Ref<Texture2D> get_icon() const;
	void set_code(const int &new_code);
	int get_code() const;
	void set_item_properties(const Dictionary &new_item_properties);
	Dictionary get_item_properties() const;
	void set_item_dynamic_properties(const TypedArray<String> &new_item_dynamic_properties);
	TypedArray<String> get_item_dynamic_properties() const;
};

#endif // ITEM_CATEGORY