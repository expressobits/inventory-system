#ifndef ITEM_STACK_CLASS_H
#define ITEM_STACK_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ItemStack : public Resource {
	GDCLASS(ItemStack, Resource);

private:
    String item_id = "";
    int amount = 0;
	Dictionary properties;

protected:
	static void _bind_methods();

public:
	ItemStack();
	~ItemStack();
    void set_item_id(const String &new_item_id);
	String get_item_id() const;
	void set_amount(const int &new_amount);
	int get_amount() const;
	void set_properties(const Dictionary &new_properties);
	Dictionary get_properties() const;
    String serialize() const;
    void deserialize(String data);

	static String serialize_properties(const Dictionary properties);
	static Dictionary deserialize_properties(const String properties_data);
};

#endif // ITEM_STACK_CLASS_H