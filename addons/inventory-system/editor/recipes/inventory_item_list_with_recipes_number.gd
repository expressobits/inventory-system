@tool
extends InventoryItemListEditor


var recipe_item_map : Dictionary = {}

func load_items(database : InventoryDatabase) -> void:
	for recipe in database.recipes:
		if not recipe_item_map.has(recipe.product):
			recipe_item_map[recipe.product.item] = [ recipe ]
		else:
			recipe_item_map[recipe.product.item].append(recipe)
	super.load_items(database)


func update_item(index : int):
	var item_database = item_list_handler[index]
	var name_to_show : String = str(item_database.id)
	var icon : Texture2D = null
	var recipe_count = 0
	if item_database.item != null:
		if item_database.item.name.is_empty():
			name_to_show = "No name"
		else:
			name_to_show = item_database.item.name
		if recipe_item_map.has(item_database.item):
			recipe_count = recipe_item_map[item_database.item].size()
		icon = item_database.item.icon
	list.set_item_text(index, name_to_show +" ("+str(recipe_count)+")")
	list.set_item_disabled(index, recipe_count <= 0)
	list.set_item_selectable(index, recipe_count > 0)
	list.set_item_metadata(index, recipe_count)
	list.set_item_tooltip(index, str(recipe_count) + " recipes")
	list.set_item_icon(index, icon)
