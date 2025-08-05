#ifdef TOOLS_ENABLED

#include "custom_properties_editor.h"
#include "../base/inventory_database.h"
#include "../base/item_definition.h"
#include "../base/item_category.h"
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void CustomPropertiesEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_resource_type", "type"), &CustomPropertiesEditor::set_resource_type);
    ClassDB::bind_method(D_METHOD("set_title", "title"), &CustomPropertiesEditor::set_title);
    ClassDB::bind_method(D_METHOD("load_resource", "database", "resource"), &CustomPropertiesEditor::load_resource);
    ClassDB::bind_method(D_METHOD("loading_properties"), &CustomPropertiesEditor::loading_properties);
    ClassDB::bind_method(D_METHOD("_on_add_button_pressed"), &CustomPropertiesEditor::_on_add_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_new_key_line_edit_text_changed", "new_text"), &CustomPropertiesEditor::_on_new_key_line_edit_text_changed);

    BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_DEFINITION);
    BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_CATEGORY);
}

CustomPropertiesEditor::CustomPropertiesEditor() {
    resource_type = RESOURCE_TYPE_ITEM_DEFINITION;
    database = nullptr;
    v_box_container = nullptr;
    add_button = nullptr;
    line_edit = nullptr;
    new_type_option_button = nullptr;
    title_text = "Custom Properties";
}

CustomPropertiesEditor::~CustomPropertiesEditor() {
}

void CustomPropertiesEditor::_ready() {
    set_h_size_flags(Control::SIZE_EXPAND_FILL);
    set_v_size_flags(Control::SIZE_EXPAND_FILL);

    // Title label
    Label* label = memnew(Label);
    label->set_text(title_text);
    label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    add_child(label);

    // Panel for controls
    Control* panel = memnew(Control);
    panel->set_clip_contents(true);
    panel->set_custom_minimum_size(Vector2(0, 40));
    add_child(panel);

    // HBox container for controls
    HBoxContainer* hbox_container = memnew(HBoxContainer);
    hbox_container->set_anchors_and_offsets_preset(Control::PRESET_CENTER_LEFT);
    hbox_container->set_anchor(SIDE_RIGHT, 1.0);
    hbox_container->set_offset(SIDE_TOP, -15.5);
    hbox_container->set_offset(SIDE_BOTTOM, 15.5);
    hbox_container->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    hbox_container->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);
    panel->add_child(hbox_container);

    // "New Property:" label
    Label* label2 = memnew(Label);
    label2->set_text("New Property:");
    label2->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    hbox_container->add_child(label2);

    // LineEdit for property name
    line_edit = memnew(LineEdit);
    line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    line_edit->set_placeholder("Place New Property Key Name Here...");
    line_edit->connect("text_changed", callable_mp(this, &CustomPropertiesEditor::_on_new_key_line_edit_text_changed));
    hbox_container->add_child(line_edit);

    // OptionButton for property type
    new_type_option_button = memnew(OptionButton);
    hbox_container->add_child(new_type_option_button);

    // Add button
    add_button = memnew(Button);
    add_button->set_text("Add New Property");
    add_button->connect("pressed", callable_mp(this, &CustomPropertiesEditor::_on_add_button_pressed));
    hbox_container->add_child(add_button);

    // ScrollContainer for properties
    ScrollContainer* scroll_container = memnew(ScrollContainer);
    scroll_container->set_custom_minimum_size(Vector2(0, 128));
    scroll_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    add_child(scroll_container);

    // VBoxContainer for property editors
    v_box_container = memnew(VBoxContainer);
    v_box_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    v_box_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    scroll_container->add_child(v_box_container);

    if (line_edit) {
        add_button->set_disabled(line_edit->get_text().is_empty());
    }
    build_type_options();
}

void CustomPropertiesEditor::set_resource_type(ResourceType p_type) {
    resource_type = p_type;
}

void CustomPropertiesEditor::set_title(const String& p_title) {
    title_text = p_title;
}

void CustomPropertiesEditor::load_resource(InventoryDatabase* p_database, const Ref<Resource>& p_resource) {
    database = p_database;
    current_resource = p_resource;
    loading_properties();
}

Dictionary CustomPropertiesEditor::get_properties_from_resource() {
    if (!current_resource.is_valid()) {
        return Dictionary();
    }

    if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
        Ref<ItemDefinition> item = current_resource;
        if (item.is_valid()) {
            return item->get_properties();
        }
    } else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
        Ref<ItemCategory> category = current_resource;
        if (category.is_valid()) {
            return category->get_item_properties();
        }
    }
    
    return Dictionary();
}

void CustomPropertiesEditor::set_properties_to_resource(const Dictionary& properties) {
    if (!current_resource.is_valid()) {
        return;
    }

    if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
        Ref<ItemDefinition> item = current_resource;
        if (item.is_valid()) {
            item->set_properties(properties);
        }
    } else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
        Ref<ItemCategory> category = current_resource;
        if (category.is_valid()) {
            category->set_item_properties(properties);
        }
    }
}

TypedArray<String> CustomPropertiesEditor::get_dynamic_properties_from_resource() {
    if (!current_resource.is_valid()) {
        return TypedArray<String>();
    }

    if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
        Ref<ItemDefinition> item = current_resource;
        if (item.is_valid()) {
            return item->get_dynamic_properties();
        }
    } else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
        Ref<ItemCategory> category = current_resource;
        if (category.is_valid()) {
            return category->get_item_dynamic_properties();
        }
    }
    
    return TypedArray<String>();
}

void CustomPropertiesEditor::set_dynamic_properties_to_resource(const TypedArray<String>& dynamic_properties) {
    if (!current_resource.is_valid()) {
        return;
    }

    if (resource_type == RESOURCE_TYPE_ITEM_DEFINITION) {
        Ref<ItemDefinition> item = current_resource;
        if (item.is_valid()) {
            item->set_dynamic_properties(dynamic_properties);
        }
    } else if (resource_type == RESOURCE_TYPE_ITEM_CATEGORY) {
        Ref<ItemCategory> category = current_resource;
        if (category.is_valid()) {
            category->set_item_dynamic_properties(dynamic_properties);
        }
    }
}

Dictionary CustomPropertiesEditor::make_dictionary_unique(const Dictionary& input_dictionary) {
    Dictionary unique_dictionary;
    Array keys = input_dictionary.keys();
    for (int i = 0; i < keys.size(); i++) {
        Variant key = keys[i];
        unique_dictionary[key] = input_dictionary[key];
    }
    return unique_dictionary;
}

void CustomPropertiesEditor::loading_properties() {
    // Clear existing property editors
    for (int i = 0; i < properties_obj.size(); i++) {
        if (ItemDefinitionPropertyEditor* prop = Object::cast_to<ItemDefinitionPropertyEditor>(properties_obj[i])) {
            prop->queue_free();
        }
    }
    properties_obj.clear();

    if (!current_resource.is_valid()) {
        return;
    }

    Dictionary new_item_properties = make_dictionary_unique(get_properties_from_resource());
    Array keys = new_item_properties.keys();
    
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i].get_type() == Variant::STRING) {
            String key = keys[i];
            ItemDefinitionPropertyEditor* property = memnew(ItemDefinitionPropertyEditor);
            TypedArray<String> dynamic_props = get_dynamic_properties_from_resource();
            bool is_dynamic = dynamic_props.find(key) != -1;
            
            // Add to scene tree first so _ready gets called
            v_box_container->add_child(property);
            properties_obj.append(property);
            
            // Then setup with the actual values
            property->setup(key, is_dynamic, get_properties_from_resource()[key]);
            property->connect("removed", callable_mp(this, &CustomPropertiesEditor::_property_removed).bind(current_resource));
            property->connect("changed", callable_mp(this, &CustomPropertiesEditor::_property_changed).bind(current_resource));
        }
    }
    
    set_properties_to_resource(new_item_properties);
}

void CustomPropertiesEditor::build_type_options() {
    if (!new_type_option_button) return;
    
    new_type_option_button->clear();
    // Note: In production, you'd get these icons from EditorInterface
    new_type_option_button->add_icon_item(get_theme_icon("bool", "EditorIcons"), "Boolean", Variant::BOOL);
    new_type_option_button->add_icon_item(get_theme_icon("int", "EditorIcons"), "Integer", Variant::INT);
    new_type_option_button->add_icon_item(get_theme_icon("float", "EditorIcons"), "Float", Variant::FLOAT);
    new_type_option_button->add_icon_item(get_theme_icon("String", "EditorIcons"), "String", Variant::STRING);
    new_type_option_button->add_icon_item(get_theme_icon("Color", "EditorIcons"), "Color", Variant::COLOR);

    // Select the first item after adding all items
    if (new_type_option_button->get_item_count() > 0) {
        new_type_option_button->select(0);
    }
}

void CustomPropertiesEditor::_property_removed(const String& key, const Ref<Resource>& resource) {
    if (!resource.is_valid()) return;
    
    Dictionary properties = get_properties_from_resource();
    properties.erase(key);
    set_properties_to_resource(properties);
    loading_properties();
}

void CustomPropertiesEditor::_property_changed(const String& key, bool is_dynamic, const Variant& value, const Ref<Resource>& resource) {
    if (!resource.is_valid()) return;
    
    Dictionary properties = get_properties_from_resource();
    properties[key] = value;
    set_properties_to_resource(properties);
    
    TypedArray<String> dynamic_properties = get_dynamic_properties_from_resource();
    int index = dynamic_properties.find(key);
    bool contains = index != -1;
    
    if (contains != is_dynamic) {
        if (is_dynamic) {
            dynamic_properties.append(key);
        } else {
            dynamic_properties.remove_at(index);
        }
        set_dynamic_properties_to_resource(dynamic_properties);
    }
}

void CustomPropertiesEditor::_on_add_button_pressed() {
    if (!current_resource.is_valid() || !line_edit || !new_type_option_button) return;
    
    String key_text = line_edit->get_text();
    if (key_text.is_empty()) return;
    
    Dictionary new_item_properties = make_dictionary_unique(get_properties_from_resource());
    
    // Set default value based on type
    int selected_type = new_type_option_button->get_selected_id();
    switch (selected_type) {
        case Variant::BOOL:
            new_item_properties[key_text] = false;
            break;
        case Variant::INT:
            new_item_properties[key_text] = 0;
            break;
        case Variant::FLOAT:
            new_item_properties[key_text] = 0.0;
            break;
        case Variant::STRING:
            new_item_properties[key_text] = String();
            break;
        case Variant::COLOR:
            new_item_properties[key_text] = Color();
            break;
    }
    
    set_properties_to_resource(new_item_properties);
    line_edit->set_text("");
    loading_properties();
}

void CustomPropertiesEditor::_on_new_key_line_edit_text_changed(const String& new_text) {
    if (add_button) {
        add_button->set_disabled(new_text.is_empty());
    }
}

#endif // TOOLS_ENABLED