extends Node
class_name	SyncHotbar

@export var hotbar : Hotbar

func _ready() -> void:
	#multiplayer.peer_connected.connect(_on_connected)
	hotbar.equipped.connect(_on_equipped_stack_changed)
	hotbar.unequipped.connect(_on_unequipped_stack_changed)



func _on_equipped_stack_changed(slot_index: int):
	if not multiplayer.is_server():
		return
	var stack = hotbar.get_selected_stack()
	var stack_index = hotbar.get_inventory().stacks.find(stack)
	equipped_stack_changed_rpc.rpc(stack_index, slot_index)


func _on_unequipped_stack_changed(slot_index: int):
	if not multiplayer.is_server():
		return
	unequipped_stack_changed_rpc.rpc(slot_index)


#func _on_connected(peer_id : int):
	#if not multiplayer.is_server():
		#return
	#
#
#
#func on_change_selection(selection_index : int):
#if not multiplayer.is_server():
	#return
#change_selection_rpc.rpc(selection_index)
#

@rpc
func equipped_stack_changed_rpc(stack_index : int, slot_index: int):
	var stack: ItemStack = null
	if stack_index >= 0 and stack_index < hotbar.get_inventory().stacks.size():
		stack = hotbar.get_inventory().stacks[stack_index]
	hotbar.equip(stack, slot_index)


@rpc
func unequipped_stack_changed_rpc(slot_index: int):
	hotbar.unequip(slot_index)
