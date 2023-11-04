extends Node
class_name FPSDemo

@export var item_wood : InventoryItem
@export var item_stone : InventoryItem
@export var item_grass : InventoryItem
@export var database : InventoryDatabase


func _process(_delta):
	if Input.is_action_just_released("add_item_a"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_wood, 1)
	if Input.is_action_just_released("remove_item_a"):
		InventorySystem.inventory_handler.inventory.remove(item_wood, 1)
		
	if Input.is_action_just_released("add_item_b"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_stone, 1)
	if Input.is_action_just_released("remove_item_b"):
		InventorySystem.inventory_handler.inventory.remove(item_stone, 1)
		
	if Input.is_action_just_released("add_item_c"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_grass, 1)
	if Input.is_action_just_released("remove_item_c"):
		InventorySystem.inventory_handler.inventory.remove(item_grass, 1)
