class_name BoxInventory
extends Node3D

@onready var inventory : Inventory = $Inventory
@export var actions : Array[InteractAction]
@onready var openable : Openable = $Openable

func _ready():
	openable.closed.connect(_on_openable_closed)
	openable.opened.connect(_on_openable_opened)

func get_inventory() -> Inventory:
	return $Inventory


func _on_inventory_opened():
	_on_open()


func _on_open():
	$box.visible = false
	$boxOpen.visible = true
	$Open.play()


func _on_close():
	$box.visible = true
	$boxOpen.visible = false
	$Close.play()


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array[InteractAction]:
	if inventory.is_open:
		return []
	return actions


func interact(character : Node, _action_index : int = 0):
	character.character_inventory_system.open_inventory(inventory)
	open(character)


func open(character : Node):
	openable.open(character)

func close(character : Node):
	print("close")
	openable.close(character)

func _on_openable_opened(character: Node) -> void:
	_on_open()


func _on_openable_closed(character: Node) -> void:
	_on_close()
