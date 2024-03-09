class_name FPSDemo
extends Node

@export var item_wood : Item
@export var item_stone : Item
@export var item_grass : Item
@export var database : InventoryDatabase

@onready var main_inventory : Inventory

func _ready():
	#main_inventory = InventorySystem.get_inventory_handler().get_inventory(0)
	$"UI/Inventory System UI".setup($"Player/CharacterInventorySystem")
	
#
#func _process(_delta):
	#if Input.is_action_just_released("add_item_a"):
		#InventorySystem.get_inventory_handler().add_to_inventory(main_inventory, item_wood, 1)
	#if Input.is_action_just_released("remove_item_a"):
		#main_inventory.remove(item_wood, 1)
#
	#if Input.is_action_just_released("add_item_b"):
		#InventorySystem.get_inventory_handler().add_to_inventory(main_inventory, item_stone, 1)
	#if Input.is_action_just_released("remove_item_b"):
		#main_inventory.remove(item_stone, 1)
#
	#if Input.is_action_just_released("add_item_c"):
		#InventorySystem.get_inventory_handler().add_to_inventory(main_inventory, item_grass, 1)
	#if Input.is_action_just_released("remove_item_c"):
		#main_inventory.remove(item_grass, 1)
