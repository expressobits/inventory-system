@icon("res://addons/inventory-system/icons/recipe.svg")
extends Resource
class_name Recipe

## Main end result of crafting done with this recipe.
@export var product : Slot

## Products that are generated in addition to the main product when finished crafting this recipe
@export var byproducts : Array[Slot]

## Time to complete this craft
@export var time_to_craft := 4.0

## What type of season can make this craft
@export var station : CraftStationType

## Ingredient List For crafting this recipe, the ingredients contain a [Slot] data structure, which stores an [InventoryItem] type and its quantity.
@export var ingredients : Array[Slot]
