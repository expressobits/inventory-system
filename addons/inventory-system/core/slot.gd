extends Resource
class_name Slot

@export var item : InventoryItem
@export var amount := 0

func get_item_id() -> int:
	if item == null:
		return InventoryItem.NONE
	else:
		return item.id
