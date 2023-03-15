extends Control
class_name RecipeUI

var recipe : Recipe
var recipe_index : int
var crafter : Crafter
@onready var icon : TextureRect = get_node("Control/ItemIcon")
@onready var item_name : Label = get_node("ItemName")
@onready var time_to_craft : Label = get_node("TimeToCraft")
@onready var craft_button : Button = get_node("CraftButton")
@onready var ingredients_container : HBoxContainer = get_node("RequiredItemsList")

@export var ingredient_scene : PackedScene 
var ingredients : Array[IngredientUI]

func _ready():
	craft_button.button_down.connect(_on_craft_button_button_down.bind())


func set_recipe(crafter : Crafter, recipe : Recipe, recipe_index : int):
	self.crafter = crafter
	self.recipe = recipe
	self.recipe_index = recipe_index
	icon.texture = recipe.product.item.icon
	item_name.text = recipe.product.item.name
	time_to_craft.text = str(recipe.time_to_craft)
	clear_ingredients()
	for ingredient in recipe.ingredients:
		var ingredient_obj : IngredientUI = ingredient_scene.instantiate()
		ingredients_container.add_child(ingredient_obj)
		ingredient_obj.setup(ingredient)
		ingredients.append(ingredient_obj)


func clear_ingredients():
	for ingredient_ui in ingredients:
		ingredient_ui.queue_free()
	ingredients.clear()		


func _on_craft_button_button_down():
	crafter.main_station.craft(recipe_index)
