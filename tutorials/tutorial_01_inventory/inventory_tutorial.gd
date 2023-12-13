extends Node3D

@onready var inventory : Inventory = $Inventory

@export var item : Item

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("interact"):
		print("Inventory Slots:")
		for slot in inventory.slots:
			if slot.item != null:
				print(slot.item.definition.name," x ", slot.amount)
			else:
				print("Empty")
	if Input.is_action_just_pressed("add_item_a"):
		inventory.add(item, 1)
