/**
 * ingredient_item_in_recipe_item.cpp
 * =============================================================================
 * Copyright (c) 2024 Expresso Bits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file.
 * =============================================================================
 */

#ifdef TOOLS_ENABLED

#include "ingredient_item_in_recipe_item.h"

#include "../base/inventory_database.h"
#include "../base/item_stack.h"
#include "../base/item_definition.h"

using namespace godot;

void IngredientItemInRecipeItem::_bind_methods() {
}

void IngredientItemInRecipeItem::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_create_ui();
		} break;
	}
}

IngredientItemInRecipeItem::IngredientItemInRecipeItem() {
	database = nullptr;
	texture_rect = nullptr;
	label = nullptr;
	
	set_custom_minimum_size(Vector2(48, 0));
	add_theme_constant_override("separation", 0);
}

IngredientItemInRecipeItem::~IngredientItemInRecipeItem() {
}

void IngredientItemInRecipeItem::_create_ui() {
	// Label for amount
	label = memnew(Label);
	add_child(label);
	label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	label->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);

	// Texture rect for item icon
	texture_rect = memnew(TextureRect);
	add_child(texture_rect);
	texture_rect->set_custom_minimum_size(Vector2(24, 24));
	texture_rect->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
	texture_rect->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	texture_rect->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	texture_rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	
	
	// Update display if we already have data
	if (item_stack.is_valid() && database) {
		Ref<ItemDefinition> definition = database->get_item(item_stack->get_item_id());
		if (definition.is_valid() && definition->get_icon().is_valid()) {
			texture_rect->set_texture(definition->get_icon());
		} else {
			texture_rect->set_texture(Ref<Texture2D>());
		}
		label->set_text(String::num_uint64(item_stack->get_amount()));
	}
}

void IngredientItemInRecipeItem::setup(InventoryDatabase *p_database, const Ref<ItemStack> &p_item_stack) {
	database = p_database;
	item_stack = p_item_stack;
	
	if (!texture_rect || !label) {
		return; // UI not ready yet
	}
	
	if (item_stack.is_valid() && database) {
		Ref<ItemDefinition> definition = database->get_item(item_stack->get_item_id());
		if (definition.is_valid() && definition->get_icon().is_valid()) {
			texture_rect->set_texture(definition->get_icon());
		} else {
			texture_rect->set_texture(Ref<Texture2D>());
		}
		label->set_text(String::num_uint64(item_stack->get_amount()));
	}
}

#endif // TOOLS_ENABLED