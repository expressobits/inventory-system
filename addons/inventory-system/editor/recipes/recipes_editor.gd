@tool
class_name RecipesEditor
extends InventoryTabEditor

@onready var search_icon = $HSplitContainer/RecipesListEditor/VBoxContainer/Control/SearchIcon
@onready var recipe_editor : RecipeEditor = %RecipeEditor
@onready var recipes_list : RecipesListEditor = %RecipesListEditor

func _ready():
	super._ready()
	recipe_editor.changed.connect(_on_recipe_editor_changed.bind())
	recipes_list.selected.connect(_on_recipes_list_selected.bind())
	recipes_list.request_remove.connect(_on_recipe_item_editor_request_remove.bind())
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	recipe_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func on_load_database() -> void:
#	recipe_editor.load_item(null)
	load_recipes()


func load_recipes() -> void:
#	recipe_item_editor.clear_list()
#	inventory_item_list.load_items(database)
	recipe_editor.visible = false
	recipes_list.load_recipes(database)


func remove_current_data():
	remove_recipe(current_data)


func remove_recipe(recipe : Recipe):
	if recipe == null:
		return
	var index = database.recipes.find(recipe)
	database.recipes.remove_at(index)
	ResourceSaver.save(database, database.resource_path)
	load_recipes()


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	recipe_editor.load_recipe(recipe, database)
	recipe_editor.visible = true


func _apply_theme():
	super._apply_theme()
	if not is_instance_valid(search_icon):
		return

	search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_recipe_item_editor_changed_products_in_recipe(recipe):
	load_recipes()
#	if inventory_item_list.recipe_item_map.has(current_item):
#		var recipes = inventory_item_list.recipe_item_map[current_item]
#		recipe_item_editor.set_recipes_and_load(recipes, database)
#		inventory_item_list.select(current_item.id)
#		recipe_item_editor.select_with_recipe(recipe)
#	else:
#		recipe_item_editor.clear_list()


func _add_new_recipe_to_database(recipe : Recipe):
	if database.items.is_empty():
		push_warning("There are no items to create a recipe, create an item first.")
		return
	_add_to_database(recipe)
	

func _add_to_database(recipe : Recipe):
	database.recipes.append(recipe)
	ResourceSaver.save(database, database.resource_path)
	load_recipes()
#	var index = inventory_item_list.get_index_of_item_id(recipe.products[0].item.id)
#	inventory_item_list.select(recipe.products[0].item.id)
#	var recipes = inventory_item_list.recipe_item_map[recipe.products[0].item]
	recipes_list.load_recipes(database)
	data_changed.emit()


func _on_recipe_item_editor_recipe_removed():
	load_recipes()
#	if inventory_item_list.recipe_item_map.has(current_item):
#		var recipes = inventory_item_list.recipe_item_map[current_item]
#		recipe_item_editor.set_recipes_and_load(recipes, database)
#	else:
#		recipe_item_editor.clear_list()


func _on_new_resource_dialog_file_selected(path):
	var is_saved_resource := false
	var recipe : Recipe = Recipe.new()
	recipe.ingredients = []
	recipe.products = []
	recipe.required_items = []
	var err = ResourceSaver.save(recipe, path)
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	if err == OK:
		recipe = load(path)
		_add_new_recipe_to_database(recipe)
	else:
		print(err)


func _on_open_resource_dialog_file_selected(path):
	# TODO Check if resource exist
	var res = load(path)
	if res is Recipe:
		var recipe : Recipe = res as Recipe
		if database.recipes.has(recipe):
			push_warning("The recipe "+ recipe.resource_path +" already exists in the database!")
			return
		_add_to_database(recipe)


func _on_recipe_item_editor_request_remove(recipe, request_code):
	match request_code:
		RecipeItemListEditor.REMOVE:
			remove_confirmation_dialog.dialog_text = "Confirm Remove Recipe?"
			remove_confirmation_dialog.popup_centered()
			current_data = recipe
		RecipeItemListEditor.REMOVE_AND_DELETE_RESOURCE:
			remove_and_delete_confirmation_dialog.dialog_text = "Confirm Remove Recipe And Delete Resource of Recipe?"
			remove_and_delete_confirmation_dialog.popup_centered()
			current_data = recipe


func _on_data_changed():
	load_recipes()
	recipes_list.load_recipes(database)


func _on_recipe_editor_changed():
	recipes_list.update_recipes_ui()


func _on_recipes_list_selected(recipe : Recipe):
	load_recipe(recipe, database)
