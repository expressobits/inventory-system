#ifndef HOTBAR_CLASS_H
#define HOTBAR_CLASS_H

#include "base/node_inventories.h"
#include "inventory.h"

using namespace godot;

class Hotbar : public NodeInventories {
	GDCLASS(Hotbar, NodeInventories);

public:
	enum AutoEquipMode {
		AUTO_EQUIP_DISABLED = 0,
		AUTO_EQUIP_BY_STACK_ORDER = 1,
		AUTO_EQUIP_BY_GRID_POSITION = 2
	};

	class Slot : public RefCounted {
        GDCLASS(Slot, RefCounted);
    private:
		Ref<ItemStack> stack = nullptr;
		bool active = false;

	protected:
		static void _bind_methods();

	public:
		void clear();
		void set_stack(const Ref<ItemStack> &new_stack);
		Ref<ItemStack> get_stack() const;
		void set_active(const bool new_active);
		bool is_active() const;
	};

private:
	NodePath inventory_path;
	int max_slots = 4;
	int selection_index = -1;
	AutoEquipMode auto_equip_mode = AUTO_EQUIP_DISABLED;
	TypedArray<Slot> slots;
	void _on_contents_changed();
	void _perform_auto_equip();

protected:
	static void _bind_methods();

public:
	Hotbar();
	~Hotbar();
	virtual void _ready() override;
	void set_inventory_path(const NodePath &new_inventory);
	NodePath get_inventory_path() const;
	Inventory *get_inventory() const;
	void set_max_slots(const int &new_slots_count);
	int get_max_slots() const;
	void set_selection_index(const int &new_selection_index);
	int get_selection_index() const;
	void set_auto_equip_mode(AutoEquipMode new_auto_equip_mode);
	AutoEquipMode get_auto_equip_mode() const;
	void set_slots(const TypedArray<Hotbar::Slot> new_slots);
	TypedArray<Hotbar::Slot> get_slots() const;

	void active_slot(const int slot_index);
	void deactive_slot(const int slot_index);

	bool is_active_slot(int slot_index) const;
	void equip(Ref<ItemStack> stack, int slot_index = 0);
	void unequip(const int slot_index);
	void next_item();
	void previous_item();
	bool has_valid_stack_on_selection() const;
	bool has_valid_stack_on_slot(const int slot_index) const;
	Ref<ItemStack> get_stack_on_slot(const int slot_index) const;
	Ref<ItemStack> get_stack_on_selection() const;
};

VARIANT_ENUM_CAST(Hotbar::AutoEquipMode);

#endif // HOTBAR_CLASS_H