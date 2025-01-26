class_name HotbarUI
extends Node

## [PackedScene] which is used to instantiate [SlotUI] for each slot added in [Hotbar]
@export var slot_ui_scene: PackedScene

## Parent [Control] for the [SlotUI] instances
@export var slots_container: Container

var hotbar: Hotbar

var ui_stacks: Array[Panel]


## Defines an [Hotbar] linked to this hotbar UI.
## This function disconnects signals from the previous [Inventory] and connects signals with new [Inventory]
func set_hotbar(new_hotbar : Hotbar):
	if new_hotbar != self.hotbar:
		if self.hotbar != null:
			self.hotbar.on_change_selection.disconnect(_on_changed_selection)
		self.hotbar = new_hotbar
		self.hotbar.on_change_selection.connect(_on_changed_selection)
		self.hotbar.equipped.connect(_on_equipped_stack)
		self.hotbar.unequipped.connect(_on_equipped_stack)
		_update_slots()
		_on_changed_selection(hotbar.selection_index)
		


func _on_changed_selection(_selection_index):
	_update_selection()


func _update_selection():
	if hotbar == null:
		return
	for i in hotbar.max_slots:
		var ui_stack = ui_stacks[i]
		ui_stack.set_selection(i == hotbar.selection_index)


func _on_equipped_stack(_slot_index: int):
	_update_slots()


func _update_slots():
	for ui_stack in ui_stacks:
		ui_stack.queue_free()
		
	ui_stacks.clear()
	
	for i in hotbar.max_slots:
		var stack = hotbar.get_stack_on_slot(i)
		var stack_ui = slot_ui_scene.instantiate()
		slots_container.add_child(stack_ui)
		stack_ui.update_info_with_stack(hotbar.database, stack, i)
		stack_ui.visible = hotbar.is_active_slot(i)
		ui_stacks.append(stack_ui)
	
	_update_selection()
