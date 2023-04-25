@tool
@icon("res://addons/inventory-system/icons/node_inventory_system_base.svg")
extends Node
class_name NodeInventorySystemBase

@export var database : InventoryDatabase

var items_cache : Dictionary


func _ready():
	for item in database.items:
		items_cache[item.id] = item


func _get_configuration_warnings() -> PackedStringArray:
	var string_array : PackedStringArray
	if database == null:
		string_array.append("Database is null!")
	return string_array
	

func get_item_from_id(id : int) -> InventoryItem:
	return database.get_item(id)
#	if items_cache.has(id):
#		return items_cache[id]
#	return null


func get_id_from_item(item : InventoryItem) -> int:
	return item.id
