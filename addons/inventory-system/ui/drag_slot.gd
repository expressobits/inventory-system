extends SlotUI
class_name DragSlotUI

var inventory

func _ready():
	clear_info()
	
func update_drag_info(slot_index : int, inventory : Inventory):
	self.inventory = inventory
	var slot = inventory.slots[slot_index]
	self.visible = slot.has("item") and slot.item != null
	update_info(slot)
	self.global_position = get_global_mouse_position()
	
func clear_info():
	self.visible = false
	self.inventory = null
	super.clear_info()
	
func _input(delta):
	if self.visible:
		self.global_position = get_global_mouse_position()
		
