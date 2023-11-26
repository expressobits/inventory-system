extends Control
class_name CraftingUI

## Interface that represents a crafting that is in a craft station list.

## What station is this crafting from
var station : CraftStation

## Crafting information (Time and recipe)
var crafting : CraftStation.Crafting 

@onready var _icon : TextureRect = %Icon
@onready var _progress_bar : ProgressBar = %ProgressBar


## Set crafting based on [CraftStation] crafting index
func set_crafting(station : CraftStation, crafting_index : int):
	self.station = station
	crafting = station.craftings[crafting_index]
	var recipe_index = crafting.recipe_index
	var recipe = station.database.recipes[recipe_index]
	var item = recipe.products[0].item
	_icon.texture = item.definition.icon
	_progress_bar.max_value = recipe.time_to_craft


func _process(delta):
	_progress_bar.value =  _progress_bar.max_value - crafting.time
