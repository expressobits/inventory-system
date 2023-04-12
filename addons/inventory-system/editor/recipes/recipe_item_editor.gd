@tool
extends Control

@export var recipe_item_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/recipe_item.tscn")
@onready var v_box_container = $HSplitContainer/ScrollContainer/VBoxContainer
@onready var recipe_editor = $HSplitContainer/RecipeEditorContainer/RecipeEditor


var database : InventoryDatabase
var recipes_ui : Array[RecipeItem] 
var recipes : Array[Recipe]

signal changed_product_in_recipe


func load_recipes(recipes : Array, database : InventoryDatabase):
	self.database = database
	self.recipes = recipes
	clear_list()
	recipe_editor.visible = false
	for index in recipes.size():
		var recipe = recipes[index]
		var recipe_node = recipe_item_scene.instantiate()
		var recipe_item : RecipeItem = recipe_node as RecipeItem
		recipe_item.load_recipe(recipe, database)
		recipes_ui.append(recipe_item)
		v_box_container.add_child(recipe_item)
		recipe_item.selected.connect(_on_recipe_item_selected.bind(index))
	
func load_recipe(recipe : Recipe, database : InventoryDatabase):
	recipe_editor.load_recipe(recipe, database)
	recipe_editor.visible = true


func select(index : int):
	load_recipe(recipes[index], database)


func clear_list():
	for editor in recipes_ui:
		editor.queue_free()
	recipes_ui.clear()


func _on_recipe_editor_changed_product():
	emit_signal("changed_product_in_recipe")


func _on_recipe_item_selected(index):
	select(index)
