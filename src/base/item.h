#ifndef ITEM_CLASS_H
#define ITEM_CLASS_H

#include "item_definition.h"
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Item : public Resource {
	GDCLASS(Item, Resource);

private:
	Ref<ItemDefinition> definition;
	Dictionary properties;

protected:
	static void _bind_methods();

public:
	Item();
	~Item();
	void set_definition(const Ref<ItemDefinition> new_definition);
	Ref<ItemDefinition> get_definition() const;
	void set_properties(const Dictionary &new_properties);
	Dictionary get_properties() const;
	bool is_stack_with(Ref<Item> other_item) const;
};

#endif // ITEM_CLASS_H