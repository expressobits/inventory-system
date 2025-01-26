class_name BoxInventory
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@onready var inventory : Inventory = $Inventory
@export var actions : Array
@onready var openable : Openable = $Openable
@export var constraints : Array[GridInventoryConstraint]

func _ready():
	openable.closed.connect(_on_openable_closed)
	openable.opened.connect(_on_openable_opened)
	inventory.grid_constraints = constraints

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


func get_interact_actions(_interactor : Interactor) -> Array:
	if openable.is_open:
		return []
	return actions


func interact(character : Node, _action_index : int = 0):
	if !openable.is_open:
		character.character_inventory_system.open_inventory(inventory)
		open(character)


func open(character : Node):
	openable.open(character)


func close(character : Node):
	openable.close(character)


func _on_openable_opened(_character: Node) -> void:
	_on_open()


func _on_openable_closed(_character: Node) -> void:
	_on_close()
