class_name VisualInventory
extends Node3D

@export var visual_slots_paths : Array[NodePath]
@export var visual_slots : Array[VisualSlot3D]
@export var property_name_of_visual = "visual_3d"
@onready var inventory = $"../Inventory"


func _ready():
	for path in visual_slots_paths:
		var visual_slot : VisualSlot3D = get_node(path)
		visual_slots.append(visual_slot)
	for i in visual_slots.size():
		var visual_slot = visual_slots[i]
		visual_slot.setup(inventory, i, property_name_of_visual)
