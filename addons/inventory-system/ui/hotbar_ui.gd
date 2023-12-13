extends Node
class_name HotbarUI

## List of [SlotUI] representing each [Hotbar] slot
var slots : Array[SlotUI]

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
			self.hotbar.on_change_selection.disconnect(_on_changed_selection.bind())
			self.hotbar.inventory.updated_slot.disconnect(_on_updated_slot.bind())
		self.hotbar = hotbar
		self.hotbar.on_change_selection.connect(_on_changed_selection.bind())
		self.hotbar.inventory.updated_slot.connect(_on_updated_slot.bind())
		_update_slots()
		_on_changed_selection(hotbar.selection_index)


func _on_changed_selection(selection_index):
	if hotbar == null:
		return
	for i in min(hotbar.slots_in_hot_bar, hotbar.inventory.slots.size()):
		var slot = slots[i]
		slot.set_selection(i == hotbar.selection_index)


func _on_updated_slot(index):
	if index < 0 or index >= slots.size():
		return
	slots[index].update_info_with_slot(hotbar.inventory.slots[index])


func _update_slots():
	for slot in slots:
		slot.queue_free()
		
	slots.clear()
		
	for i in min(hotbar.slots_in_hot_bar, hotbar.inventory.slots.size()):
		var slot = hotbar.inventory.slots[i]
		var slot_obj = slot_ui_scene.instantiate()
		slots_container.add_child(slot_obj)
		slots.append(slot_obj)
		slot_obj.update_info_with_slot(slot)
