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
#include "base/loot_item.h"
#include "base/loot.h"
#include "constraints/inventory_constraint.h"
#include "constraints/grid_inventory_constraint.h"
#include "core/quad_tree.h"
#include "core/hotbar.h"
#include "core/inventory.h"
#include "core/grid_inventory.h"
#include "core/loot_generator.h"
#include "craft/craft_station.h"

#ifdef TOOLS_ENABLED
#include "editor/base_inventory_editor.h"
#include "editor/icon_selector.h"
#include "editor/resource_id_editor.h"
#include "editor/item_stack_selector.h"
#include "editor/item_definition_property_editor.h"
#include "editor/custom_properties_editor.h"
#include "editor/categories_in_item_editor.h"
#include "editor/inventory_editor_plugin.h"
#include "editor/inventory_settings.h"
#include "editor/inventory_item_list_editor.h"
#include "editor/item_definitions_editor.h"
#include "editor/item_definition_editor.h"
#include "editor/recipes_editor.h"
#include "editor/recipe_editor.h"
#include "editor/recipes_list_editor.h"
#include "editor/recipe_item_list_editor.h"
#include "editor/ingredient_item_in_recipe_item.h"
#include "editor/craft_station_types_editor.h"
#include "editor/craft_station_type_editor.h"
#include "editor/item_categories_editor.h"
#include "editor/item_category_editor.h"
#include "editor/loots_editor.h"
#include "editor/loot_editor.h"
#endif

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(CraftStationType);
		GDREGISTER_CLASS(InventoryDatabase);
		GDREGISTER_CLASS(ItemCategory);
		GDREGISTER_CLASS(ItemDefinition);
		GDREGISTER_CLASS(ItemStack);
		GDREGISTER_CLASS(LootItem);
		GDREGISTER_CLASS(Loot);
		GDREGISTER_CLASS(NodeInventories);
		GDREGISTER_CLASS(Recipe);
		GDREGISTER_CLASS(InventoryConstraint);
		GDREGISTER_CLASS(GridInventoryConstraint);
		GDREGISTER_CLASS(QuadTree);
		GDREGISTER_CLASS(QuadTree::QuadNode);
		GDREGISTER_CLASS(QuadTree::QuadRect);
		GDREGISTER_CLASS(Hotbar);
		GDREGISTER_CLASS(Hotbar::Slot);
		GDREGISTER_CLASS(Inventory);
		GDREGISTER_CLASS(GridInventory);
		GDREGISTER_CLASS(LootGenerator);
		GDREGISTER_CLASS(CraftStation);
		GDREGISTER_CLASS(Crafting);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		GDREGISTER_INTERNAL_CLASS(InventoryEditor);
		GDREGISTER_INTERNAL_CLASS(BaseInventoryEditor);
		GDREGISTER_INTERNAL_CLASS(IconSelector);
		GDREGISTER_INTERNAL_CLASS(ResourceIDEditor);
		GDREGISTER_INTERNAL_CLASS(ItemStackSelector);
		GDREGISTER_INTERNAL_CLASS(ItemDefinitionPropertyEditor);
		GDREGISTER_INTERNAL_CLASS(CustomPropertiesEditor);
		GDREGISTER_INTERNAL_CLASS(CategoriesInItemEditor);
		GDREGISTER_INTERNAL_CLASS(InventoryEditorPlugin);
		GDREGISTER_INTERNAL_CLASS(InventorySettings);
		GDREGISTER_INTERNAL_CLASS(InventoryItemListEditor);
		GDREGISTER_INTERNAL_CLASS(ItemDefinitionsEditor);
		GDREGISTER_INTERNAL_CLASS(ItemDefinitionEditor);
		GDREGISTER_INTERNAL_CLASS(RecipesEditor);
		GDREGISTER_INTERNAL_CLASS(RecipeEditor);
		GDREGISTER_INTERNAL_CLASS(RecipesListEditor);
		GDREGISTER_INTERNAL_CLASS(RecipeItemListEditor);
		GDREGISTER_INTERNAL_CLASS(IngredientItemInRecipeItem);
		GDREGISTER_INTERNAL_CLASS(CraftStationTypesEditor);
		GDREGISTER_INTERNAL_CLASS(CraftStationTypeEditor);
		GDREGISTER_INTERNAL_CLASS(ItemCategoriesEditor);
		GDREGISTER_INTERNAL_CLASS(ItemCategoryEditor);
		GDREGISTER_INTERNAL_CLASS(LootsEditor);
		GDREGISTER_INTERNAL_CLASS(LootEditor);
		EditorPlugins::add_by_type<InventoryEditorPlugin>();
	}
#endif
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	// Nothing to cleanup yet
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