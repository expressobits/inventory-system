#ifdef TOOLS_ENABLED

#ifndef ITEM_DEFINITION_PROPERTY_EDITOR_H
#define ITEM_DEFINITION_PROPERTY_EDITOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/color_picker.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

namespace godot {

class ItemDefinitionPropertyEditor : public HBoxContainer {
    GDCLASS(ItemDefinitionPropertyEditor, HBoxContainer)

private:
    ColorRect* color_rect;
    TextureRect* value_type;
    Label* key_label;
    LineEdit* line_edit;
    Label* value_label;
    Label* no_compatible;
    CheckBox* value_bool;
    SpinBox* value_integer;
    SpinBox* value_float_line_edit;
    LineEdit* value_line_edit;
    ColorPicker* value_color;
    HBoxContainer* dynamic_property;
    Label* dynamic_property_label;
    CheckBox* check_box;
    Button* delete_button;
    ConfirmationDialog* remove_confirmation_dialog;

    String key;
    Variant value;
    bool is_dynamic_property;
    
    // Setup parameters (stored until _ready is called)
    bool setup_called;
    String pending_key;
    Variant pending_value;
    bool pending_is_dynamic;

protected:
    static void _bind_methods();

public:
    ItemDefinitionPropertyEditor();
    ~ItemDefinitionPropertyEditor();

    void _ready() override;
    void setup(const String& p_key, bool p_is_dynamic_property, const Variant& p_value);
    void apply_setup();

private:
    void _changed();
    void _on_dynamic_property_checkbox_toggled(bool toggled);
    void _on_delete_button_pressed();
    void _on_remove_confirmation_dialog_confirmed();
    void _on_value_line_edit_text_changed(const String& new_text);
    void _on_value_float_value_changed(double new_value);
    void _on_value_integer_value_changed(double new_value);
    void _on_value_bool_toggled(bool button_pressed);
    void _on_value_color_color_changed(const Color& color);
};

} // namespace godot

#endif // ITEM_DEFINITION_PROPERTY_EDITOR_H

#endif // TOOLS_ENABLED