@tool
@icon("res://addons/inventory-system/icons/node_inventory_system_base.svg")
extends Node
class_name NodeInventorySystemBase

@export var database : InventoryDatabase


func _get_configuration_warnings() -> PackedStringArray:
	var string_array : PackedStringArray
	if database == null:
		string_array.append("Database is null!")
	return string_array
	
