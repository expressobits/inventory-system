#ifndef ICON_SELECTOR_H
#define ICON_SELECTOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {

class IconSelector : public HBoxContainer {
    GDCLASS(IconSelector, HBoxContainer)

private:
    Label* label;
    EditorResourcePicker* texture_picker;

protected:
    static void _bind_methods();

public:
    IconSelector();
    ~IconSelector();

    void _ready() override;
    void load_icon(const Ref<Texture2D>& icon);

private:
    void _on_texture_picker_resource_changed(const Ref<Resource>& resource);
};

} // namespace godot

#endif // ICON_SELECTOR_H