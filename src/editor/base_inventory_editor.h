#ifndef BASE_INVENTORY_EDITOR_H
#define BASE_INVENTORY_EDITOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/resource.hpp>
#include "../base/inventory_database.h"

namespace godot {

class BaseInventoryEditor : public Control {
    GDCLASS(BaseInventoryEditor, Control)

private:
    InventoryDatabase* database;
    EditorPlugin* editor_plugin;
    Ref<Resource> current_data;
    
    public:
    static const int ITEM_REMOVE = 105;
    static const int ITEM_DUPLICATE = 106;
    
protected:
    ConfirmationDialog* remove_confirmation_dialog;
    static void _bind_methods();

public:
    BaseInventoryEditor();
    ~BaseInventoryEditor();

    void _ready() override;

    // Static utility methods
    static String get_name_of_resource(const Ref<Resource>& resource);
    static String get_name_of_resource_path(const String& path);

    // Main functionality
    void load_from_database(InventoryDatabase* p_database);
    virtual void on_load_database() {} // Override in derived classes

    virtual void _apply_theme();

    // Data management
    virtual bool remove_current_data();
    void duplicate_current_data();

    // Getters/Setters
    virtual void set_editor_plugin(EditorPlugin* p_plugin) { editor_plugin = p_plugin; }
    EditorPlugin* get_editor_plugin() const { return editor_plugin; }
    
    void set_current_data(const Ref<Resource>& p_data) { current_data = p_data; }
    Ref<Resource> get_current_data() const { return current_data; }

    InventoryDatabase* get_database() const { return database; }

private:
    void _on_remove_confirmation_dialog_confirmed();
};

} // namespace godot

#endif // BASE_INVENTORY_EDITOR_H