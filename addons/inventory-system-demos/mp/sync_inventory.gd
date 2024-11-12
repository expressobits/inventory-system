class_name SyncInventory
extends Node

@export var sync_item_added_signal := true
@export var sync_item_removed_signal := true

@export var inventory : GridInventory

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
	#inventory.stack_added.connect(_on_stack_added)
	#inventory.updated_stack.connect(_on_updated_stack)
	#inventory.stack_removed.connect(_on_stack_removed)
	inventory.contents_changed.connect(_on_contents_changed)
	#if sync_item_added_signal:
		#inventory.item_added.connect(_on_item_added)
	#if sync_item_removed_signal:
		#inventory.item_removed.connect(_on_item_removed)


func _on_contents_changed():
	if not multiplayer.is_server():
		return
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc(inv_data)


func _on_connected(id):
	if not multiplayer.is_server():
		return
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc_id(id, inv_data)


func _on_stack_added(stack_index : int):
	if not multiplayer.is_server():
		return
	
	_stack_added_rpc.rpc(stack_index)


func _on_updated_stack(stack_index : int):
	if not multiplayer.is_server():
		return
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc(inv_data)
	#_updated_slot_rpc.rpc(inventory.stacks[stack_index].serialize())


func _on_stack_removed(stack_index : int):
	if not multiplayer.is_server():
		return
	
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc(inv_data)
	#_stack_removed_rpc.rpc(stack_index)


func _on_item_added(item_id : String, amount : int):
	if not multiplayer.is_server():
		return
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc(inv_data)
	#_item_added_rpc.rpc(item_id, amount)


func _on_item_removed(item_id : String, amount : int):
	if not multiplayer.is_server():
		return
	var inv_data = inventory.serialize()
	_update_inventory_rpc.rpc(inv_data)
	#_item_removed_rpc.rpc(item_id, amount)


@rpc
func _update_inventory_rpc(inv_data : Dictionary):
	if not multiplayer.is_server():
		inventory.deserialize(inv_data)
		inventory.contents_changed.emit()


@rpc
func _stack_added_rpc(stack_index : int):
	if multiplayer.is_server():
		return
	#inventory.insert_stack(stack_index)


#@rpc
#func _updated_slot_rpc(stack_index : int, slot_data : Array):
	#if multiplayer.is_server():
		#return
	#var stack : ItemStack = inventory.items[stack_index]
	#stack.deserialize(slot_data)
	#inventory.updated_stack.emit(stack_index)

@rpc
func _stack_removed_rpc(stack_index : int):
	if multiplayer.is_server():
		return
	inventory.remove_stack_at(stack_index)


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
