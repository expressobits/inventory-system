#ifndef ITEM_STACK_CLASS_H
#define ITEM_STACK_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ItemStack : public Resource {
	GDCLASS(ItemStack, Resource);

private:
    String item_id = "";
    int amount = 0;

protected:
	static void _bind_methods();

public:
	ItemStack();
	~ItemStack();
    void set_item_id(const String &new_item_id);
	String get_item_id() const;
	void set_amount(const int &new_amount);
	int get_amount() const;
    String serialize() const;
    void deserialize(String data);
};

#endif // ITEM_STACK_CLASS_H