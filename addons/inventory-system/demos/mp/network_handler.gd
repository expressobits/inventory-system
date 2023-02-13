extends Node
class_name NetworkHandler

@export_node_path("InventoryHandler") var inventory_handler_path = NodePath("InventoryHandler")
@onready var inventory_handler : InventoryHandler = get_node(inventory_handler_path)

@export var sync_opened_signal := true
@export var only_owner_response_signal := true

func _enter_tree():
	set_multiplayer_authority(1)


func _ready():
	if is_multiplayer_authority():
		inventory_handler.opened.connect(_on_opened.bind())


func _on_opened(inventory : Inventory):
	if not is_multiplayer_authority():
		return
	if not sync_opened_signal:
		return
	_on_open_rpc.rpc(inventory)


@rpc
func _on_open_rpc(inventory : Inventory):
	if not multiplayer.is_multiplayer_authority():
		return
	inventory_handler.emit_signal("opened", inventory)
