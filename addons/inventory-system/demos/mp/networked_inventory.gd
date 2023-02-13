extends Node
class_name NetworkedInventory

@export_node_path("Inventory") var inventory_path = NodePath("..")
@onready var inventory: Inventory = get_node(inventory_path)


func _ready():
	multiplayer.peer_connected.connect(_on_connected.bind())

func _on_connected(id):
	if is_multiplayer_authority():
		inventory.opened.connect(_on_opened.bind())
		inventory.closed.connect(_on_closed.bind())


func _on_opened():
	opened_rpc.rpc()


func _on_closed():
	closed_rpc.rpc()


@rpc
func opened_rpc():
	if is_multiplayer_authority():
		return
	inventory.emit_signal("opened")
	
@rpc
func closed_rpc():
	if is_multiplayer_authority():
		return
	inventory.emit_signal("closed")
