#include "item_stack_selector.h"
#include "../base/inventory_database.h"
#include "../base/item_definition.h"

#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ItemStackSelector::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "item_stack", "database"), &ItemStackSelector::setup);
    ClassDB::bind_method(D_METHOD("set_item_stack", "item_stack"), &ItemStackSelector::set_item_stack);
    ClassDB::bind_method(D_METHOD("get_item_stack"), &ItemStackSelector::get_item_stack);
    ClassDB::bind_method(D_METHOD("set_show_quantity", "show_quantity"), &ItemStackSelector::set_show_quantity);
    ClassDB::bind_method(D_METHOD("get_show_quantity"), &ItemStackSelector::get_show_quantity);
    ClassDB::bind_method(D_METHOD("_on_product_id_spin_box_value_changed", "value"), &ItemStackSelector::_on_product_id_spin_box_value_changed);
    ClassDB::bind_method(D_METHOD("_on_product_amount_spin_box_value_changed", "value"), &ItemStackSelector::_on_product_amount_spin_box_value_changed);
    ClassDB::bind_method(D_METHOD("_on_option_button_item_selected", "index"), &ItemStackSelector::_on_option_button_item_selected);

    ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::OBJECT, "item_stack", PROPERTY_HINT_RESOURCE_TYPE, "ItemStack")));
}

ItemStackSelector::ItemStackSelector() {
    resource_id_editor = nullptr;
    option_button = nullptr;
    product_amount_spin_box = nullptr;
    amount_label = nullptr;
    database = nullptr;
    show_quantity = true;
}

ItemStackSelector::~ItemStackSelector() {
}

void ItemStackSelector::_ready() {
    // Create resource ID editor
    resource_id_editor = memnew(ResourceIDEditor);
    resource_id_editor->set_ids_must_exist_in_database(true);
    resource_id_editor->set_resource_type(ResourceIDEditor::RESOURCE_TYPE_ITEM_DEFINITION);
    resource_id_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    resource_id_editor->connect("changed", Callable(this, "_on_product_id_spin_box_value_changed"));
    add_child(resource_id_editor);

    // Create option button
    option_button = memnew(OptionButton);
    option_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    option_button->set_expand_icon(true);
    option_button->connect("item_selected", Callable(this, "_on_option_button_item_selected"));
    add_child(option_button);

    // Create amount label
    amount_label = memnew(Label);
    amount_label->set_text("Amount");
    amount_label->set_custom_minimum_size(Vector2(32, 0));
    add_child(amount_label);

    // Create amount spin box
    product_amount_spin_box = memnew(SpinBox);
    product_amount_spin_box->set_min(1);
    product_amount_spin_box->set_max(100000000);
    product_amount_spin_box->set_value(1);
    product_amount_spin_box->set_use_rounded_values(true);
    product_amount_spin_box->connect("value_changed", Callable(this, "_on_product_amount_spin_box_value_changed"));
    add_child(product_amount_spin_box);
    
    // Apply initial show_quantity setting
    set_show_quantity(show_quantity);
}

void ItemStackSelector::setup(const Ref<ItemStack>& p_item_stack, InventoryDatabase* p_database) {
    item_stack = p_item_stack;
    database = p_database;
    
    if (!item_stack.is_valid() || !database) {
        return;
    }

    // Always clear the lists first to prevent duplication
    ids_list.clear();
    option_button->clear();

    // Setup resource ID editor and amount
    String id = item_stack->get_item_id();
    resource_id_editor->setup(database, id);
    product_amount_spin_box->set_value(item_stack->get_amount());

    // Populate option button with items from database
    TypedArray<ItemDefinition> items = database->get_items();
    int selected_index = -1;
    for (int i = 0; i < items.size(); i++) {
        Ref<ItemDefinition> item = items[i];
        if (item.is_valid()) {
            option_button->add_icon_item(item->get_icon(), item->get_name());
            ids_list.append(item);
            if (item->get_id() == item_stack->get_item_id()) {
                selected_index = i;
            }
        }
    }
    
    // If no valid selection found but items exist, select the first item
    if (selected_index == -1 && ids_list.size() > 0) {
        selected_index = 0;
        // Update the item_stack with the first item's ID
        Ref<ItemDefinition> first_item = ids_list[0];
        if (first_item.is_valid()) {
            item_stack->set_item_id(first_item->get_id());
            resource_id_editor->setup(database, first_item->get_id());
        }
    }
    
    if (selected_index >= 0) {
        option_button->select(selected_index);
    }

    // Set icon max width for popup menu
    PopupMenu* popup = option_button->get_popup();
    if (popup) {
        for (int i = 0; i < items.size(); i++) {
            popup->set_item_icon_max_width(i, 24);
        }
    }
}

void ItemStackSelector::_on_product_id_spin_box_value_changed(const String& value) {
    if (!database || !item_stack.is_valid()) {
        return;
    }

    Ref<ItemDefinition> item = database->get_item(value);
    if (item.is_null()) {
        if (option_button->get_selected() == -1) {
            UtilityFunctions::push_warning("An item no longer exists in the list, a first item has been replaced.");
            option_button->select(0);
        }
        if (option_button->get_selected() >= 0 && option_button->get_selected() < ids_list.size()) {
            Ref<ItemDefinition> selected_item = ids_list[option_button->get_selected()];
            if (selected_item.is_valid()) {
                item = selected_item;
            }
        }
    }
    
    if (item.is_valid()) {
        item_stack->set_item_id(item->get_id());
        
        // Find index and update selection
        for (int i = 0; i < ids_list.size(); i++) {
            Ref<ItemDefinition> list_item = ids_list[i];
            if (list_item.is_valid() && list_item->get_id() == item_stack->get_item_id()) {
                if (option_button->get_selected() != i) {
                    option_button->select(i);
                }
                break;
            }
        }
        emit_signal("changed", item_stack);
    }
}

void ItemStackSelector::_on_product_amount_spin_box_value_changed(double value) {
    if (item_stack.is_valid()) {
        item_stack->set_amount(int(value));
        emit_signal("changed", item_stack);
    }
}

void ItemStackSelector::_on_option_button_item_selected(int index) {
    if (!item_stack.is_valid() || index < 0 || index >= ids_list.size()) {
        return;
    }

    Ref<ItemDefinition> item = ids_list[index];
    if (item.is_valid()) {
        item_stack->set_item_id(item->get_id());
        if (resource_id_editor->get_id() != item->get_id()) {
            resource_id_editor->setup(database, item->get_id());
        }
        emit_signal("changed", item_stack);
    }
}

void ItemStackSelector::set_show_quantity(bool p_show_quantity) {
    show_quantity = p_show_quantity;
    
    if (amount_label) {
        amount_label->set_visible(show_quantity);
    }
    if (product_amount_spin_box) {
        product_amount_spin_box->set_visible(show_quantity);
    }
}