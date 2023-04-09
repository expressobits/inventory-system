@tool
extends Control

@export var recipe_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/recipe_editor.tscn")
@onready var v_box_container = $ScrollContainer/VBoxContainer


var database : InventoryDatabase
var recipes_editors : Array[RecipeEditor] 


signal changed_product_in_recipe


func load_recipes(recipes : Array, database : InventoryDatabase):
	self.database = database
	clear_list()
	for recipe in recipes:
		var recipe_node = recipe_scene.instantiate()
		var recipe_editor : RecipeEditor = recipe_node as RecipeEditor
		recipe_editor.load_recipe(recipe, database)
		recipes_editors.append(recipe_editor)
		v_box_container.add_child(recipe_editor)
		recipe_editor.changed_product.connect(_on_reciped_changed_product.bind())


func clear_list():
	for editor in recipes_editors:
		editor.queue_free()
	recipes_editors.clear()


func _on_reciped_changed_product():
	emit_signal("changed_product_in_recipe")
