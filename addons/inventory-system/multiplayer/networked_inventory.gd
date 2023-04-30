@tool
extends Inventory
class_name NetworkedInventory

## Networked version of inventory where server sends rpcs to client for 
## slot update, add and remove signals
## 
## Why not use [MultiplayerSyncronizer]?
## The idea of using rpc signals only when changed saves a lot of bandwidth, 
## but at the cost of being sure which signals will be called, ie calling 
## slot[i] = new Dictionary is not replicated across the network.
## Also keep in mind that signals need to be handled if switching to a use of
## MultiplayerSyncronizer

func _ready():
	super._ready()
	multiplayer.peer_connected.connect(_on_connected.bind())
	slot_added.connect(_on_slot_added.bind())
	updated_slot.connect(_on_updated_slot.bind())
	slot_removed.connect(_on_slot_removed.bind())
	opened.connect(_on_opened.bind())
	closed.connect(_on_closed.bind())


func _on_connected(id):
	if not multiplayer.is_server():
		return
	if is_open:
		_opened_rpc.rpc_id(id)
#	_update_slots_rpc.rpc_id(id, slots)
 

func _on_opened():
	if not multiplayer.is_server():
		return
	_opened_rpc.rpc()


func _on_closed():
	if not multiplayer.is_server():
		return
	_closed_rpc.rpc()


func _on_slot_added(slot_index : int):
	if not multiplayer.is_server():
		return
	_slot_added_rpc.rpc(slot_index)


func _on_updated_slot(slot_index : int):
	if not multiplayer.is_server():
		return
	var item = slots[slot_index].item
	var item_id : int
	if item == null:
		item_id = InventoryItem.NONE
	else:
		item_id = item.id
	var amount = slots[slot_index].amount
	_updated_slot_rpc.rpc(slot_index, item_id, amount)


func _on_slot_removed(slot_index : int):
	if not multiplayer.is_server():
		return
	_slot_removed_rpc.rpc(slot_index)


@rpc
func _update_slots_rpc(slots : Array[Slot]):
	self.slots = slots


@rpc
func _opened_rpc():
	if multiplayer.is_server():
		return
	is_open = true
	emit_signal("opened")


@rpc
func _closed_rpc():
	if multiplayer.is_server():
		return
	is_open = false
	emit_signal("closed")


@rpc
func _slot_added_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	_add_slot(slot_index)


@rpc
func _updated_slot_rpc(slot_index : int, item_id : int, amount : int):
	if multiplayer.is_server():
		return
	var item = get_item_from_id(item_id)
	set_slot(slot_index, item, amount)


@rpc
func _slot_removed_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	_remove_slot(slot_index)
