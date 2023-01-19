extends Control
class_name InventoryUI

@export var inventory : Inventory
@export var slot_ui_scene : PackedScene

@export var slots_container : GridContainer

var slots : Array

signal slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory)
signal inventory_point_down(event: InputEvent, inventory : Inventory)

func _ready():
	$Control.gui_input.connect(_on_inventory_gui_input.bind())
	if inventory != null:
		connection(inventory)
	
func set_inventory(inventory : Inventory):
	if inventory != self.inventory:
		if self.inventory != null:
			self.inventory.updated_slot.disconnect(_on_updated_slot.bind())
			self.inventory.slot_added.disconnect(_on_slot_added.bind())
			self.inventory.slot_removed.disconnect(_on_slot_removed.bind())
		self.inventory = inventory
		connection(inventory)
	
func connection(inventory : Inventory):
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
		slot_obj.update_info(slot)

func on_inventory_changed():
	_update_slots()
	
func _on_updated_slot(index):
	slots[index].update_info(inventory.slots[index])
	
func _on_slot_added(index):
	var slot_obj = slot_ui_scene.instantiate()
	slots_container.add_child(slot_obj)
	slots.insert(index, slot_obj)
	slot_obj.gui_input.connect(_on_slot_gui_input.bind(slot_obj))
	
func _on_slot_removed(index):
	slots[index].queue_free()
	slots.remove_at(index)
	
func _on_slot_gui_input(event : InputEvent, slot_obj):
	if event is InputEventMouseButton:
		if event.pressed:	
			var index = slots.find(slot_obj)
			if index < 0:
				return 
			emit_signal("slot_point_down", event, index, inventory)
			
func _on_inventory_gui_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			emit_signal("inventory_point_down", event, inventory)
