extends Control
class_name InventoryUI

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

## List of [SlotUI] representing each [Inventory] slot
var slots : Array[SlotUI]


func _ready():
	gui_input.connect(_on_inventory_gui_input.bind())
	$Control.gui_input.connect(_on_inventory_gui_input.bind())
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
		$Control/Label.text = inventory.inventory_name


func _disconnect_old_inventory():
	self.inventory.updated_slot.disconnect(_on_updated_slot.bind())
	self.inventory.slot_added.disconnect(_on_slot_added.bind())
	self.inventory.slot_removed.disconnect(_on_slot_removed.bind())
	self.inventory = null


func _connect_new_inventory(inventory : Inventory):
	inventory.updated_slot.connect(_on_updated_slot.bind())
	inventory.slot_added.connect(_on_slot_added.bind())
	inventory.slot_removed.connect(_on_slot_removed.bind())
	_update_slots()


func _update_slots():
	for slot in slots:
		slot.queue_free()
		
	slots.clear()
		
	for slot in inventory.slots:
		var slot_obj = slot_ui_scene.instantiate()
		slot_obj.gui_input.connect(_on_slot_gui_input.bind(slot_obj))
		slots_container.add_child(slot_obj)
		slots.append(slot_obj)
		slot_obj.update_info_with_slot(slot)

	## Set focus neighbors
	for slot_idx in slots.size():
		var neighbor_idxs = {left = slot_idx-1,top = slot_idx-slots_container.columns,right = slot_idx+1,bottom = slot_idx+slots_container.columns}
		var slot = slots[slot_idx]
		for neighbor in neighbor_idxs.keys():
			var nb_idx = neighbor_idxs[neighbor]
			if nb_idx >= 0 and nb_idx < slots.size():
				match neighbor:
					"left":
						if slot_idx % slots_container.columns != 0:
							slot.focus_neighbor_left = slots[nb_idx].get_path()
					"top":
						slot.focus_neighbor_top = slots[nb_idx].get_path()
					"right":
						if nb_idx % slots_container.columns != 0:
							slot.focus_neighbor_right = slots[nb_idx].get_path()
					"bottom":
						slot.focus_neighbor_bottom = slots[nb_idx].get_path()


func _on_updated_slot(index):
	slots[index].update_info_with_slot(inventory.slots[index])


func _on_slot_added(index):
	var slot_obj = slot_ui_scene.instantiate()
	slots_container.add_child(slot_obj)
	slots.insert(index, slot_obj)
	slot_obj.gui_input.connect(_on_slot_gui_input.bind(slot_obj))


func _on_slot_removed(index):
	slots[index].queue_free()
	slots.remove_at(index)


func _on_slot_gui_input(event : InputEvent, slot_obj):
	if event is InputEventMouseButton or event.is_action("ui_accept"):
		if event.pressed:	
			var index = slots.find(slot_obj)
			if index < 0:
				return 
			emit_signal("slot_point_down", event, index, inventory)


func _on_inventory_gui_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			emit_signal("inventory_point_down", event, inventory)
