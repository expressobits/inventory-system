#ifdef TOOLS_ENABLED

#ifndef CUSTOM_PROPERTIES_EDITOR_H
#define CUSTOM_PROPERTIES_EDITOR_H

#include "base/item_category.h"
#include "base/item_definition.h"
#include "base/inventory_database.h"
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include "item_definition_property_editor.h"

using namespace godot;

class CustomPropertiesEditor : public VBoxContainer {
    GDCLASS(CustomPropertiesEditor, VBoxContainer)

public:
    enum ResourceType {
        RESOURCE_TYPE_ITEM_DEFINITION,
        RESOURCE_TYPE_ITEM_CATEGORY
    };

private:
    ResourceType resource_type;
    Ref<Resource> current_resource; // Can be ItemDefinition or ItemCategory
    InventoryDatabase* database;
    TypedArray<ItemDefinitionPropertyEditor> properties_obj;

    // Main UI elements
    VBoxContainer* v_box_container;
    String title_text;
    
    // Properties list section  
    Label* properties_label;
    Button* add_property_button;
    Button* remove_property_button;
    ItemList* properties_list;
    
    // Property details section
    VBoxContainer* property_details_vbox;
    LineEdit* property_name_line_edit;
    OptionButton* property_type_option;
    Control* property_value_container;
    
    // Property value controls (dynamically shown based on type)
    LineEdit* string_value_edit;
    SpinBox* int_value_spinbox;
    SpinBox* float_value_spinbox;
    CheckBox* bool_value_checkbox;
    ColorPickerButton* color_value_picker;
    EditorResourcePicker* resource_value_picker;
    CheckBox* dynamic_property_checkbox;
    
    // State
    String selected_property_name;

protected:
    static void _bind_methods();

public:
    CustomPropertiesEditor();
    ~CustomPropertiesEditor();

    void _ready() override;
    void set_resource_type(ResourceType p_type);
    void set_title(const String& p_title);
    void load_resource(InventoryDatabase* p_database, const Ref<Resource>& p_resource);

private:
    Dictionary get_properties_from_resource();
    void set_properties_to_resource(const Dictionary& properties);
    TypedArray<String> get_dynamic_properties_from_resource();
    void set_dynamic_properties_to_resource(const TypedArray<String>& dynamic_properties);
    
    Dictionary make_dictionary_unique(const Dictionary& input_dictionary);
    void build_type_options();
    void _update_properties_list();
    void _update_property_details();
    void _show_property_value_control(int type);
    void _create_value_controls();
    void _apply_theme();
    
    // Signal handlers
    void _on_add_property_button_pressed();
    void _on_remove_property_button_pressed();
    void _on_properties_list_item_selected(int index);
    void _on_property_name_text_changed(const String& text);
    void _on_property_type_item_selected(int index);
    void _on_string_value_text_changed(const String& text);
    void _on_int_value_changed(double value);
    void _on_float_value_changed(double value);
    void _on_bool_value_toggled(bool pressed);
    void _on_color_value_changed(const Color& color);
    void _on_resource_value_changed(const Ref<Resource>& resource);
    void _on_dynamic_property_toggled(bool pressed);
};

VARIANT_ENUM_CAST(CustomPropertiesEditor::ResourceType);

#endif // CUSTOM_PROPERTIES_EDITOR_H

#endif // TOOLS_ENABLED