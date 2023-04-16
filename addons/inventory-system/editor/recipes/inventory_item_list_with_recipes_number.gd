@tool
extends InventoryItemListEditor

var recipe_item_map : Dictionary = {}

func load_items(database : InventoryDatabase) -> void:
	recipe_item_map.clear()
	var no_products_count = 0
	for recipe in database.recipes:
		if is_instance_valid(recipe.product):
			if not recipe_item_map.has(recipe.product.id):
				var array : Array[Recipe] = []
				recipe_item_map[recipe.product.id] = array
			recipe_item_map[recipe.product.id].append(recipe)
	clear_items()
	self.database = database
	for recipes in database.recipes:
		if not item_ids.has(recipes.product.id):
			add_item_id(recipes.product.id)
	


func select(item_id : int):
	var idx = get_index_of_item_id(item_id)
	list.select(idx)


func update_item(index : int):
	var item_id = item_list_handler[index]
	var name_to_show : String = str(item_id)
	
	var item_database = database.get_item_database(item_id)
	var icon : Texture2D = null
	
	if item_database != null and item_database.item != null:
		if item_database.item.name.is_empty():
			name_to_show = "No name"
		else:
			name_to_show = item_database.item.name
		icon = item_database.item.icon
	
	var recipe_count = 0
	if recipe_item_map.has(item_id):
		recipe_count = recipe_item_map[item_id].size()
		
	list.set_item_text(index, name_to_show +" ("+str(recipe_count)+")")
	list.set_item_disabled(index, recipe_count <= 0)
	list.set_item_selectable(index, recipe_count > 0)
	list.set_item_metadata(index, recipe_count)
	list.set_item_tooltip(index, str(recipe_count) + " recipes")
	list.set_item_icon(index, icon)
