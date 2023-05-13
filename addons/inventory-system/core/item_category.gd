@tool
@icon("res://addons/inventory-system/icons/item_category.svg")
extends Resource
class_name ItemCategory

@export var name : String

@export var color := Color(1,1,1)

@export var icon : Texture2D

var code : int = 0
