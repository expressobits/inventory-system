@icon("res://addons/inventory-system/icons/recipe.svg")
extends Resource
class_name Recipe

@export var product : Slot
@export var subproducts : Array[Slot]
@export var time_to_craft := 4.0
@export var station : CraftStationType
@export var ingredients : Array[Slot]
