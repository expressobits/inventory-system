extends Control

@export var inventory : Inventory
@export var slot_ui_scene : PackedScene

@export var slots_container : GridContainer

var slots : Array

func _ready():
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
		slots_container.add_child(slot_obj)
		slots.append(slot_obj)
		slot_obj.update_info(slot)

func on_inventory_changed():
	_update_slots()
	
func _on_updated_slot(index):
	slots[index].update_info(inventory.slots[index])
	
func _on_slot_added(index):
	var slot_obj = slot_ui_scene.instantiate()
	slots_container.add_child(slot_obj)
	slots.append(slot_obj)
	
func _on_slot_removed(index):
	slots[index].queue_free()
	slots.remove_at(index)
