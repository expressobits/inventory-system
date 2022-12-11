extends Node3D

var inventory_path := NodePath("Inventory")
@onready var inventory: Inventory = get_node(inventory_path)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_released("Add item"):
		inventory.add(1, 1)
	if Input.is_action_just_released("Remove item"):
		inventory.remove(1, 1)
		
	if Input.is_action_just_released("Add item B"):
		inventory.add(2, 1)
	if Input.is_action_just_released("Remove item B"):
		inventory.remove(2, 1)
		
	if Input.is_action_just_released("Has"):
		print(inventory.has(2))
	if Input.is_action_just_released("GetAmountOf"):
		print(inventory.get_amount_of(2))

func _on_inventory_inventory_changed():
	print("Inventory")
	print(inventory.slots)
	print("\n")
