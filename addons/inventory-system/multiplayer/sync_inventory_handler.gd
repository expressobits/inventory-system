extends Node
class_name SyncInventoryHandler

@export var inventory_handler : InventoryHandler

func _ready() -> void:
	inventory_handler.updated_transaction_slot.connect(on_updated_transaction_slot.bind())
	inventory_handler.opened.connect(on_opened.bind())
	inventory_handler.closed.connect(on_closed.bind())

	
func on_updated_transaction_slot():
	if not multiplayer.is_server():
		return
	var item_id : int
	if not inventory_handler.transaction_slot.has_valid():
		item_id = ItemDefinition.NONE
	else:
		item_id = inventory_handler.transaction_slot.item.definition.id
	updated_transaction_slot_rpc.rpc(item_id, inventory_handler.transaction_slot.amount)


func on_opened(inventory : Inventory):
	if not multiplayer.is_server():
		return
	opened_rpc.rpc(inventory_handler.get_path_to(inventory))


func on_closed(inventory : Inventory):
	if not multiplayer.is_server():
		return
	closed_rpc.rpc(inventory_handler.get_path_to(inventory))

@rpc
func updated_transaction_slot_rpc(item_id : int, amount : int):
	var definition = inventory_handler.database.get_item(item_id)
	var item = Item.new()
	item.definition = definition
	inventory_handler.change_transaction_slot(item, amount);


@rpc
func opened_rpc(inventory_path : NodePath):
	var inventory = inventory_handler.get_node(inventory_path)
	inventory_handler.opened_inventories.append(inventory_handler.get_path_to(inventory))
	inventory_handler.opened.emit(inventory)


@rpc
func closed_rpc(inventory_path : NodePath):
	var inventory = inventory_handler.get_node(inventory_path)
	var index = inventory_handler.opened_inventories.find(inventory_handler.get_path_to(inventory))
	if index == -1:
		return
	inventory_handler.opened_inventories.remove_at(index)
	inventory_handler.closed.emit(inventory)
