extends Inventory
class_name NetworkedInventory


func _ready():
	multiplayer.peer_connected.connect(_on_connected.bind())
	super._ready()
	slot_added.connect(_on_slot_added.bind())
	updated_slot.connect(_on_updated_slot.bind())
	slot_removed.connect(_on_slot_removed.bind())
	opened.connect(_on_opened.bind())
	closed.connect(_on_closed.bind())


func _on_connected(id):
	if not multiplayer.is_server():
		return
	if is_open:
		opened_rpc.rpc_id(id)
 

func _on_opened():
	if not multiplayer.is_server():
		return
	opened_rpc.rpc()


func _on_closed():
	if not multiplayer.is_server():
		return
	closed_rpc.rpc()


func _on_slot_added(slot_index : int):
	if not multiplayer.is_server():
		return
	slot_added_rpc.rpc(slot_index)


func _on_updated_slot(slot_index : int):
	if not multiplayer.is_server():
		return
	var item_id = slots[slot_index].item_id
	var item = database.get_item(item_id)
	var amount = slots[slot_index].amount
	updated_slot_rpc.rpc(slot_index, item_id, amount)


func _on_slot_removed(slot_index : int):
	if not multiplayer.is_server():
		return
	slot_removed_rpc.rpc(slot_index)


@rpc
func opened_rpc():
	if multiplayer.is_server():
		return
	is_open = true
	emit_signal("opened")


@rpc
func closed_rpc():
	if multiplayer.is_server():
		return
	is_open = false
	emit_signal("closed")


@rpc
func slot_added_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	add_slot(slot_index)


@rpc
func updated_slot_rpc(slot_index : int, item_id : int, amount : int):
	if multiplayer.is_server():
		return
	var item = database.get_item(item_id)
	set_slot(slot_index, item, amount)


@rpc
func slot_removed_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	remove_slot(slot_index)
