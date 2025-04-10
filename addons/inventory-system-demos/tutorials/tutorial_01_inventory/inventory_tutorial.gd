extends Node3D

@onready var inventory : Inventory = $Inventory

@export var item_id : String

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("interact"):
		print_inventory()
	if Input.is_action_just_pressed("add_item_a"):
		inventory.add(item_id, 1)
		print_inventory()
	if Input.is_action_just_pressed("remove_item_a"):
		inventory.remove(item_id, 1)
		print_inventory()

func print_inventory():
	print("Inventory Items:")
	for item in inventory.stacks:
		if item.item_id != "":
			print(item.item_id," x ", item.amount)
		else:
			print("Empty")
