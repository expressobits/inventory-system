extends SlotUI
class_name DragSlotUI

var inventory
var slot_index
var amount
var offset := Vector2.ZERO

func _ready():
	clear_info()
	
func has_slot() -> bool:
	return self.inventory != null and self.slot_index != -1
	
func update_drag_info(slot_index : int, inventory : Inventory, amount := 1, offset := Vector2.ZERO):
	self.inventory = inventory
	self.slot_index = slot_index
	self.offset = offset
	var slot = inventory.slots[slot_index]
	self.amount = amount
	self.visible = slot.has("item") and slot.item != null
	if slot.has("item") and slot.item != null:
		update_info_data(slot.item, amount)
	self.global_position = get_global_mouse_position() + offset
	
func clear_info():
	self.visible = false
	self.inventory = null
	self.slot_index = -1
	self.amount = 0
	super.clear_info()
	
func _input(delta):
	if has_slot():
		self.global_position = get_global_mouse_position() + offset
		
