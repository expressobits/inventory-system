extends Control
class_name CraftingUI

var station : CraftStation
var recipe : Recipe
var crafting : CraftStation.Crafting 
@onready var icon : TextureRect = get_node("TextureProgressBar/Icon")
@onready var progress_bar : TextureProgressBar = get_node("TextureProgressBar")

func set_crafting(station : CraftStation, crafting_index : int):
	self.station = station
	crafting = station.craftings[crafting_index]
	var recipe_index = crafting.recipe_index
	self.recipe = station.database.recipes[recipe_index]
	var item = recipe.products[0].item
	self.icon.texture = item.icon
	progress_bar.max_value = recipe.time_to_craft


func _process(delta):
	progress_bar.value =  progress_bar.max_value - crafting.time
