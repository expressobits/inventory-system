#ifdef TOOLS_ENABLED

#ifndef CUSTOM_PROPERTIES_EDITOR_H
#define CUSTOM_PROPERTIES_EDITOR_H

#include "base/item_category.h"
#include "base/item_definition.h"
#include "base/inventory_database.h"
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
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

    VBoxContainer* v_box_container;
    Button* add_button;
    LineEdit* line_edit;
    OptionButton* new_type_option_button;
    String title_text;

protected:
    static void _bind_methods();

public:
    CustomPropertiesEditor();
    ~CustomPropertiesEditor();

    void _ready() override;
    void set_resource_type(ResourceType p_type);
    void set_title(const String& p_title);
    void load_resource(InventoryDatabase* p_database, const Ref<Resource>& p_resource);
    void loading_properties();

private:
    Dictionary get_properties_from_resource();
    void set_properties_to_resource(const Dictionary& properties);
    TypedArray<String> get_dynamic_properties_from_resource();
    void set_dynamic_properties_to_resource(const TypedArray<String>& dynamic_properties);
    
    Dictionary make_dictionary_unique(const Dictionary& input_dictionary);
    void build_type_options();
    void _property_removed(const String& key, const Ref<Resource>& resource);
    void _property_changed(const String& key, bool is_dynamic, const Variant& value, const Ref<Resource>& resource);
    void _on_add_button_pressed();
    void _on_new_key_line_edit_text_changed(const String& new_text);
};

VARIANT_ENUM_CAST(CustomPropertiesEditor::ResourceType);

#endif // CUSTOM_PROPERTIES_EDITOR_H

#endif // TOOLS_ENABLED