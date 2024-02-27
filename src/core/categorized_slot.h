#ifndef CATEGORIZED_SLOT_CLASS_H
#define CATEGORIZED_SLOT_CLASS_H

#include "base/item.h"
#include "base/slot.h"

using namespace godot;

class CategorizedSlot : public Slot {
	GDCLASS(CategorizedSlot, Slot);

private:
	TypedArray<ItemCategory> accepted_categories;
	int accepted_categories_code = 0;
	void _update_categories_code();
	bool _is_accept_any_categories(const TypedArray<ItemCategory> &other_list) const;

protected:
	static void _bind_methods();

public:
	CategorizedSlot();
	~CategorizedSlot();
	void set_accepted_categories(const TypedArray<ItemCategory> &new_accepted_categories);
	TypedArray<ItemCategory> get_accepted_categories() const;
	int add(const Ref<Item> item, const int &amount) override;
	bool is_accept_any_categories_of_item(const Ref<ItemDefinition> &other_item) const;
};

#endif // CATEGORIZED_SLOT_CLASS_H