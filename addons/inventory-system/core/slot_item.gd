class_name SlotItem
extends Resource

@export var definition : InventoryItem
@export var properties : Dictionary


func is_stack_with(other_item : SlotItem) -> bool:
	if other_item.definition == definition:
		return properties.is_empty() and other_item.properties.is_empty()
	else:
		return false 
