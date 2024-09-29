class_name FPSDemo
extends Node

@export var database : InventoryDatabase

var main_inventory : Inventory

func _ready():
	$"UI/Inventory System UI".setup($"Player/CharacterInventorySystem")
	main_inventory = get_node("Player").get_node("CharacterInventorySystem").get_node("Inventory")
	
#
func _process(_delta):
	pass
	#if Input.is_action_just_released("add_item_a"):
		#main_inventory.add(item_wood, 1)
	#if Input.is_action_just_released("remove_item_a"):
		#main_inventory.remove(item_wood, 1)
#
	#if Input.is_action_just_released("add_item_b"):
		#main_inventory.add(item_stone, 1)
	#if Input.is_action_just_released("remove_item_b"):
		#main_inventory.remove(item_stone, 1)
##
	#if Input.is_action_just_released("add_item_c"):
		#main_inventory.add(item_grass, 1)
	#if Input.is_action_just_released("remove_item_c"):
		#main_inventory.remove(item_grass, 1)
