extends Node
class_name FPSDemo

@export var item_wood : SlotItem
@export var item_stone : SlotItem
@export var item_grass : SlotItem
@export var database : InventoryDatabase

@onready var main_inventory : Inventory

func _ready():
	main_inventory = InventorySystem.inventory_handler.inventories[0]

func _process(_delta):
	if Input.is_action_just_released("add_item_a"):
		InventorySystem.inventory_handler.add_to_inventory(main_inventory, item_wood, 1)
	if Input.is_action_just_released("remove_item_a"):
		main_inventory.remove(item_wood, 1)

	if Input.is_action_just_released("add_item_b"):
		InventorySystem.inventory_handler.add_to_inventory(main_inventory, item_stone, 1)
	if Input.is_action_just_released("remove_item_b"):
		main_inventory.remove(item_stone, 1)

	if Input.is_action_just_released("add_item_c"):
		InventorySystem.inventory_handler.add_to_inventory(main_inventory, item_grass, 1)
	if Input.is_action_just_released("remove_item_c"):
		main_inventory.remove(item_grass, 1)
