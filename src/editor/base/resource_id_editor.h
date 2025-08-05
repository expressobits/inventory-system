#ifndef RESOURCE_ID_EDITOR_H
#define RESOURCE_ID_EDITOR_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/theme.hpp>
#include "../../base/inventory_database.h"

using namespace godot;

class ResourceIDEditor : public HBoxContainer {
    GDCLASS(ResourceIDEditor, HBoxContainer)

public:
    enum ResourceType {
        RESOURCE_TYPE_ITEM_DEFINITION,
        RESOURCE_TYPE_ITEM_CATEGORY,
        RESOURCE_TYPE_CRAFT_STATION_TYPE
    };

private:
    LineEdit* id_line_edit;
    Button* button;
    Label* label;
    bool ids_must_exist_in_database;
    ResourceType resource_type;

    Ref<InventoryDatabase> database;
    String id;

protected:
    static void _bind_methods();

public:
    ResourceIDEditor();
    ~ResourceIDEditor();

    void _ready() override;
    void _apply_theme();

    void setup(InventoryDatabase* p_database, const String& p_id);

    // Getters/Setters
    void set_ids_must_exist_in_database(bool p_value) { ids_must_exist_in_database = p_value; }
    bool get_ids_must_exist_in_database() const { return ids_must_exist_in_database; }
    
    void set_resource_type(ResourceType p_type) { resource_type = p_type; }
    ResourceType get_resource_type() const { return resource_type; }
    
    String get_id() const { return id; }

private:
    void _on_button_pressed();
    void _on_id_value_changed(const String& value);
    void _on_theme_changed();
    void _check_valid_id();
    bool has_in_database(const String& p_id) const;
};

VARIANT_ENUM_CAST(ResourceIDEditor::ResourceType);

#endif // RESOURCE_ID_EDITOR_H