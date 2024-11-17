extends Node
class_name	SyncHotbar

@export var hotbar : Hotbar

func _ready() -> void:
	multiplayer.peer_connected.connect(_on_connected)
	hotbar.equipped.connect(_on_equipped_stack_changed)
	hotbar.unequipped.connect(_on_unequipped_stack_changed)
	hotbar.on_change_selection.connect(_on_change_selection)



func _on_equipped_stack_changed(slot_index: int):
	if not multiplayer.is_server():
		return
	var stack = hotbar.get_stack_on_slot(slot_index)
	var stack_index = hotbar.get_inventory().stacks.find(stack)
	equipped_stack_changed_rpc.rpc(stack_index, slot_index)


func _on_unequipped_stack_changed(slot_index: int):
	if not multiplayer.is_server():
		return
	unequipped_stack_changed_rpc.rpc(slot_index)


func _on_connected(peer_id : int):
	if not multiplayer.is_server():
		return
	change_selection_rpc.rpc_id(peer_id, hotbar.selection_index)
	for i in hotbar.max_slots:
		var stack = hotbar.get_stack_on_slot(i)
		if stack == null:
			continue
		var stack_index = hotbar.get_inventory().stacks.find(stack)
		equipped_stack_changed_rpc.rpc_id(peer_id,stack_index, i)


func _on_change_selection(selection_index : int):
	if not multiplayer.is_server():
		return
	change_selection_rpc.rpc(selection_index)


@rpc
func equipped_stack_changed_rpc(stack_index : int, slot_index: int):
	var stack: ItemStack = null
	if stack_index >= 0 and stack_index < hotbar.get_inventory().stacks.size():
		stack = hotbar.get_inventory().stacks[stack_index]
	hotbar.equip(stack, slot_index)


@rpc
func unequipped_stack_changed_rpc(slot_index: int):
	hotbar.unequip(slot_index)


@rpc
func change_selection_rpc(selection_index: int):
	hotbar.selection_index = selection_index
