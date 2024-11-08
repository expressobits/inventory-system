class_name FPSDemo
extends Node

@export var database : InventoryDatabase

var main_inventory : Inventory

func _ready():
	#Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	$"UI/Inventory System UI".setup($"Player/CharacterInventorySystem")
	main_inventory = $"Player/CharacterInventorySystem".main_inventory
	main_inventory.add("workbench", 1)
	
#
func _process(_delta):
	pass
	if Input.is_action_just_released("add_item_a"):
		main_inventory.add("wood", 1)
	if Input.is_action_just_released("remove_item_a"):
		main_inventory.remove("wood", 1)

	if Input.is_action_just_released("add_item_b"):
		main_inventory.add("stone", 1)
	if Input.is_action_just_released("remove_item_b"):
		main_inventory.remove("stone", 1)
#
	if Input.is_action_just_released("add_item_c"):
		main_inventory.add("grass", 1)
	if Input.is_action_just_released("remove_item_c"):
		main_inventory.remove("grass", 1)
