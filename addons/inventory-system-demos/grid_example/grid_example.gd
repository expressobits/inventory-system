extends Node

@onready var ctrl_inventory_left : GridInventoryPanel = %GridInventoryPanelLeft
@onready var ctrl_inventory_right : GridInventoryPanel = %GridInventoryPanelRight
@onready var control: Control = $Control


func _ready() -> void:
	ctrl_inventory_left.request_split.connect(_request_split)
	ctrl_inventory_right.request_split.connect(_request_split)
	ctrl_inventory_left.request_transfer_to.connect(_request_transfer_to)
	ctrl_inventory_right.request_transfer_to.connect(_request_transfer_to)
	ctrl_inventory_left.request_sort.connect(_on_inventory_sort.bind(ctrl_inventory_left.inventory))
	ctrl_inventory_right.request_sort.connect(_on_inventory_sort.bind(ctrl_inventory_right.inventory))
	
	await get_tree().create_timer(0.2).timeout
	
	ctrl_inventory_left.inventory.add("wood", 16)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("stone_axe")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("stone_pickaxe")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("campfire", 2)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("workbench")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("grass", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("raw_meat", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("cooked_meat", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("lashing", 3)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("helmet_labor", 2)
	

func _request_split(inventory: GridInventory, stack_index: int, amount: int):
	inventory.split(stack_index, amount)


func _request_transfer_to(origin_inv: GridInventory, origin_pos: Vector2i, dest_inv: GridInventory, dest_pos: Vector2i, amount: int):
	origin_inv.transfer_to(origin_pos, dest_inv, dest_pos, amount)


func _can_drop_data(_at_position: Vector2, _data: Variant) -> bool:
	return true


func _drop_data(_at_position: Vector2, data: Variant) -> void:
	ctrl_inventory_left.inventory.remove_item(data.item)
	ctrl_inventory_right.inventory.remove_item(data.item)
	# Replace the following line with custom logic for handling the item drop:
	data.item.queue_free()


func _on_inventory_sort(inventory) -> void:
	if !inventory.sort():
		print("Warning: InventoryGrid.sort() returned false!")
