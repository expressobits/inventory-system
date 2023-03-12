@icon("res://addons/inventory-system/icons/inventory_item.svg")
extends Resource
class_name Recipe

@export_group("Product")
@export var product : Array[Slot]

@export_group("Crafting")
@export var time_to_craft := 4.0
@export var station : CraftStationType
@export var required_items : Array[Slot]
