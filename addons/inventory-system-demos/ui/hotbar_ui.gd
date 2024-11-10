class_name HotbarUI
extends Node

## [PackedScene] which is used to instantiate [SlotUI] for each slot added in [Hotbar]
@export var slot_ui_scene : PackedScene

## Parent [Control] for the [SlotUI] instances
@export var slots_container : Container

var hotbar : Hotbar


## Defines an [Hotbar] linked to this hotbar UI.
## This function disconnects signals from the previous [Inventory] and connects signals with new [Inventory]
func set_hotbar(hotbar : Hotbar):
	if hotbar != self.hotbar:
		if self.hotbar != null:
			self.hotbar.on_change_selection.disconnect(_on_changed_selection)
			self.hotbar.get_inventory().updated_stack.disconnect(_on_updated_stack)
		self.hotbar = hotbar
		self.hotbar.on_change_selection.connect(_on_changed_selection)
		self.hotbar.get_inventory().updated_stack.connect(_on_updated_stack)
		_update_stacks()
		_on_changed_selection(hotbar.selection_index)


func _on_changed_selection(selection_index):
	if hotbar == null:
		return
	#for i in min(hotbar.slots_in_hot_bar, hotbar.get_inventory().stacks.size()):
		#var ui_stack = ui_stacks[i]
		#ui_stack.set_selection(i == hotbar.selection_index)


func _on_updated_stack(index):
	return
	#if index < 0 or index >= ui_stacks.size():
		#return
	#ui_stacks[index].update_info_with_stack(hotbar.database, hotbar.get_inventory().stacks[index])


func _update_stacks():
	pass
	#for ui_stack in ui_stacks:
		#ui_stack.queue_free()
		
	#ui_stacks.clear()
		#
	#for i in min(hotbar.slots_in_hot_bar, hotbar.get_inventory().stacks.size()):
		#var stack = hotbar.get_inventory().items[i]
		#var stack_ui = slot_ui_scene.instantiate()
		#slots_container.add_child(stack_ui)
		#ui_stacks.append(stack_ui)
		#stack_ui.update_info_with_slot(hotbar.database, stack)
