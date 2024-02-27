#ifndef CRAFTING_CLASS_H
#define CRAFTING_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Crafting : public Resource {
	GDCLASS(Crafting, Resource);

private:
	int recipe_index;
	float time;

protected:
	static void _bind_methods();

public:
	void set_recipe_index(const int &new_recipe);
	int get_recipe_index() const;
	void set_time(const float &new_time);
	float get_time() const;
	bool is_finished() const;
	void process(float delta);
	Array to_data() const;
	void from_data(Array data);
};

#endif // CRAFTING_CLASS_H