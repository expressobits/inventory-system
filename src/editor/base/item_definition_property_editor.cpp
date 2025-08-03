#include "item_definition_property_editor.h"

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
    line_edit = nullptr;
    no_compatible = nullptr;
    value_bool = nullptr;
    value_integer = nullptr;
    value_float_line_edit = nullptr;
    value_line_edit = nullptr;
    value_color = nullptr;
    dynamic_property = nullptr;
    check_box = nullptr;
    delete_button = nullptr;
    remove_confirmation_dialog = nullptr;
    is_dynamic_property = false;

    // Initialize icons array
    icons_name.append("null");
    icons_name.append("bool");
    icons_name.append("int");
    icons_name.append("float");
    icons_name.append("String");
    icons_name.append("Vector2");
    icons_name.append("Vector2i");
    icons_name.append("Rect2");
    icons_name.append("Rect2i");
    icons_name.append("Vector3");
    icons_name.append("Vector3i");
    icons_name.append("Transform2D");
    icons_name.append("Vector4");
    icons_name.append("Vector4i");
    icons_name.append("Plane");
    icons_name.append("Quaternion");
    icons_name.append("AABB");
    icons_name.append("Basis");
    icons_name.append("Transform3D");
    icons_name.append("Projection");
    icons_name.append("Color");
    icons_name.append("StringName");
    icons_name.append("NodePath");
    icons_name.append("RID");
    icons_name.append("Object");
    icons_name.append("Callable");
    icons_name.append("Signal");
    icons_name.append("Dictionary");
    icons_name.append("Array");
    icons_name.append("PackedByteArray");
    icons_name.append("PackedInt32Array");
    icons_name.append("PackedInt64Array");
    icons_name.append("PackedFloat32Array");
    icons_name.append("PackedFloat64Array");
    icons_name.append("PackedStringArray");
    icons_name.append("PackedVector2Array");
    icons_name.append("PackedVector3Array");
    icons_name.append("PackedColorArray");
}

ItemDefinitionPropertyEditor::~ItemDefinitionPropertyEditor() {
}

void ItemDefinitionPropertyEditor::_ready() {
    set_custom_minimum_size(Vector2(0, 32));
    set_h_size_flags(Control::SIZE_EXPAND_FILL);

    // Create value type icon
    value_type = memnew(TextureRect);
    value_type->set_custom_minimum_size(Vector2(24, 24));
    value_type->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);
    // Note: Would need access to EditorInterface to get theme icon
    add_child(value_type);

    // Create key label
    Label* key_label = memnew(Label);
    key_label->set_text("Key");
    add_child(key_label);

    // Create key line edit
    line_edit = memnew(LineEdit);
    line_edit->set_custom_minimum_size(Vector2(64, 0));
    line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    line_edit->set_placeholder("Key here...");
    line_edit->set_editable(false);
    add_child(line_edit);

    // Create value label
    Label* label_value = memnew(Label);
    label_value->set_text("Value");
    add_child(label_value);

    // Create value container
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

    // Create integer editor
    value_integer = memnew(SpinBox);
    value_integer->set_visible(false);
    value_integer->set_custom_minimum_size(Vector2(128, 0));
    value_integer->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_integer->set_rounded(true);
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

    // Create string editor
    value_line_edit = memnew(LineEdit);
    value_line_edit->set_visible(false);
    value_line_edit->set_custom_minimum_size(Vector2(128, 0));
    value_line_edit->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_line_edit->set_placeholder("Value here...");
    value_line_edit->connect("text_changed", Callable(this, "_on_value_line_edit_text_changed"));
    control->add_child(value_line_edit);

    // Create color editor
    value_color = memnew(ColorPicker);
    value_color->set_visible(false);
    value_color->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    value_color->connect("color_changed", Callable(this, "_on_value_color_color_changed"));
    control->add_child(value_color);

    // Create dynamic property container
    dynamic_property = memnew(HBoxContainer);
    add_child(dynamic_property);

    Label* dynamic_property_label = memnew(Label);
    dynamic_property_label->set_text("Is Dynamic");
    dynamic_property->add_child(dynamic_property_label);

    check_box = memnew(CheckBox);
    check_box->set_pressed(is_dynamic_property);
    check_box->connect("toggled", Callable(this, "_on_dynamic_property_checkbox_toggled"));
    dynamic_property->add_child(check_box);

    // Create delete button
    delete_button = memnew(Button);
    delete_button->set_tooltip_text("Delete");
    delete_button->set_flat(true);
    delete_button->set_text("X"); // Would use Remove icon in real implementation
    delete_button->connect("pressed", Callable(this, "_on_delete_button_pressed"));
    add_child(delete_button);

    // Create confirmation dialog
    remove_confirmation_dialog = memnew(ConfirmationDialog);
    remove_confirmation_dialog->set_size(Vector2i(400, 100));
    remove_confirmation_dialog->set_dialog_text("Remove?");
    remove_confirmation_dialog->connect("confirmed", Callable(this, "_on_remove_confirmation_dialog_confirmed"));
    add_child(remove_confirmation_dialog);
}

void ItemDefinitionPropertyEditor::setup(const String& p_key, bool p_is_dynamic_property, const Variant& p_value) {
    key = p_key;
    value = p_value;
    is_dynamic_property = p_is_dynamic_property;

    if (line_edit) {
        line_edit->set_text(key);
    }
    
    if (check_box) {
        check_box->set_pressed(is_dynamic_property);
    }

    // Hide all value editors first
    if (value_bool) value_bool->set_visible(false);
    if (value_integer) value_integer->set_visible(false);
    if (value_float_line_edit) value_float_line_edit->set_visible(false);
    if (value_line_edit) value_line_edit->set_visible(false);
    if (value_color) value_color->set_visible(false);
    if (no_compatible) no_compatible->set_visible(false);

    // Show appropriate editor based on value type
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
        case Variant::VECTOR4: // Using VECTOR4 as a stand-in for COLOR since Godot 4 changed this
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