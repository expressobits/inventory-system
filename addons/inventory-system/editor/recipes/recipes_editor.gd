@tool
extends BaseInventoryEditor
class_name RecipesEditor

@onready var new_recipe_resource_dialog = $NewRecipeResourceDialog
@onready var open_recipe_dialog = $OpenRecipeDialog
@onready var search_icon = $HSplitContainer/InventoryItemList/Control/SearchIcon
@onready var inventory_item_list = $HSplitContainer/InventoryItemList
@onready var recipe_item_editor = $HSplitContainer/RecipeItemEditor
@onready var recipe_remove_confirmation_dialog = %RecipeRemoveConfirmationDialog
@onready var recipe_remove_and_delete_confirmation_dialog = %RecipeRemoveAndDeleteConfirmationDialog

var last_item_selected_id := -1
var current_recipe : Recipe

func _ready():
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
#	recipe_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func on_load_database() -> void:
#	recipe_editor.load_item(null)
	load_recipes()


func load_recipes() -> void:
	recipe_item_editor.clear_list()
	inventory_item_list.load_items(database)


func new_recipe_pressed():
	if not is_instance_valid(database):
		return
	new_recipe_resource_dialog.popup_centered()


func new_recipe_from_resource_pressed():
	if not is_instance_valid(database):
		return
	open_recipe_dialog.popup_centered()


func remove_recipe(recipe : Recipe):
	if recipe == null:
		return
	var index = database.recipes.find(recipe)
	database.recipes.remove_at(index)
	load_recipes()


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_recipe_resource_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_recipe_resource_dialog.min_size = Vector2(600, 500) * scale
	
	search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_inventory_item_list_item_selected(item, index):
	last_item_selected_id = item.id
	var recipes = inventory_item_list.recipe_item_map[item.id]
	recipe_item_editor.set_recipes_and_load(recipes, database)


func _on_recipe_item_editor_changed_product_in_recipe(new_product, recipe):
	load_recipes()
	last_item_selected_id = recipe.product.item.id
	if inventory_item_list.recipe_item_map.has(last_item_selected_id):
		var recipes = inventory_item_list.recipe_item_map[last_item_selected_id]
		recipe_item_editor.set_recipes_and_load(recipes, database)
		inventory_item_list.select(last_item_selected_id)
		recipe_item_editor.select_with_recipe(recipe)
	else:
		recipe_item_editor.clear_list()


func _add_new_recipe_to_database(recipe : Recipe):
	if database.items.is_empty():
		push_warning("There are no items to create a recipe, create an item first.")
		return
	
	recipe.product = Slot.new()
	var id = last_item_selected_id
	if not database.has_item_id(id):
		id = database.get_valid_id()
	recipe.product.item = database.get_item(id)
	recipe.product.amount = 1
	
	_add_to_database(recipe)
	

func _add_to_database(recipe : Recipe):
	database.recipes.append(recipe)
	load_recipes()
	var index = inventory_item_list.get_index_of_item_id(recipe.product.item.id)
	inventory_item_list.select(recipe.product.item.id)
	var recipes = inventory_item_list.recipe_item_map[recipe.product.item.id]
	recipe_item_editor.set_recipes_and_load(recipes, database)


func _on_recipe_item_editor_recipe_removed():
	load_recipes()
	if inventory_item_list.recipe_item_map.has(last_item_selected_id):
		var recipes = inventory_item_list.recipe_item_map[last_item_selected_id]
		recipe_item_editor.set_recipes_and_load(recipes, database)
	else:
		recipe_item_editor.clear_list()


func _on_inventory_item_list_no_products_item_selected():
	var recipes : Array[Recipe]
	for recipe in database.recipes:
		if recipe.product.item == null:
			recipes.append(recipe)
	recipe_item_editor.set_recipes_and_load(recipes, database)


func _on_items_items_changed():
	load_recipes()
	recipe_item_editor.reload()


func _on_craft_stations_station_added():
	load_recipes()
	recipe_item_editor.reload()


func _on_craft_stations_station_removed():
	load_recipes()
	recipe_item_editor.reload()


func _on_new_recipe_resource_dialog_file_selected(path):
	var is_saved_resource := false
	var recipe : Recipe = Recipe.new()
	var err = ResourceSaver.save(recipe, path)
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	if err == OK:
		recipe = load(path)
		_add_new_recipe_to_database(recipe)
	else:
		print(err)


func _on_open_recipe_dialog_file_selected(path):
	# TODO Check if resource exist
	var res = load(path)
	if res is Recipe:
		var recipe : Recipe = res as Recipe
		if database.recipes.has(recipe):
			push_warning("The recipe "+ recipe.resource_path +" already exists in the database!")
		_add_to_database(recipe)


func _on_recipe_item_editor_request_remove(recipe, request_code):
	match request_code:
		RecipeItemListEditor.REMOVE:
			recipe_remove_confirmation_dialog.dialog_text = "Confirm Remove Recipe?"
			recipe_remove_confirmation_dialog.popup_centered()
			current_recipe = recipe
		RecipeItemListEditor.REMOVE_AND_DELETE_RESOURCE:
			recipe_remove_and_delete_confirmation_dialog.dialog_text = "Confirm Remove Recipe And Delete Resource of Recipe?"
			recipe_remove_and_delete_confirmation_dialog.popup_centered()
			current_recipe = recipe


func _on_recipe_remove_confirmation_dialog_confirmed():
	remove_recipe(current_recipe)


func _on_recipe_remove_and_delete_confirmation_dialog_confirmed():
	var dir = DirAccess.open(".")
	if current_recipe == null:
		return
	var code = dir.remove_absolute(current_recipe.resource_path)
	if code == OK:
		remove_recipe(current_recipe)
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
