#ifndef ITEM_CATEGORY_CLASS_H
#define ITEM_CATEGORY_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class ItemCategory : public Resource {
	GDCLASS(ItemCategory, Resource);

private:
	String name;
	Color color;
	Ref<Texture2D> icon;
	int code = 0;

protected:
	static void _bind_methods();

public:
	ItemCategory();
	~ItemCategory();
	void set_name(const String &new_name);
	String get_name() const;
	void set_color(const Color &new_color);
	Color get_color() const;
	void set_icon(const Ref<Texture2D> &new_icon);
	Ref<Texture2D> get_icon() const;
	void set_code(const int &new_code);
	int get_code() const;
};

#endif // ITEM_CATEGORY