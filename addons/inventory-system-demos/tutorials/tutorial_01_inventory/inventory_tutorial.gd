extends Node3D

@onready var inventory : Inventory = $Inventory

@export var item_id : String

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("interact"):
		print_inv()
	if Input.is_action_just_pressed("add_item_a"):
		var amount = inventory.add_stack("wood")
		print_inv()
	if Input.is_action_just_pressed("remove_item_a"):
		var amount = inventory.remove_stack("wood")
		print_inv()
	if Input.is_action_just_pressed("add_item_b"):
		var amount = inventory.add_stack("stone")
		print_inv()
	if Input.is_action_just_pressed("remove_item_b"):
		var amount = inventory.remove_stack("stone")
		print_inv()

func print_inv():
	print("---- INV ---")
	print("Slots:")
	for slot in inventory.slots:
		print(slot.stack_index)
	print("Stacks:")
	for stack in inventory.stacks:
		print(stack.serialize())
