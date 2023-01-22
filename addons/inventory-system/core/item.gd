@icon("res://addons/inventory-system/icons/box-svgrepo-com.svg")
extends Resource
class_name Item

@export var max_stack := 0
@export var name : String
@export var icon : Texture2D
@export var pickable_item : PackedScene
