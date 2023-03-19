extends Control
class_name CraftStationUI


@export var recipe_ui_scene : PackedScene
@export var input_inventory_ui : InventoryUI
@export var output_inventory_ui : InventoryUI
@export var view_input_inventory := true
@export var view_output_inventory := true

@onready var recipes_container : VBoxContainer = get_node("RecipesUI/Control/MarginContainer/ScrollContainer/UI Inventory")
@onready var craftings_ui : CraftingsUI = get_node(NodePath("CraftingsUI"))


var recipe_uis : Array[RecipeUI]


func open(craft_station : CraftStation):
	clear()
	craftings_ui.set_craft_station(craft_station)
	var recipes = craft_station.database.recipes
	for i in craft_station.valid_recipes.size():
		var recipe_index = craft_station.valid_recipes[i]
		var recipe = craft_station.database.recipes[recipe_index]
		var recipe_ui : RecipeUI = recipe_ui_scene.instantiate()
		recipe_uis.append(recipe_ui)
		recipes_container.add_child(recipe_ui)
		recipe_ui.set_recipe(craft_station, recipe, recipe_index)
	visible = true
	var valid_input = view_input_inventory and craft_station.input_inventory != null
	var valid_output = view_output_inventory and craft_station.output_inventory != null
	if valid_input: 
		input_inventory_ui.set_inventory(craft_station.input_inventory)
	if valid_output: 
		input_inventory_ui.set_inventory(craft_station.input_inventory)
	input_inventory_ui.visible = valid_input
	output_inventory_ui.visible = valid_output


func clear():
	for recipe_ui in recipe_uis:
		recipe_ui.queue_free()
	recipe_uis.clear()
	

func close():
	visible = false
