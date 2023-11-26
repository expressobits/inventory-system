extends SlotUI
class_name TransactionSlotUI

## Special [SlotUI] that only represents slot information in transaction

func _ready():
	clear_info()
	
func update_info_with_item(slot : Slot):
	super.update_info_with_item(slot)
	visible = slot.amount > 0
	self.global_position = get_global_mouse_position() - size/2
	$DropIcon.visible = false


func clear_info():
	super.clear_info()
	self.visible = false
	$DropIcon.visible = false


func _process(delta):
	if self.visible:
		if InventorySystem.is_console_mode:
			if get_viewport().gui_get_focus_owner():
				self.global_position = get_viewport().gui_get_focus_owner().global_position
			else:
				self.global_position = get_global_mouse_position() - size/2
		else:
			self.global_position = get_global_mouse_position() - size/2


func _on_drop_area_mouse_exited():
	$DropIcon.visible = false


func _on_drop_area_mouse_entered():
	$DropIcon.visible = true
