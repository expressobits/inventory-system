@tool
extends Inventory
class_name NetworkedInventory

@export var sync_item_added_signal := true
@export var sync_item_removed_signal := true

## Networked version of inventory where server sends rpcs to client for 
## slot update, add and remove signals
## 
## Why not use [MultiplayerSyncronizer]?
## The idea of using rpc signals only when changed saves a lot of bandwidth, 
## but at the cost of being sure which signals will be called, ie calling 
## slot[i] = new Dictionary is not replicated across the network.
## Also keep in mind that signals need to be handled if switching to a use of
## MultiplayerSyncronizer
##
## Note: Slot categories are not synced

var slots_sync : Array:
	set(value):
		slots_sync = value
		if not multiplayer.is_server():
			for i in range(slots_sync.size(), slots.size()):
				slots.remove_at(i)
			for i in slots_sync.size():
				if i >= slots.size():
					var slot = Slot.new()
					slot.item = SlotItem.new()
					slots.append(slot)
				slots[i].amount = slots_sync[i].amount
				var item = database.get_item(slots_sync[i].item_id)
				slots[i].item.definition = item


func _ready():
	super._ready()
	if Engine.is_editor_hint():
		return
	multiplayer.peer_connected.connect(_on_connected.bind())
	slot_added.connect(_on_slot_added.bind())
	updated_slot.connect(_on_updated_slot.bind())
	slot_removed.connect(_on_slot_removed.bind())
	if sync_item_added_signal:
		item_added.connect(_on_item_added.bind())
	if sync_item_removed_signal:
		item_removed.connect(_on_item_removed.bind())
	opened.connect(_on_opened.bind())
	closed.connect(_on_closed.bind())
	if multiplayer.is_server():
		slots_sync.clear()
		for i in slots.size():
			var slot = slots[i]
			slots_sync.append({"item_id" = slot.get_item_id() , "amount" = slot.amount})


func _on_connected(id):
	if not multiplayer.is_server():
		return
	slots_sync.clear()
	for i in slots.size():
		var slot = slots[i]
		slots_sync.append({"item_id" = slot.get_item_id() , "amount" = slot.amount})
	if is_open:
		_opened_rpc.rpc_id(id)
	_update_slots_rpc.rpc_id(id, slots_sync)
 

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
	var slot = slots[slot_index]
	var slot_dict = {"item_id" = InventoryItem.NONE , "amount" = 0}
	slots_sync.append(slot_dict)
	_slot_added_rpc.rpc(slot_index)


func _on_updated_slot(slot_index : int):
	if not multiplayer.is_server():
		return
	var item : SlotItem = slots[slot_index].item
	var item_id : int
	if item.definition == null:
		item_id = InventoryItem.NONE
	else:
		item_id = item.definition.id
	var amount = slots[slot_index].amount
	slots_sync[slot_index]["item_id"] = item_id
	slots_sync[slot_index]["amount"] = amount
	slots_sync[slot_index]["properties"] = item.properties
	_updated_slot_rpc.rpc(slot_index, item_id, amount, item.properties)


func _on_slot_removed(slot_index : int):
	if not multiplayer.is_server():
		return
	slots_sync.remove_at(slot_index)
	_slot_removed_rpc.rpc(slot_index)


func _on_item_added(item : SlotItem, amount : int):
	if not multiplayer.is_server():
		return
	_item_added_rpc.rpc(item.definition.id, amount)


func _on_item_removed(item : SlotItem, amount : int):
	if not multiplayer.is_server():
		return
	_item_removed_rpc.rpc(item.definition.id, amount)


@rpc
func _update_slots_rpc(slots_sync : Array):
	self.slots_sync = slots_sync


@rpc
func _opened_rpc():
	if multiplayer.is_server():
		return
	is_open = true
	opened.emit()


@rpc
func _closed_rpc():
	if multiplayer.is_server():
		return
	is_open = false
	closed.emit()


@rpc
func _slot_added_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	_add_slot(slot_index)


@rpc
func _updated_slot_rpc(slot_index : int, item_id : int, amount : int, properties : Dictionary):
	if multiplayer.is_server():
		return
	var item : InventoryItem = get_item_from_id(item_id)
	set_slot_content(slot_index, item, properties, amount)


@rpc
func _slot_removed_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	_remove_slot(slot_index)


@rpc
func _item_added_rpc(item_id : int, amount : int):
	if multiplayer.is_server():
		return
	var item = SlotItem.new()
	item.definition = database.get_item(item_id)
	if item.definition == null:
		return
	item_added.emit(item, amount)


@rpc
func _item_removed_rpc(item_id : int, amount : int):
	if multiplayer.is_server():
		return
	var item = SlotItem.new()
	item.definition = database.get_item(item_id)
	if item.definition == null:
		return
	item_removed.emit(item, amount)
