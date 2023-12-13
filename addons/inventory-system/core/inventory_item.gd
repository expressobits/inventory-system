@icon("res://addons/inventory-system/icons/inventory_item.svg")
extends Resource
class_name InventoryItem
## Definition of an inventory item
##
## This information will be stored in slots within the [Inventory]
## The [InventoryDatabase] must be used to link items with an identifier.
## This script is meant to be expanded to contain new information, example:
## FoodItem containing information about how much food satisfies hunger
## Usage:
## [codeblock]
## 	extends InventoryItem
## 	class_name FoodItem
## 	
## 	@export var satisfies_hunger = 12
## [/codeblock]

@export var id := 0

@export var can_stack := true

## Maximum amount of this item within an [Inventory] slot
@export var max_stack := 0:
	get:
		if can_stack:
			return max_stack
		else:
			return 1

## Name of item
@export var name : String

## Item icon in texture2D, displayed by [SlotUI]
@export var icon : Texture2D

## Item weight in float
@export var weight : float

## Item custom properties
@export var properties : Dictionary

## TODO Use this on craft finish with as option
## Custom Properties that are instantiated in the [SlotItem] to be used dynamically. Example: current durability
@export var dynamic_properties : Array[String]

## Item Categories (Use Bit Flags)
@export var categories : Array[ItemCategory]

## Id represents none item (Used in networked scripts)
const NONE = -1


func contains_category(category : ItemCategory) -> bool:
	for c in categories:
		if c == category:
			return true
	return false
