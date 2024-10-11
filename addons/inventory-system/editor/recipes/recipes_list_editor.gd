@tool
class_name RecipesListEditor
extends Control

@export var recipe_item_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/recipe_item_list_editor.tscn")
@onready var v_box_container = $VBoxContainer/ScrollContainer/VBoxContainer
@onready var search_line_edit = $VBoxContainer/Control/SearchLineEdit

var database : InventoryDatabase
var recipes_ui : Array[RecipeItemListEditor] 
var recipes : Array[Recipe]

var filter: String:
	set(next_filter):
		filter = next_filter
		apply_filter()
	get:
		return filter

signal changed_products_in_recipe(recipe : Recipe)
signal request_remove(recipe : Recipe)
signal selected(recipe : Recipe)


func _ready():
	search_line_edit.text_changed.connect(_on_search_line_edit_text_changed)


func load_recipes(database : InventoryDatabase):
	self.database = database
	apply_filter()


func update_recipes():
	clear_list()
	for index in recipes.size():
		var recipe = recipes[index]
		var recipe_node = recipe_item_scene.instantiate()
		var recipe_item : RecipeItemListEditor = recipe_node as RecipeItemListEditor
		recipe_item.load_recipe(recipe, database)
		recipes_ui.append(recipe_item)
		v_box_container.add_child(recipe_item)
		recipe_item.selected.connect(_on_recipe_item_selected.bind(index))
		recipe_item.request_remove.connect(_on_recipe_item_request_remove_menu)
	if recipes.size() > 0:
		recipes_ui[0].select()


func select_last():
	recipes_ui[recipes_ui.size()-1].select()


func reload():
	load_recipes(database)


func select_with_recipe(recipe : Recipe):
	for i in recipes.size():
		if recipes[i] == recipe:
			recipes_ui[i].select()
			_on_recipe_item_selected(i)


func select(index : int):
	selected.emit(recipes[index])


func update_recipes_ui():
	for recipe_ui in recipes_ui:
		recipe_ui.update_recipe()


func clear_list():
	for editor in recipes_ui:
		editor.queue_free()
	recipes_ui.clear()


func apply_filter() -> void:
	recipes.clear()
	for recipe in database.recipes:
		if is_contains_product_name(recipe, filter):
			recipes.append(recipe)
	update_recipes()


func is_contains_product_name(recipe : Recipe, filter_name : String) -> bool:
	if filter_name == "":
		return true
	for product in recipe.products:
		var item = database.get_item(product.item_id)
		if item == null:
			return true
		if filter_name.to_lower() in item.name.to_lower():
			return true
	return false


func _on_recipe_item_selected(index):
	for i in recipes.size():
		if i == index:
			select(index)
		else:
			recipes_ui[i].unselect()


func _on_recipe_item_request_remove_menu(recipe : Recipe):
	request_remove.emit(recipe)


func _on_search_line_edit_text_changed(new_text : String):
	filter = new_text
