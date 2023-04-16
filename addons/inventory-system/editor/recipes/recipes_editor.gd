@tool
extends Control
class_name RecipesEditor


var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var new_recipe_resource_dialog = $NewRecipeResourceDialog
@onready var search_icon = $HSplitContainer/InventoryItemList/Control/SearchIcon
@onready var inventory_item_list = $HSplitContainer/InventoryItemList
@onready var recipe_item_editor = $HSplitContainer/RecipeItemEditor
var last_item_selected_id : int


func _ready():
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
#	recipe_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
#	recipe_editor.load_item(null)
	load_recipes()


func load_recipes() -> void:
	recipe_item_editor.clear_list()
	inventory_item_list.load_items(database)


func new_recipe_pressed():
	if not is_instance_valid(database):
		return
	_add_new_recipe_to_database(Recipe.new())
#	new_recipe_resource_dialog.popup_centered()


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_recipe_resource_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_recipe_resource_dialog.min_size = Vector2(600, 500) * scale
	
	search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_inventory_item_list_item_selected(item_database, index):
	last_item_selected_id = item_database.id
	var recipes = inventory_item_list.recipe_item_map[item_database.id]
	recipe_item_editor.set_recipes_and_load(recipes, database)


func _on_recipe_item_editor_changed_product_in_recipe(new_product, recipe):
	load_recipes()
	last_item_selected_id = database.get_id_from_item(recipe.product.item)
	if inventory_item_list.recipe_item_map.has(last_item_selected_id):
		var recipes = inventory_item_list.recipe_item_map[last_item_selected_id]
		recipe_item_editor.set_recipes_and_load(recipes, database)
		inventory_item_list.select(last_item_selected_id)
		recipe_item_editor.select_with_recipe(recipe)
	else:
		recipe_item_editor.clear_list()


func _on_new_recipe_resource_dialog_file_selected(path):
	var is_saved_resource := false
	var recipe : Recipe = Recipe.new()
	if is_saved_resource:
		var err = ResourceSaver.save(recipe, path)
		if err == OK:
			recipe = load(path)
			_add_new_recipe_to_database(recipe)
		else:
			print(err)
	else:
		_add_new_recipe_to_database(recipe)


func _add_new_recipe_to_database(recipe : Recipe):
	if database.items.is_empty():
		push_warning("There are no items to create a recipe, create an item first.")
		return
	recipe.product = Slot.new()
	recipe.product.item = database.items[last_item_selected_id].item
	recipe.product.amount = 1
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	database.recipes.append(recipe)
	load_recipes()
	inventory_item_list.select(last_item_selected_id)
	var id = database.get_id_from_item(recipe.product.item)
	var recipes = inventory_item_list.recipe_item_map[id]
	recipe_item_editor.set_recipes_and_load(recipes, database)
	recipe_item_editor.select_last()


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
