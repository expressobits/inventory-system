@tool
extends Control

@export var recipe_item_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/recipe_item_list_editor.tscn")
@onready var v_box_container = $HSplitContainer/Panel/ScrollContainer/VBoxContainer
@onready var recipe_editor = $HSplitContainer/RecipeEditorContainer/RecipeEditor


var database : InventoryDatabase
var recipes_ui : Array[RecipeItemListEditor] 
var recipes : Array[Recipe]


signal changed_product_in_recipe(new_product : InventoryItem, recipe : Recipe)
signal request_remove(recipe : Recipe, request_code : int)


func set_recipes_and_load(recipes : Array, database : InventoryDatabase):
	self.database = database
	self.recipes = recipes
	load_recipes()


func load_recipes():
	clear_list()
	recipe_editor.visible = false
	for index in recipes.size():
		var recipe = recipes[index]
		var recipe_node = recipe_item_scene.instantiate()
		var recipe_item : RecipeItemListEditor = recipe_node as RecipeItemListEditor
		recipe_item.load_recipe(recipe, database)
		recipes_ui.append(recipe_item)
		v_box_container.add_child(recipe_item)
		recipe_item.selected.connect(_on_recipe_item_selected.bind(index))
		recipe_item.request_remove.connect(_on_recipe_item_request_remove_menu.bind())
	if recipes.size() > 0:
		recipes_ui[0].select()


func select_last():
	recipes_ui[recipes_ui.size()-1].select()


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	recipe_editor.load_recipe(recipe, database)
	recipe_editor.visible = true


func reload():
	load_recipes()
	recipe_editor.visible = false


func select_with_recipe(recipe : Recipe):
	for i in recipes.size():
		if recipes[i] == recipe:
			recipes_ui[i].select()
			_on_recipe_item_selected(i)


func select(index : int):
	load_recipe(recipes[index], database)


func clear_list():
	for editor in recipes_ui:
		editor.queue_free()
	recipes_ui.clear()
	recipe_editor.visible = false


func _on_recipe_editor_changed_product(recipe):
	emit_signal("changed_product_in_recipe", recipe.product.item, recipe)


func _on_recipe_item_selected(index):
	for i in recipes.size():
		if i == index:
			select(index)
		else:
			recipes_ui[i].unselect()


func _on_recipe_item_request_remove_menu(recipe : Recipe, request_code : int):
	emit_signal("request_remove", recipe, request_code)


func _on_recipe_editor_changed():
	for recipe_ui in recipes_ui:
		recipe_ui.update_recipe()
