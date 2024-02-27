#ifndef CRAFT_STATION_TYPE_CLASS_H
#define CRAFT_STATION_TYPE_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class CraftStationType : public Resource {
	GDCLASS(CraftStationType, Resource);

private:
	String name;
	Ref<Texture2D> icon;

protected:
	static void _bind_methods();

public:
	CraftStationType();
	~CraftStationType();
	void set_name(const String &new_name);
	String get_name() const;
	void set_icon(const Ref<Texture2D> &new_icon);
	Ref<Texture2D> get_icon() const;
};

#endif // CRAFT_STATION_TYPE_CLASS_H