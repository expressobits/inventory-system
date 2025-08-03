#include "item_id_editor.h"
#include "../../base/inventory_database.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ItemIDEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "database", "id"), &ItemIDEditor::setup);
    ClassDB::bind_method(D_METHOD("_apply_theme"), &ItemIDEditor::_apply_theme);
    ClassDB::bind_method(D_METHOD("_on_button_pressed"), &ItemIDEditor::_on_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_id_value_changed", "value"), &ItemIDEditor::_on_id_value_changed);
    ClassDB::bind_method(D_METHOD("_on_theme_changed"), &ItemIDEditor::_on_theme_changed);
    ClassDB::bind_method(D_METHOD("_check_valid_id"), &ItemIDEditor::_check_valid_id);
    ClassDB::bind_method(D_METHOD("has_in_database", "id"), &ItemIDEditor::has_in_database);

    ClassDB::bind_method(D_METHOD("set_ids_must_exist_in_database", "value"), &ItemIDEditor::set_ids_must_exist_in_database);
    ClassDB::bind_method(D_METHOD("get_ids_must_exist_in_database"), &ItemIDEditor::get_ids_must_exist_in_database);
    ClassDB::bind_method(D_METHOD("get_id"), &ItemIDEditor::get_id);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ids_must_exist_in_database"), "set_ids_must_exist_in_database", "get_ids_must_exist_in_database");

    ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "id")));
}

ItemIDEditor::ItemIDEditor() {
    id_line_edit = nullptr;
    button = nullptr;
    label = nullptr;
    ids_must_exist_in_database = false;
    database = nullptr;
}

ItemIDEditor::~ItemIDEditor() {
}

void ItemIDEditor::_ready() {
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

void ItemIDEditor::_apply_theme() {
    if (!button) {
        return;
    }
    
    // Try to get editor icons - in a real implementation you'd need access to EditorInterface
    // For now, we'll use default icons or text
    button->set_text("Edit");
    button->set_tooltip_text("Edit");
}

void ItemIDEditor::setup(InventoryDatabase* p_database, const String& p_id) {
    database = p_database;
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

void ItemIDEditor::_on_button_pressed() {
    if (!id_line_edit || !button) {
        return;
    }

    bool is_editable = id_line_edit->is_editable();
    id_line_edit->set_editable(!is_editable);
    
    String new_id = id_line_edit->get_text();
    
    if (id_line_edit->is_editable()) {
        button->set_tooltip_text("Confirm");
        button->set_text("✓"); // Unicode checkmark
    } else {
        button->set_tooltip_text("Edit");
        button->set_text("Edit");
        if (id != new_id) {
            id = new_id;
            emit_signal("changed", id);
        }
    }
}

void ItemIDEditor::_check_valid_id() {
    if (!id_line_edit || !button || !database) {
        return;
    }

    String new_id = id_line_edit->get_text();
    bool valid;
    String warn_text;
    
    if (ids_must_exist_in_database) {
        valid = (new_id == id) || has_in_database(new_id);
        warn_text = "Item id does not exist in database!";
    } else {
        valid = (new_id == id) || !has_in_database(new_id);
        warn_text = "Item id already exists in database!";
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

bool ItemIDEditor::has_in_database(const String& p_id) const {
    if (!database) {
        return false;
    }
    return database->has_item_id(p_id);
}

void ItemIDEditor::_on_theme_changed() {
    _apply_theme();
}

void ItemIDEditor::_on_id_value_changed(const String& value) {
    _check_valid_id();
}

} // namespace godot