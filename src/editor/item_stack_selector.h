#ifndef ITEM_STACK_SELECTOR_H
#define ITEM_STACK_SELECTOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

#include "resource_id_editor.h"
#include "../base/item_stack.h"
#include "../base/item_definition.h"
#include "../base/inventory_database.h"

using namespace godot;

class ItemStackSelector : public HBoxContainer {
    GDCLASS(ItemStackSelector, HBoxContainer)

private:
    ResourceIDEditor* resource_id_editor;
    OptionButton* option_button;
    SpinBox* product_amount_spin_box;
    
    Ref<ItemStack> item_stack;
    InventoryDatabase* database;
    TypedArray<ItemDefinition> ids_list;

protected:
    static void _bind_methods();

public:
    ItemStackSelector();
    ~ItemStackSelector();

    void _ready() override;
    void setup(const Ref<ItemStack>& p_item_stack, InventoryDatabase* p_database);
    
    // Getters/Setters
    void set_item_stack(const Ref<ItemStack>& p_item_stack) { item_stack = p_item_stack; }
    Ref<ItemStack> get_item_stack() const { return item_stack; }

private:
    void _on_product_id_spin_box_value_changed(const String& value);
    void _on_product_amount_spin_box_value_changed(double value);
    void _on_option_button_item_selected(int index);
};

#endif // ITEM_STACK_SELECTOR_H