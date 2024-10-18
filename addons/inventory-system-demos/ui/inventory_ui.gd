class_name InventoryUI
extends Control

## Visually represent an [Inventory],
## use the [PackedScene] of SlotUI to create visual slots by inventory add signal

## Emitted when a [SlotUI] is clicked
signal slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory)

## Emitted when [InventoryUI] background is clicked and not a [SlotUI]
signal inventory_point_down(event: InputEvent, inventory : Inventory)

## Linked [Inventory]
@export var inventory : Inventory

## [PackedScene] which is used to instantiate [SlotUI] for each slot added in [Inventory]
@export var slot_ui_scene : PackedScene

## Parent [Control] for the [SlotUI] instances
@export var slots_container : Container

@export var console_mode : bool

## List of [SlotUI] representing each [Inventory] slot
var ui_stacks : Array[SlotUI]




func _ready():
	gui_input.connect(_on_inventory_gui_input)
#	gui_input.connect(_on_inventory_gui_input)
	if inventory != null:
		_connect_new_inventory(inventory)

## Defines an [Inventory] linked to this inventory UI.
## This function disconnects signals from the previous [Inventory] and connects signals with new [Inventory]
func set_inventory(inventory : Inventory):
	if inventory != self.inventory:
		if self.inventory != null:
			_disconnect_old_inventory()
		self.inventory = inventory
		_connect_new_inventory(inventory)
		%TitleLabel.text = inventory.inventory_name


func _disconnect_old_inventory():
	self.inventory.updated_stack.disconnect(_on_updated_stack)
	self.inventory.stack_added.disconnect(_on_stack_added)
	self.inventory.stack_removed.disconnect(_on_stack_removed)
	self.inventory = null


func _connect_new_inventory(inventory : Inventory):
	inventory.updated_stack.connect(_on_updated_stack)
	inventory.stack_added.connect(_on_stack_added)
	inventory.stack_removed.connect(_on_stack_removed)
	_update_slots()


func _update_slots():
	for ui_stack in ui_stacks:
		ui_stack.queue_free()
		
	ui_stacks.clear()
		
	for stack in inventory.items:
		var stack_obj = slot_ui_scene.instantiate()
		stack_obj.gui_input.connect(_on_slot_gui_input.bind(stack_obj))
		slots_container.add_child(stack_obj)
		ui_stacks.append(stack_obj)
		stack_obj.update_info_with_stack(inventory.database, stack)
		
	if not console_mode:
		return

	## Set focus neighbors
	for stack_index in ui_stacks.size():
		var ui_stack = ui_stacks[stack_index]
		for neighbor in ["left", "top", "right", "bottom"]:
			var neighbor_idx: int
			match neighbor:
				"left":
					neighbor_idx = stack_index - 1
					if stack_index % slots_container.columns == 0:
						neighbor_idx = -1
				"top":
					neighbor_idx = stack_index - slots_container.columns
				"right":
					neighbor_idx = stack_index + 1
					if (stack_index + 1) % slots_container.columns == 0:
						neighbor_idx = -1
				"bottom":
					neighbor_idx = stack_index + slots_container.columns
			if neighbor_idx >= 0 and neighbor_idx < ui_stacks.size():
				ui_stack.set("focus_neighbor_"+neighbor, ui_stacks[neighbor_idx].get_path())


func _on_updated_stack(index):
	ui_stacks[index].update_info_with_stack(inventory.database, inventory.items[index])


func _on_stack_added(index):
	var stack_obj = slot_ui_scene.instantiate()
	slots_container.add_child(stack_obj)
	ui_stacks.insert(index, stack_obj)
	stack_obj.gui_input.connect(_on_slot_gui_input.bind(stack_obj))


func _on_stack_removed(index):
	ui_stacks[index].queue_free()
	ui_stacks.remove_at(index)


func _on_slot_gui_input(event : InputEvent, ui_stack):
	if event is InputEventMouseButton or (console_mode and event.is_action("ui_accept")):
		if event.pressed:	
			var index = ui_stacks.find(ui_stack)
			if index < 0:
				return 
			slot_point_down.emit(event, index, inventory)


func _on_inventory_gui_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			inventory_point_down.emit(event, inventory)
