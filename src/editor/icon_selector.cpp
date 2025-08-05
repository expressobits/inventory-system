#include "icon_selector.h"

#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void IconSelector::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_icon", "icon"), &IconSelector::load_icon);
    ClassDB::bind_method(D_METHOD("set_editor_plugin", "editor_plugin"), &IconSelector::set_editor_plugin);
    ClassDB::bind_method(D_METHOD("_on_texture_picker_resource_changed", "resource"), &IconSelector::_on_texture_picker_resource_changed);

    ADD_SIGNAL(MethodInfo("icon_changed", PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D")));
}

IconSelector::IconSelector() {
    label = nullptr;
    texture_picker = nullptr;
    editor_plugin = nullptr;
}

IconSelector::~IconSelector() {
}

void IconSelector::_ready() {
    // Create label
    label = memnew(Label);
    label->set_custom_minimum_size(Vector2(160, 0));
    label->set_text("Icon");
    add_child(label);

    // Create texture picker
    texture_picker = memnew(EditorResourcePicker);
    texture_picker->set_base_type("Texture2D");
    texture_picker->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    texture_picker->connect("resource_changed", Callable(this, "_on_texture_picker_resource_changed"));
    add_child(texture_picker);
}

void IconSelector::load_icon(const Ref<Texture2D>& icon) {
    if (icon.is_valid() && texture_picker) {
        texture_picker->set_edited_resource(icon);
    }
}

void IconSelector::set_editor_plugin(EditorPlugin* p_editor_plugin) {
    editor_plugin = p_editor_plugin;
    // EditorResourcePicker will automatically use the current editor plugin context
    // No explicit set_editor_plugin method needed for EditorResourcePicker
}

void IconSelector::_on_texture_picker_resource_changed(const Ref<Resource>& resource) {
    Ref<Texture2D> texture = resource;
    emit_signal("icon_changed", texture);
}

// namespace godot