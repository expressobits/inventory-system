#ifdef TOOLS_ENABLED

#include "item_definition_property_editor.h"
#include "property_type_icons.h"

#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/color_picker.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ItemDefinitionPropertyEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "key", "is_dynamic_property", "value"), &ItemDefinitionPropertyEditor::setup);
    ClassDB::bind_method(D_METHOD("_changed"), &ItemDefinitionPropertyEditor::_changed);
    ClassDB::bind_method(D_METHOD("_on_dynamic_property_checkbox_toggled", "toggled"), &ItemDefinitionPropertyEditor::_on_dynamic_property_checkbox_toggled);
    ClassDB::bind_method(D_METHOD("_on_delete_button_pressed"), &ItemDefinitionPropertyEditor::_on_delete_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_remove_confirmation_dialog_confirmed"), &ItemDefinitionPropertyEditor::_on_remove_confirmation_dialog_confirmed);
    ClassDB::bind_method(D_METHOD("_on_value_line_edit_text_changed", "new_text"), &ItemDefinitionPropertyEditor::_on_value_line_edit_text_changed);
    ClassDB::bind_method(D_METHOD("_on_value_float_value_changed", "new_value"), &ItemDefinitionPropertyEditor::_on_value_float_value_changed);
    ClassDB::bind_method(D_METHOD("_on_value_integer_value_changed", "new_value"), &ItemDefinitionPropertyEditor::_on_value_integer_value_changed);
    ClassDB::bind_method(D_METHOD("_on_value_bool_toggled", "button_pressed"), &ItemDefinitionPropertyEditor::_on_value_bool_toggled);
    ClassDB::bind_method(D_METHOD("_on_value_color_color_changed", "color"), &ItemDefinitionPropertyEditor::_on_value_color_color_changed);

    ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::STRING, "key")));
    ADD_SIGNAL(MethodInfo("changed", PropertyInfo(Variant::STRING, "key"), PropertyInfo(Variant::BOOL, "is_dynamic_property"), PropertyInfo(Variant::NIL, "value")));
}

ItemDefinitionPropertyEditor::ItemDefinitionPropertyEditor() {
    color_rect = nullptr;
    value_type = nullptr;
    key_label = nullptr;
    line_edit = nullptr;
    value_label = nullptr;
    no_compatible = nullptr;
    value_bool = nullptr;
    value_integer = nullptr;
    value_float_line_edit = nullptr;
    value_line_edit = nullptr;
    value_color = nullptr;
    dynamic_property = nullptr;
    dynamic_property_label = nullptr;
    check_box = nullptr;
    delete_button = nullptr;
    remove_confirmation_dialog = nullptr;
    is_dynamic_property = false;
    setup_called = false;
}

ItemDefinitionPropertyEditor::~ItemDefinitionPropertyEditor() {
}

void ItemDefinitionPropertyEditor::_ready() {
    set_custom_minimum_size(Vector2(0, 32));
    set_h_size_flags(Control::SIZE_EXPAND_FILL);

    // Create value type icon - matching GDScript structure
    value_type = memnew(TextureRect);
    value_type->set_custom_minimum_size(Vector2(24, 24));
    value_type->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);
    value_type->set_texture(get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(value.get_type()), "EditorIcons"));
    add_child(value_type);
    
    // Add "Key" label - matching GDScript line 38-41
    key_label = memnew(Label);
    key_label->set_text("Key");
    add_child(key_label);

    // Create key line edit - matching GDScript line 43-52
    line_edit = memnew(LineEdit);
    line_edit->set_custom_minimum_size(Vector2(64, 0));
    line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    line_edit->set_placeholder("Key here...");
    line_edit->set_editable(false); // Make non-editable like GDScript version
    // Add caret blink settings like GDScript
    line_edit->set_caret_blink_enabled(true);
    line_edit->set_caret_blink_interval(0.5);
    add_child(line_edit);
    
    // Add "Value" label - matching GDScript line 54-57
    value_label = memnew(Label);
    value_label->set_text("Value");
    add_child(value_label);

    // Create value container - matching GDScript line 59-62
    HBoxContainer* control = memnew(HBoxContainer);
    control->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    add_child(control);

    // Create no compatible label
    no_compatible = memnew(Label);
    no_compatible->set_visible(false);
    no_compatible->set_tooltip_text("Use the default inspector of godot to edit this item's value.");
    no_compatible->set_text("This type is not compatible with the item editor.");
    control->add_child(no_compatible);

    // Create bool editor
    value_bool = memnew(CheckBox);
    value_bool->set_visible(false);
    value_bool->set_pressed(true);
    value_bool->connect("toggled", Callable(this, "_on_value_bool_toggled"));
    control->add_child(value_bool);

    // Create integer editor - matching GDScript line 77-85
    value_integer = memnew(SpinBox);
    value_integer->set_visible(false);
    value_integer->set_custom_minimum_size(Vector2(128, 0));
    value_integer->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    // Set rounded = true like GDScript version
    value_integer->set_step(1.0);
    value_integer->set_allow_greater(true);
    value_integer->set_allow_lesser(true);
    value_integer->connect("value_changed", Callable(this, "_on_value_integer_value_changed"));
    control->add_child(value_integer);

    // Create float editor
    value_float_line_edit = memnew(SpinBox);
    value_float_line_edit->set_visible(false);
    value_float_line_edit->set_custom_minimum_size(Vector2(128, 0));
    value_float_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_float_line_edit->set_step(0.001);
    value_float_line_edit->set_allow_greater(true);
    value_float_line_edit->set_allow_lesser(true);
    value_float_line_edit->connect("value_changed", Callable(this, "_on_value_float_value_changed"));
    control->add_child(value_float_line_edit);

    // Create string editor - matching GDScript line 97-105
    value_line_edit = memnew(LineEdit);
    value_line_edit->set_visible(false);
    value_line_edit->set_custom_minimum_size(Vector2(128, 0));
    value_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_line_edit->set_placeholder("Value here...");
    // Add caret blink settings like GDScript
    value_line_edit->set_caret_blink_enabled(true);
    value_line_edit->set_caret_blink_interval(0.5);
    value_line_edit->connect("text_changed", Callable(this, "_on_value_line_edit_text_changed"));
    control->add_child(value_line_edit);

    // Create color editor - matching GDScript line 107-111 (uses ColorPicker, not ColorPickerButton)
    value_color = memnew(ColorPicker);
    value_color->set_visible(false);
    value_color->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_color->connect("color_changed", Callable(this, "_on_value_color_color_changed"));
    control->add_child(value_color);

    // Create dynamic property container - matching GDScript line 113-125
    dynamic_property = memnew(HBoxContainer);
    add_child(dynamic_property);
    
    // Add "Is Dynamic" label - matching GDScript line 117-120
    dynamic_property_label = memnew(Label);
    dynamic_property_label->set_text("Is Dynamic");
    dynamic_property->add_child(dynamic_property_label);

    check_box = memnew(CheckBox);
    check_box->set_pressed(is_dynamic_property);
    check_box->connect("toggled", Callable(this, "_on_dynamic_property_checkbox_toggled"));
    dynamic_property->add_child(check_box);

    // Create delete button - matching GDScript line 127-132
    delete_button = memnew(Button);
    delete_button->set_tooltip_text("Delete");
    delete_button->set_flat(true);
    delete_button->set_button_icon(get_theme_icon("Remove", "EditorIcons"));
    delete_button->connect("pressed", Callable(this, "_on_delete_button_pressed"));
    add_child(delete_button);

    // Create confirmation dialog - matching GDScript line 135-139
    remove_confirmation_dialog = memnew(ConfirmationDialog);
    remove_confirmation_dialog->set_size(Vector2i(400, 100));
    remove_confirmation_dialog->set_text("Remove?");
    remove_confirmation_dialog->connect("confirmed", Callable(this, "_on_remove_confirmation_dialog_confirmed"));
    add_child(remove_confirmation_dialog);
    
    // Apply any pending setup parameters now that controls are created
    apply_setup();
}

void ItemDefinitionPropertyEditor::setup(const String& p_key, bool p_is_dynamic_property, const Variant& p_value) {
    pending_key = p_key;
    pending_value = p_value;
    pending_is_dynamic = p_is_dynamic_property;
    setup_called = true;
    
    // If controls are already created (after _ready), apply immediately
    if (line_edit != nullptr) {
        apply_setup();
    }
}

void ItemDefinitionPropertyEditor::apply_setup() {
    if (!setup_called) {
        return;
    }
    
    key = pending_key;
    value = pending_value;
    is_dynamic_property = pending_is_dynamic;

    if (line_edit) {
        line_edit->set_text(key);
    }
    
    if (check_box) {
        check_box->set_pressed(is_dynamic_property);
    }

    // Update type icon - matching GDScript line 35
    if (value_type) {
        value_type->set_texture(get_theme_icon(PropertyTypeIcons::get_icon_name_for_type(value.get_type()), "EditorIcons"));
    }

    // Hide all value editors first
    if (value_bool) value_bool->set_visible(false);
    if (value_integer) value_integer->set_visible(false);
    if (value_float_line_edit) value_float_line_edit->set_visible(false);
    if (value_line_edit) value_line_edit->set_visible(false);
    if (value_color) value_color->set_visible(false);
    if (no_compatible) no_compatible->set_visible(false);

    // Show appropriate editor based on value type - matching GDScript line 141-167
    switch (value.get_type()) {
        case Variant::BOOL:
            if (value_bool) {
                value_bool->set_pressed(value);
                value_bool->set_visible(true);
            }
            break;
        case Variant::INT:
            if (value_integer) {
                value_integer->set_value(int(value));
                value_integer->set_visible(true);
            }
            break;
        case Variant::FLOAT:
            if (value_float_line_edit) {
                value_float_line_edit->set_value(value);
                value_float_line_edit->set_visible(true);
            }
            break;
        case Variant::STRING:
            if (value_line_edit) {
                value_line_edit->set_text(value);
                value_line_edit->set_visible(true);
            }
            break;
        case Variant::COLOR:
            if (value_color) {
                value_color->set_pick_color(value);
                value_color->set_visible(true);
            }
            break;
        default:
            if (no_compatible) {
                no_compatible->set_visible(true);
            }
            break;
    }
}

void ItemDefinitionPropertyEditor::_changed() {
    emit_signal("changed", key, is_dynamic_property, value);
}

void ItemDefinitionPropertyEditor::_on_dynamic_property_checkbox_toggled(bool toggled) {
    is_dynamic_property = toggled;
    _changed();
}

void ItemDefinitionPropertyEditor::_on_delete_button_pressed() {
    if (remove_confirmation_dialog) {
        remove_confirmation_dialog->popup_centered();
    }
}

void ItemDefinitionPropertyEditor::_on_remove_confirmation_dialog_confirmed() {
    emit_signal("removed", key);
}

void ItemDefinitionPropertyEditor::_on_value_line_edit_text_changed(const String& new_text) {
    value = new_text;
    _changed();
}

void ItemDefinitionPropertyEditor::_on_value_float_value_changed(double new_value) {
    value = new_value;
    _changed();
}

void ItemDefinitionPropertyEditor::_on_value_integer_value_changed(double new_value) {
    value = int(new_value);
    _changed();
}

void ItemDefinitionPropertyEditor::_on_value_bool_toggled(bool button_pressed) {
    value = button_pressed;
    _changed();
}

void ItemDefinitionPropertyEditor::_on_value_color_color_changed(const Color& color) {
    value = color;
    _changed();
}

} // namespace godot

#endif // TOOLS_ENABLED