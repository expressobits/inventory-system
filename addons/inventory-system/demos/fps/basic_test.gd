extends Node3D

var inventory_path := NodePath("Inventory")
@onready var inventory: Inventory = get_node(inventory_path)

@export var item_wood : Item
@export var item_metal : Item


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	if Input.is_action_just_released("Add item"):
		inventory.add(item_wood)
	if Input.is_action_just_released("Remove item"):
		inventory.remove(item_wood, 2)
		
	if Input.is_action_just_released("Add item B"):
		inventory.add(item_metal)
	if Input.is_action_just_released("Remove item B"):
		inventory.remove(item_metal, 2)
		
	if Input.is_action_just_released("Has"):
		print(inventory.contains(item_metal))
		$"CanvasLayer/Inventory System UI/Inventory UI"._update_slots()
	if Input.is_action_just_released("GetAmountOf"):
		print(inventory.get_amount_of(item_metal))

