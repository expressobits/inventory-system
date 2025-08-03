@tool
class_name RecipesEditor
extends BaseInventoryEditor

@onready var search_icon = $HSplitContainer/RecipesListEditor/VBoxContainer/Control/SearchIcon
@onready var recipe_editor : RecipeEditor = %RecipeEditor
@onready var recipes_list : RecipesListEditor = %RecipesListEditor

func _ready():
	super._ready()
	recipe_editor.changed.connect(_on_recipe_editor_changed)
	recipes_list.selected.connect(_on_recipes_list_selected)
	recipes_list.request_remove.connect(_on_recipe_item_editor_request_remove)
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	recipe_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func on_load_database() -> void:
	load_recipes()


func load_recipes() -> void:
	recipe_editor.visible = false
	recipes_list.load_recipes(database)


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


func _add_new_recipe_to_database(recipe : Recipe):
	if database.items.is_empty():
		push_warning("There are no items to create a recipe, create an item first.")
		return
	_add_to_database(recipe)
	

func _add_to_database(recipe : Recipe):
	database.recipes.append(recipe)
	ResourceSaver.save(database, database.resource_path)
	load_recipes()
	recipes_list.load_recipes(database)
	data_changed.emit()


func _on_recipe_item_editor_recipe_removed():
	load_recipes()


func _on_recipe_item_editor_request_remove(recipe):
	remove_confirmation_dialog.dialog_text = "Confirm Remove Recipe?"
	remove_confirmation_dialog.popup_centered()
	current_data = recipe


func _on_data_changed():
	load_recipes()
	recipes_list.load_recipes(database)


func _on_recipe_editor_changed():
	recipes_list.update_recipes_ui()


func _on_recipes_list_selected(recipe : Recipe):
	load_recipe(recipe, database)
