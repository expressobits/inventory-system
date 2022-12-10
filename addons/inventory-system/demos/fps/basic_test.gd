extends Node3D

var inventory_path := NodePath("Inventory")
@onready var inventory: Inventory = get_node(inventory_path)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	inventory.add(1, 1)
	print(inventory.slots)
