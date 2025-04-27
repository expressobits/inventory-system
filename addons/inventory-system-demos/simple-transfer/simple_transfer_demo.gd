extends Node3D

@export var inventory_left: Inventory
@export var inventory_right: Inventory
@onready var button_move_to_right: Button = %MoveToRight
@onready var button_move_to_left: Button = %MoveToLeft
@onready var inventory_ui_left: SimpleInventoryUI = %InventoryUILeft
@onready var inventory_ui_right: SimpleInventoryUI = %InventoryUIRight

func _ready() -> void:
	button_move_to_right.pressed.connect(_on_button_move_to_right_pressed)
	button_move_to_left.pressed.connect(_on_button_move_to_left_pressed)
	
	inventory_left.add("wood", 16)
	await get_tree().create_timer(0.2).timeout
	inventory_left.add("stone_axe")
	await get_tree().create_timer(0.2).timeout
	inventory_left.add("stone_pickaxe")
	await get_tree().create_timer(0.2).timeout
	inventory_left.add("campfire", 2)
	await get_tree().create_timer(0.2).timeout
	inventory_left.add("workbench")
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("grass", 8)
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("raw_meat", 8)
	await get_tree().create_timer(0.2).timeout
	inventory_right.add("cooked_meat", 8)


func _on_button_move_to_right_pressed() -> void:
	var selected_items: Array[int] = inventory_ui_left.get_selected_inventory_items()
	if selected_items.is_empty():
		return

	for selected_item_index in selected_items:
		inventory_left.transfer(selected_item_index, inventory_right)


func _on_button_move_to_left_pressed() -> void:
	var selected_items: Array[int] = inventory_ui_right.get_selected_inventory_items()
	if selected_items.is_empty():
		return

	for selected_item_index in selected_items:
		inventory_right.transfer(selected_item_index, inventory_left)
