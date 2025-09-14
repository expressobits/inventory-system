class_name HotbarUI
extends Control

## [PackedScene] which is used to instantiate [SlotUI] for each slot added in [Hotbar]
@export var slot_ui_scene: PackedScene

## Parent [Control] for the [SlotUI] instances
@export var slots_container: Container

var hotbar: Hotbar

var ui_stacks: Array[Panel]
# Group of slot indices for each UI (same order as ui_stacks)
var ui_stack_groups: Array[PackedInt32Array] = []


## Defines an [Hotbar] linked to this hotbar UI.
## This function disconnects signals from the previous [Inventory] and connects signals with new [Inventory]
func set_hotbar(new_hotbar: Hotbar):
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
	# Highlight only the grouped UI that contains the current selection index
	for i in ui_stacks.size():
		var ui_stack: Panel = ui_stacks[i]
		var group: PackedInt32Array = ui_stack_groups[i]
		var selected := group.has(hotbar.selection_index)
		if ui_stack.has_method("set_selection"):
			ui_stack.set_selection(selected)


func _on_equipped_stack(_slot_index: int):
	_update_slots()


func _update_slots():
	# Clear previous UI
	for ui_stack in ui_stacks:
		ui_stack.queue_free()
	ui_stacks.clear()
	ui_stack_groups.clear()

	if hotbar == null:
		return

	# Map stacks to all slot indices they occupy (group duplicates)
	var stack_groups := { }  # key: instance_id (int) or "empty:<slot>" string; value: PackedInt32Array
	for i in hotbar.max_slots:
		var stack := hotbar.get_stack_on_slot(i)
		if stack != null and stack.has_valid():
			var key := str(stack.get_instance_id())
			if not stack_groups.has(key):
				stack_groups[key] = PackedInt32Array()
			stack_groups[key].append(i)
		else:
			# Keep empty/inactive slots as unique entries so UI still shows them
			var key_empty := "empty:%d" % i
			stack_groups[key_empty] = PackedInt32Array([i])

	# Create one UI per group (first slot is the primary index)
	for key in stack_groups.keys():
		var indices: PackedInt32Array = stack_groups[key]
		var primary_index := indices[0]
		var stack := hotbar.get_stack_on_slot(primary_index)

		var stack_ui: HotbarSlotUI = slot_ui_scene.instantiate()
		slots_container.add_child(stack_ui)

		stack_ui.update_info_with_stack(hotbar.database, stack, hotbar.get_inventory(), indices)

		# Visible if any covered slot is active (or at least the primary is active)
		var any_active := false
		for idx in indices:
			if hotbar.is_active_slot(idx):
				any_active = true
				break
		stack_ui.visible = any_active

		ui_stacks.append(stack_ui)
		ui_stack_groups.append(indices)

	_update_selection()
