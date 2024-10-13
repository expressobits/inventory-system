class_name SyncInventory
extends Node

@export var sync_item_added_signal := true
@export var sync_item_removed_signal := true

@export var inventory : Inventory

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

func _ready():
	if Engine.is_editor_hint():
		return
	multiplayer.peer_connected.connect(_on_connected)
	if(inventory != null):
		setup()


func setup():
	inventory.slot_added.connect(_on_slot_added)
	inventory.updated_slot.connect(_on_updated_slot)
	inventory.slot_removed.connect(_on_slot_removed)
	if sync_item_added_signal:
		inventory.item_added.connect(_on_item_added)
	if sync_item_removed_signal:
		inventory.item_removed.connect(_on_item_removed)
	

func _on_connected(id):
	if not multiplayer.is_server():
		return
	var slots_sync = inventory.serialize()
	_update_slots_rpc.rpc_id(id, slots_sync)


func _on_slot_added(slot_index : int):
	if not multiplayer.is_server():
		return
	
	_slot_added_rpc.rpc(slot_index)


func _on_updated_slot(slot_index : int):
	if not multiplayer.is_server():
		return
	_updated_slot_rpc.rpc(slot_index, inventory.database.serialize_slot(inventory.slots[slot_index]))


func _on_slot_removed(slot_index : int):
	if not multiplayer.is_server():
		return
	
	_slot_removed_rpc.rpc(slot_index)


func _on_item_added(item_id : String, amount : int):
	if not multiplayer.is_server():
		return
	_item_added_rpc.rpc(item_id, amount)


func _on_item_removed(item_id : String, amount : int):
	if not multiplayer.is_server():
		return
	_item_removed_rpc.rpc(item_id, amount)


@rpc
func _update_slots_rpc(slots_sync : Dictionary):
	if not multiplayer.is_server():
		inventory.deserialize(slots_sync)


@rpc
func _slot_added_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	inventory.insert_slot(slot_index)


@rpc
func _updated_slot_rpc(slot_index : int, slot_data : String):
	if multiplayer.is_server():
		return
	var slot : Slot = inventory.slots[slot_index]
	inventory.database.deserialize_slot(slot, slot_data)
	inventory.updated_slot.emit(slot_index)

@rpc
func _slot_removed_rpc(slot_index : int):
	if multiplayer.is_server():
		return
	inventory.remove_slot_at(slot_index)


@rpc
func _item_added_rpc(item_id : String, amount : int):
	if multiplayer.is_server():
		return
	inventory.item_added.emit(item_id, amount)


@rpc
func _item_removed_rpc(item_id : String, amount : int):
	if multiplayer.is_server():
		return
	inventory.item_removed.emit(item_id, amount)
