extends InventoryHandler
class_name NetworkedHandler

func _ready():
	if multiplayer.is_server():
		updated_transaction_slot.connect(_on_updated_transaction_slot.bind())


func _on_updated_transaction_slot(item : InventoryItem, amount : int):
	var item_id = database.get_id_from_item(item)
	if item_id > 0:
		_on_updated_transaction_slot_rpc.rpc(item_id, amount)


@rpc
func _on_updated_transaction_slot_rpc(item_id : int, amount : int):
	var item = database.get_item(item_id)
	if item != null:
		_set_transaction_slot(item, amount)


func drop(item : InventoryItem, amount := 1) -> bool:
	var item_id = database.get_id_from_item(item)
	if item_id <= 0:
		return false
	if not multiplayer.is_server():
		drop_rpc.rpc_id(1, item_id, amount)
	else:
		drop_rpc(item_id, amount)
	return true


func open(inventory : Inventory) -> bool:
	if not multiplayer.is_server():
		open_rpc.rpc_id(1, inventory.get_path())
		emit_signal("opened", inventory)
	else:
		open_rpc(inventory.get_path())
	return true


func close(inventory : Inventory) -> bool:
	if not multiplayer.is_server():
		close_rpc.rpc_id(1, inventory.get_path())
	else:
		close_rpc(inventory.get_path())
	return true


func close_all_inventories():
	if not multiplayer.is_server():
		close_all_inventories_rpc.rpc_id(1)
	else:
		close_all_inventories_rpc()
	return true


func to_transaction(slot_index : int , inventory : Inventory, amount : int):
	if not multiplayer.is_server():
		to_transaction_rpc.rpc_id(1, slot_index, inventory.get_path(), amount)
	else:
		to_transaction_rpc(slot_index, inventory.get_path(), amount)


func transaction_to_at(slot_index : int, inventory : Inventory):
	if not multiplayer.is_server():
		transaction_to_at_rpc.rpc_id(1, slot_index, inventory.get_path())
	else:
		transaction_to_at_rpc(slot_index, inventory.get_path())


func transaction_to(inventory : Inventory):
	if not multiplayer.is_server():
		transaction_to_rpc.rpc_id(1, inventory.get_path())
	else:
		transaction_to_rpc(inventory.get_path())


# Commands to server from client

@rpc("any_peer")
func drop_rpc(item_id : int, amount : int):
	if not multiplayer.is_server():
		return
	var item = database.get_item(item_id)
	if item == null:
		return
	var id = multiplayer.get_remote_sender_id()
	super.drop(item, amount)


@rpc("any_peer")
func open_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	var id = multiplayer.get_remote_sender_id()
	if super.open(inventory) and id > 1:
		_on_open_rpc.rpc_id(multiplayer.get_remote_sender_id(), object_path)


@rpc("any_peer")
func to_transaction_rpc(slot_index : int, object_path : NodePath, amount : int):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.to_transaction(slot_index, inventory, amount)


@rpc("any_peer")
func transaction_to_at_rpc(slot_index : int, object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.transaction_to_at(slot_index, inventory)


@rpc("any_peer")
func transaction_to_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.transaction_to(inventory)


@rpc("any_peer")
func close_all_inventories_rpc():
	super.close_all_inventories()


@rpc("any_peer")
func close_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.close(inventory)


# Responses from server to client

@rpc
func _on_open_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	emit_signal("opened", inventory)


## Returns [code]true[/code] if main [Inventory] is open.
func is_open_main_inventory() -> bool:
	return super.is_open(inventory)


## Open main [Inventory]. Return [code]true[/code] if opened successfully.
func open_main_inventory() -> bool:
	return super.open(inventory)


## Open main [Inventory]. Return [code]true[/code] if opened successfully.
func close_main_inventory() -> bool:
	return super.close(inventory)
