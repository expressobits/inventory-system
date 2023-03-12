@icon("res://addons/inventory-system/icons/inventory_item.svg")
extends Resource
class_name Recipe

@export var products : Array[Slot]
@export var time_to_craft := 4.0
@export var station : CraftStationType
@export var required_items : Array[Slot]
