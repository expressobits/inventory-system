@tool
extends Control
class_name RecipesEditor


var database : InventoryDatabase
var editor_plugin : EditorPlugin

@onready var new_recipe_resource_dialog = $NewRecipeResourceDialog
@onready var search_icon = $HSplitContainer/InventoryItemList/Control/SearchIcon
@onready var inventory_item_list = $HSplitContainer/InventoryItemList
@onready var recipe_item_editor = $HSplitContainer/RecipeItemEditor


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
	inventory_item_list.load_items(database)


func new_recipe_pressed():
	if not is_instance_valid(database):
		return
	
	new_recipe_resource_dialog.popup_centered()


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_recipe_resource_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_recipe_resource_dialog.min_size = Vector2(600, 500) * scale
	
	search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_inventory_item_list_item_selected(item_database, index):
	var recipes = inventory_item_list.recipe_item_map[item_database.item]
	recipe_item_editor.load_recipes(recipes, database)


func _on_recipe_item_editor_changed_product_in_recipe():
	load_recipes()


func _on_new_recipe_resource_dialog_file_selected(path):
	var item : Recipe = Recipe.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : Recipe = load(path)
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		database.recipes.append(res)
		load_recipes()
	else:
		print(err)
