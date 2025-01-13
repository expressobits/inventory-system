extends Control
class_name DropArea

signal request_drop(stack: ItemStack, inventory: Inventory)

@onready var player_grid_inventory_ui: GridInventoryPanel = %PlayerGridInventoryUI


func _can_drop_data(_at_position: Vector2, _data: Variant) -> bool:
	return true


func _drop_data(_at_position: Vector2, data: Variant) -> void:
	request_drop.emit(data.stack, data.inventory)
