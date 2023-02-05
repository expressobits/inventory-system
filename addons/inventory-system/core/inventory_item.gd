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

## Maximum amount of this item within an [Inventory] slot
@export var max_stack := 0

## Name of item
@export var name : String

## Item icon in texture2D, displayed by [SlotUI]
@export var icon : Texture2D
