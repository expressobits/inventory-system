extends SlotUI
class_name DragSlotUI

var amount
var offset := Vector2.ZERO

var inventory_handler : InventoryHandler

func _ready():
	clear_info()
	
func has_slot() -> bool:
	return self.inventory != null and self.slot_index != -1
	
func update_data(item : Item, amount := 1):
	update_info_data(item, amount)
	visible = amount > 0
	self.global_position = get_global_mouse_position() + offset
	
func clear_info():
	self.visible = false
	self.amount = 0
	super.clear_info()
	$DropIcon.visible = false
	
func _input(delta):
	if inventory_handler != null and inventory_handler.is_transaction_active():
		self.global_position = get_global_mouse_position() + offset
		
func enter_in_drop_area():
	$DropIcon.visible = true
