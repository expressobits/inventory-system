#include "inventory_system.h"

void InventorySystem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_console_mode", "console_mode"), &InventorySystem::set_console_mode);
	ClassDB::bind_method(D_METHOD("is_console_mode"), &InventorySystem::is_console_mode);
	ClassDB::bind_method(D_METHOD("setup_inventory_handler", "inventory_handler"), &InventorySystem::setup_inventory_handler);
	ClassDB::bind_method(D_METHOD("setup_hotbar", "hotbar"), &InventorySystem::setup_hotbar);
	ClassDB::bind_method(D_METHOD("setup_crafter", "crafter"), &InventorySystem::setup_crafter);
	ClassDB::bind_method(D_METHOD("setup_interactor", "interactor"), &InventorySystem::setup_interactor);
	ClassDB::bind_method(D_METHOD("get_inventory_handler"), &InventorySystem::get_inventory_handler);
	ClassDB::bind_method(D_METHOD("get_crafter"), &InventorySystem::get_crafter);
	ClassDB::bind_method(D_METHOD("get_hotbar"), &InventorySystem::get_hotbar);
	ClassDB::bind_method(D_METHOD("get_interactor"), &InventorySystem::get_interactor);
	ADD_SIGNAL(MethodInfo("inventory_handler_changed"));
	ADD_SIGNAL(MethodInfo("crafter_changed"));
	ADD_SIGNAL(MethodInfo("hotbar_changed"));
	ADD_SIGNAL(MethodInfo("interactor_changed"));
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "console_mode"), "set_console_mode", "is_console_mode");
}

void InventorySystem::set_console_mode(const bool new_console_mode) {
	console_mode = new_console_mode;
}

bool InventorySystem::is_console_mode() const {
	return console_mode;
}

void InventorySystem::setup_inventory_handler(InventoryHandler *new_inventory_handler) {
	inventory_handler = new_inventory_handler;
	emit_signal("inventory_handler_changed");
}

void InventorySystem::setup_crafter(Crafter *new_crafter) {
	crafter = new_crafter;
	emit_signal("crafter_changed");
}

void InventorySystem::setup_hotbar(Hotbar *new_hotbar) {
	hotbar = new_hotbar;
	emit_signal("hotbar_changed");
}

void InventorySystem::setup_interactor(Interactor *new_interactor) {
	interactor = new_interactor;
	emit_signal("interactor_changed");
}

InventoryHandler *InventorySystem::get_inventory_handler() const {
	return inventory_handler;
}

Crafter *InventorySystem::get_crafter() const {
	return crafter;
}

Hotbar *InventorySystem::get_hotbar() const {
	return hotbar;
}

Interactor *InventorySystem::get_interactor() const {
	return interactor;
}
