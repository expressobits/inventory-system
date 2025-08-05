#include "base_inventory_editor.h"
#include "../base/inventory_database.h"

#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void BaseInventoryEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_from_database", "database"), &BaseInventoryEditor::load_from_database);
    ClassDB::bind_method(D_METHOD("on_load_database"), &BaseInventoryEditor::on_load_database);
    ClassDB::bind_method(D_METHOD("_apply_theme"), &BaseInventoryEditor::_apply_theme);
    ClassDB::bind_method(D_METHOD("remove_current_data"), &BaseInventoryEditor::remove_current_data);
    ClassDB::bind_method(D_METHOD("duplicate_current_data"), &BaseInventoryEditor::duplicate_current_data);
    ClassDB::bind_method(D_METHOD("_on_remove_confirmation_dialog_confirmed"), &BaseInventoryEditor::_on_remove_confirmation_dialog_confirmed);

    ClassDB::bind_method(D_METHOD("set_editor_plugin", "plugin"), &BaseInventoryEditor::set_editor_plugin);
    ClassDB::bind_method(D_METHOD("get_editor_plugin"), &BaseInventoryEditor::get_editor_plugin);
    ClassDB::bind_method(D_METHOD("set_current_data", "data"), &BaseInventoryEditor::set_current_data);
    ClassDB::bind_method(D_METHOD("get_current_data"), &BaseInventoryEditor::get_current_data);

    ADD_SIGNAL(MethodInfo("data_changed"));
    ADD_SIGNAL(MethodInfo("removed", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "Resource")));
    ADD_SIGNAL(MethodInfo("duplicated", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "Resource")));
}

BaseInventoryEditor::BaseInventoryEditor() {
    database = nullptr;
    editor_plugin = nullptr;
    remove_confirmation_dialog = nullptr;
}

BaseInventoryEditor::~BaseInventoryEditor() {
}

void BaseInventoryEditor::_ready() {
    // Create remove confirmation dialog
    remove_confirmation_dialog = memnew(ConfirmationDialog);
    remove_confirmation_dialog->set_name("Remove Confirmation Dialog");
    remove_confirmation_dialog->set_text("Are you sure you want to remove this item?");
    add_child(remove_confirmation_dialog);

    // Connect the confirmed signal
    remove_confirmation_dialog->connect("confirmed", Callable(this, "_on_remove_confirmation_dialog_confirmed"));
}

String BaseInventoryEditor::get_name_of_resource(const Ref<Resource>& resource) {
    if (resource.is_null()) {
        return "";
    }
    return get_name_of_resource_path(resource->get_path());
}

String BaseInventoryEditor::get_name_of_resource_path(const String& path) {
    String filename = path.get_file();
    // Remove .tres extension
    if (filename.ends_with(".tres")) {
        return filename.substr(0, filename.length() - 5);
    }
    return filename;
}

void BaseInventoryEditor::load_from_database(InventoryDatabase* p_database) {
    database = p_database;
    on_load_database();
}

void BaseInventoryEditor::_apply_theme() {
    if (!editor_plugin || !remove_confirmation_dialog) {
        return;
    }
    
    // Apply editor scaling if available
    // This would be used to scale UI elements according to editor scale
}

bool BaseInventoryEditor::remove_current_data() {
    if (current_data.is_valid()) {
        emit_signal("removed", current_data);
        return true;
    }
    return false;
}

void BaseInventoryEditor::duplicate_current_data() {
    if (current_data.is_valid()) {
        emit_signal("duplicated", current_data);
    }
}

void BaseInventoryEditor::_on_remove_confirmation_dialog_confirmed() {
    remove_current_data();
}

} // namespace godot