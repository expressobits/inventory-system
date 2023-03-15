extends Control
class_name RecipesUI


@export var recipe_ui_scene : PackedScene

@onready var recipes_container : VBoxContainer = get_node("Control/MarginContainer/ScrollContainer/UI Inventory")
var recipe_uis : Array[RecipeUI]
var player_crafter : Crafter


func set_crafter(crafter : Crafter):
	player_crafter = crafter
	player_crafter.opened.connect(open_craft_station.bind())


func open_craft_station(craft_station : CraftStation):
	clear()
	var recipes = craft_station.database.recipes
	for i in recipes.size():
		var recipe = recipes[i]
		var recipe_ui : RecipeUI = recipe_ui_scene.instantiate()
		recipe_uis.append(recipe_ui)
		recipes_container.add_child(recipe_ui)
		recipe_ui.set_recipe(player_crafter, recipe, i)
	visible = true


func clear():
	for recipe_ui in recipe_uis:
		recipe_ui.queue_free()
	recipe_uis.clear()
	

func close():
	visible = false
