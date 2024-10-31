#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "base/craft_station_type.h"
#include "base/inventory_database.h"
#include "base/item_category.h"
#include "base/item_definition.h"
#include "base/item_stack.h"
#include "base/node_inventories.h"
#include "base/recipe.h"
#include "base/slot.h"
#include "core/quad_tree.h"
#include "core/hotbar.h"
#include "core/inventory.h"
#include "core/grid_inventory.h"
#include "craft/craft_station.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(CraftStationType);
	GDREGISTER_CLASS(InventoryDatabase);
	GDREGISTER_CLASS(ItemCategory);
	GDREGISTER_CLASS(ItemDefinition);
	GDREGISTER_CLASS(ItemStack);
	GDREGISTER_CLASS(NodeInventories);
	GDREGISTER_CLASS(Recipe);
	GDREGISTER_CLASS(Slot);
	GDREGISTER_CLASS(QuadTree);
	GDREGISTER_CLASS(QuadTree::QuadNode);
	GDREGISTER_CLASS(QuadTree::QuadRect);
	GDREGISTER_CLASS(Hotbar);
	GDREGISTER_CLASS(Inventory);
	GDREGISTER_CLASS(GridInventory);
	GDREGISTER_CLASS(CraftStation);
	GDREGISTER_CLASS(Crafting);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT inventory_system_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

	return init_obj.init();
}
}