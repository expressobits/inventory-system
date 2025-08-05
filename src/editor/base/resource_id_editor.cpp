#include "resource_id_editor.h"
#include "../../base/inventory_database.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ResourceIDEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "database", "id"), &ResourceIDEditor::setup);
    ClassDB::bind_method(D_METHOD("_apply_theme"), &ResourceIDEditor::_apply_theme);
    ClassDB::bind_method(D_METHOD("_on_button_pressed"), &ResourceIDEditor::_on_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_id_value_changed", "value"), &ResourceIDEditor::_on_id_value_changed);
    ClassDB::bind_method(D_METHOD("_on_theme_changed"), &ResourceIDEditor::_on_theme_changed);
    ClassDB::bind_method(D_METHOD("_check_valid_id"), &ResourceIDEditor::_check_valid_id);
    ClassDB::bind_method(D_METHOD("has_in_database", "id"), &ResourceIDEditor::has_in_database);

    ClassDB::bind_method(D_METHOD("set_ids_must_exist_in_database", "value"), &ResourceIDEditor::set_ids_must_exist_in_database);
    ClassDB::bind_method(D_METHOD("get_ids_must_exist_in_database"), &ResourceIDEditor::get_ids_must_exist_in_database);
    ClassDB::bind_method(D_METHOD("set_resource_type", "type"), &ResourceIDEditor::set_resource_type);
    ClassDB::bind_method(D_METHOD("get_resource_type"), &ResourceIDEditor::get_resource_type);
    ClassDB::bind_method(D_METHOD("get_id"), &ResourceIDEditor::get_id);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ids_must_exist_in_database"), "set_ids_must_exist_in_database", "get_ids_must_exist_in_database");

    BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_DEFINITION);
    BIND_ENUM_CONSTANT(RESOURCE_TYPE_ITEM_CATEGORY);
    BIND_ENUM_CONSTANT(RESOURCE_TYPE_CRAFT_STATION_TYPE);

    ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "id")));
}

ResourceIDEditor::ResourceIDEditor() {
    id_line_edit = nullptr;
    button = nullptr;
    label = nullptr;
    ids_must_exist_in_database = false;
    resource_type = RESOURCE_TYPE_ITEM_DEFINITION; // Default to item definition
}

ResourceIDEditor::~ResourceIDEditor() {
}

void ResourceIDEditor::_ready() {
    set_custom_minimum_size(Vector2(241, 32));

    // Create label
    label = memnew(Label);
    label->set_text("ID");
    label->set_custom_minimum_size(Vector2(128, 0));
    add_child(label);

    // Create button
    button = memnew(Button);
    button->set_tooltip_text("Edit");
    button->connect("pressed", Callable(this, "_on_button_pressed"));
    add_child(button);

    // Create line edit
    id_line_edit = memnew(LineEdit);
    id_line_edit->set_custom_minimum_size(Vector2(128, 0));
    id_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    id_line_edit->set_placeholder("Place ID here");
    id_line_edit->connect("text_changed", Callable(this, "_on_id_value_changed"));
    add_child(id_line_edit);

    // Connect theme changed signal
    connect("theme_changed", Callable(this, "_on_theme_changed"));
    _apply_theme();
}

void ResourceIDEditor::_apply_theme() {
    if (!button) {
        return;
    }
    
    // Try to get editor icons - in a real implementation you'd need access to EditorInterface
    // For now, we'll use default icons or text
    button->set_button_icon(get_theme_icon("Edit", "EditorIcons"));
    button->set_tooltip_text("Edit");
}

void ResourceIDEditor::setup(InventoryDatabase* p_database, const String& p_id) {
    database = Ref<InventoryDatabase>(p_database);
    id = p_id;
    
    if (id_line_edit) {
        id_line_edit->set_text(p_id);
        id_line_edit->set_editable(false);
    }
    
    if (button) {
        button->set_disabled(false);
        button->set_tooltip_text("Edit");
    }
    
    if (id_line_edit) {
        id_line_edit->set_tooltip_text("");
        id_line_edit->set_modulate(Color(1, 1, 1)); // Color.WHITE
    }
}

void ResourceIDEditor::_on_button_pressed() {
    if (!id_line_edit || !button) {
        return;
    }

    bool is_editable = id_line_edit->is_editable();
    id_line_edit->set_editable(!is_editable);
    
    String new_id = id_line_edit->get_text();
    
    if (id_line_edit->is_editable()) {
        button->set_tooltip_text("Confirm");
        button->set_button_icon(get_theme_icon("ImportCheck", "EditorIcons"));
    } else {
        button->set_tooltip_text("Edit");
        button->set_button_icon(get_theme_icon("Edit", "EditorIcons"));
        if (id != new_id) {
            id = new_id;
            emit_signal("changed", id);
        }
    }
}

void ResourceIDEditor::_check_valid_id() {
    if (!id_line_edit || !button || database.is_null()) {
        return;
    }

    String new_id = id_line_edit->get_text();
    bool valid;
    String warn_text;
    
    if (ids_must_exist_in_database) {
        valid = (new_id == id) || has_in_database(new_id);
        warn_text = "Resource id does not exist in database!";
    } else {
        valid = (new_id == id) || !has_in_database(new_id);
        warn_text = "Resource id already exists in database!";
    }
    
    if (valid) {
        id_line_edit->set_tooltip_text("");
        button->set_tooltip_text("Confirm");
        id_line_edit->set_modulate(Color(1, 1, 1)); // Color.WHITE
    } else {
        id_line_edit->set_tooltip_text(warn_text);
        button->set_tooltip_text(warn_text);
        id_line_edit->set_modulate(Color(0.8f, 0.376f, 0.376f)); // Color.INDIAN_RED
    }
    
    button->set_disabled(!valid);
}

bool ResourceIDEditor::has_in_database(const String& p_id) const {
    if (database.is_null()) {
        return false;
    }
    
    switch (resource_type) {
        case RESOURCE_TYPE_ITEM_DEFINITION:
            return database->has_item_id(p_id);
        case RESOURCE_TYPE_ITEM_CATEGORY:
            return database->has_item_category_id(p_id);
        case RESOURCE_TYPE_CRAFT_STATION_TYPE:
            return database->has_craft_station_type_id(p_id);
        default:
            return false;
    }
}

void ResourceIDEditor::_on_theme_changed() {
    _apply_theme();
}

void ResourceIDEditor::_on_id_value_changed(const String& value) {
    _check_valid_id();
}