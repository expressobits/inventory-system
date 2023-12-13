extends Control
class_name CraftStationUI

## Represents a [CraftStation] in the interface.
## It uses instanced scenes from [RecipeUI] to represent every possible station recipe.
## Use the [CraftingsUI] to represent each active crafting in the station.



## It uses instanced scenes from [RecipeUI] to represent every possible station recipe.
@export var recipe_ui_scene : PackedScene

## [InventoryUI] which represents the input inventory.
@export var input_inventory_ui : InventoryUI

## [InventoryUI] which represents the output inventory.
@export var output_inventory_ui : InventoryUI

## Show input inventory when open [CraftStationUI].
@export var view_input_inventory := true

## Show output inventory when open [CraftStationUI].
@export var view_output_inventory := true

@onready var _recipes_container : VBoxContainer = %RecipesContent
@onready var _craftings_ui : CraftingsUI = get_node(NodePath("CraftingsUI"))


var _recipe_uis : Array[RecipeUI]


## Configure a craftstation for the [Recipe] list and [CraftingsUI] list
func open(craft_station : CraftStation):
	_clear()
	_craftings_ui.set_craft_station(craft_station)
	var recipes = craft_station.database.recipes
	for i in craft_station.valid_recipes.size():
		var recipe_index = craft_station.valid_recipes[i]
		var recipe = craft_station.database.recipes[recipe_index]
		var recipe_ui : RecipeUI = recipe_ui_scene.instantiate()
		_recipe_uis.append(recipe_ui)
		_recipes_container.add_child(recipe_ui)
		recipe_ui.set_recipe(craft_station, recipe, recipe_index)
	visible = true
	var valid_input = view_input_inventory and craft_station.input_inventories.size() > 0 and craft_station.input_inventories[0] != null
	var valid_output = view_output_inventory and craft_station.output_inventories.size() > 0 and craft_station.output_inventories[0] != null
	if valid_input: 
		input_inventory_ui.set_inventory(craft_station.input_inventories[0])
	if valid_output: 
		input_inventory_ui.set_inventory(craft_station.input_inventories[0])
	input_inventory_ui.visible = valid_input
	output_inventory_ui.visible = valid_output


## Hidden this UI
func close():
	visible = false


func _clear():
	for recipe_ui in _recipe_uis:
		recipe_ui.queue_free()
	_recipe_uis.clear()
