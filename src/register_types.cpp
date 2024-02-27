#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "base/craft_station_type.h"
#include "base/inventory_database.h"
#include "base/item.h"
#include "base/item_category.h"
#include "base/item_definition.h"
#include "base/node_inventories.h"
#include "base/recipe.h"
#include "base/slot.h"
#include "core/categorized_slot.h"
#include "core/hotbar.h"
#include "core/inventory.h"
#include "core/inventory_handler.h"
#include "craft/craft_station.h"
#include "craft/crafter.h"
#include "craft/crafting.h"
#include "interact/interact_action.h"
#include "inventory_system.h"

using namespace godot;

InventorySystem *inventory_system = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<CraftStationType>();
	ClassDB::register_class<InventoryDatabase>();
	ClassDB::register_class<Item>();
	ClassDB::register_class<ItemCategory>();
	ClassDB::register_class<ItemDefinition>();
	ClassDB::register_class<NodeInventories>();
	ClassDB::register_class<Recipe>();
	ClassDB::register_class<Slot>();
	ClassDB::register_class<CategorizedSlot>();
	ClassDB::register_class<Hotbar>();
	ClassDB::register_class<Inventory>();
	ClassDB::register_class<InventoryHandler>();
	ClassDB::register_class<CraftStation>();
	ClassDB::register_class<Crafter>();
	ClassDB::register_class<Crafting>();
	ClassDB::register_class<InteractAction>();
	ClassDB::register_class<Interactor>();
	ClassDB::register_class<InventorySystem>();

	inventory_system = memnew(InventorySystem);
	godot::Engine::get_singleton()->register_singleton("InventorySystem", inventory_system);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Engine::get_singleton()->unregister_singleton("InventorySystem");
	memdelete(inventory_system);
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT inventory_system_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}