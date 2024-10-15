#ifndef SLOT_CLASS_H
#define SLOT_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Slot : public Resource {
	GDCLASS(Slot, Resource);

private:
	String item_id = "";
	int amount = 0;
	Dictionary properties;
	int max_stack = -1;
	bool categorized = false;
	TypedArray<String> accepted_categories;
	int accepted_categories_code = 0;
	void _validate_property(PropertyInfo &p_property) const;

protected:
	static void _bind_methods();

public:
	Slot();
	~Slot();
	void set_item_id(const String &new_item_id);
	String get_item_id() const;
	void set_amount(const int &new_amount);
	int get_amount() const;
	void set_properties(const Dictionary &new_properties);
	Dictionary get_properties() const;
	void set_max_stack(const int &new_max_stack);
	int get_max_stack() const;
	void set_categorized(const bool &new_categorized);
	int is_categorized() const;
	void set_accepted_categories(const TypedArray<String> &new_accepted_categories);
	void set_accepted_categories_code(int new_code);
	int get_accepted_categories_code() const;
	TypedArray<String> get_accepted_categories() const;
	bool is_full() const;
	bool is_empty() const;
	bool has_valid() const;
	bool contains(String item_id, int amount) const;
	int left_to_fill() const;
	bool can_stack(const String &item_id, const Dictionary &properties) const;
};

#endif // SLOT_CLASS_H