#ifndef ITEM_ID_EDITOR_H
#define ITEM_ID_EDITOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/theme.hpp>

namespace godot {

class InventoryDatabase;

class ItemIDEditor : public HBoxContainer {
    GDCLASS(ItemIDEditor, HBoxContainer)

private:
    LineEdit* id_line_edit;
    Button* button;
    Label* label;
    bool ids_must_exist_in_database;
    
    InventoryDatabase* database;
    String id;

protected:
    static void _bind_methods();

public:
    ItemIDEditor();
    ~ItemIDEditor();

    void _ready() override;
    void _apply_theme();
    
    void setup(InventoryDatabase* p_database, const String& p_id);
    
    // Getters/Setters
    void set_ids_must_exist_in_database(bool p_value) { ids_must_exist_in_database = p_value; }
    bool get_ids_must_exist_in_database() const { return ids_must_exist_in_database; }
    
    String get_id() const { return id; }

private:
    void _on_button_pressed();
    void _on_id_value_changed(const String& value);
    void _on_theme_changed();
    void _check_valid_id();
    bool has_in_database(const String& p_id) const;
};

} // namespace godot

#endif // ITEM_ID_EDITOR_H