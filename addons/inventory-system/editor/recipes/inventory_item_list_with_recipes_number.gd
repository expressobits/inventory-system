@tool
extends InventoryItemListEditor

signal no_products_item_selected

var recipe_item_map : Dictionary = {}

func load_items(database : InventoryDatabase) -> void:
	recipe_item_map.clear()
	var no_products_count = 0
	for recipe in database.recipes:
		if is_instance_valid(recipe.product) and is_instance_valid(recipe.product.item):
			var id = recipe.product.item.id
			if not recipe_item_map.has(id):
				var array : Array[Recipe] = []
				recipe_item_map[id] = array
			recipe_item_map[id].append(recipe)
		else:
			no_products_count += 1
	$NoProductsButton.text = "Recipes Without Product ("+str(no_products_count)+")"
	super.load_items(database)
	


func select(item_id : int):
	var idx = get_index_of_item_id(item_id)
	list.select(idx)


func update_item(index : int):
	var item = item_list_handler[index]
	var name_to_show : String 
	var icon : Texture2D = null
	var recipe_count = 0
	if item != null:
		if item.name.is_empty():
			name_to_show = "No name"
		else:
			name_to_show = item.name
		if recipe_item_map.has(item.id):
			recipe_count = recipe_item_map[item.id].size()
		icon = item.icon
	list.set_item_text(index, name_to_show +" ("+str(recipe_count)+")")
	list.set_item_disabled(index, recipe_count <= 0)
	list.set_item_selectable(index, recipe_count > 0)
	list.set_item_metadata(index, recipe_count)
	list.set_item_tooltip(index, str(recipe_count) + " recipes")
	list.set_item_icon(index, icon)



func _on_no_products_button_pressed():
	emit_signal("no_products_item_selected")
