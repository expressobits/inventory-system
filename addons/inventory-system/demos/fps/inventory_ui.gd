extends Control

@export var inventory : Inventory
@export var slot_ui_scene : PackedScene

@export var slots_container : GridContainer

var slots : Array

func _ready():
	inventory.inventory_changed.connect(on_inventory_changed.bind())
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

func _process(delta):
	pass
