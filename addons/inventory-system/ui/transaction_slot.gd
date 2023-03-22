extends SlotUI
class_name TransactionSlotUI

## Special [SlotUI] that only represents slot information in transaction

func _ready():
	clear_info()
	
func update_info_with_item(item : InventoryItem, amount := 1):
	super.update_info_with_item(item, amount)
	visible = amount > 0
	self.global_position = get_global_mouse_position() - size/2
	$DropIcon.visible = false
	
func clear_info():
	super.clear_info()
	self.visible = false
	$DropIcon.visible = false
	
func _process(delta):
	if self.visible:
		self.global_position = get_global_mouse_position() - size/2


func _on_drop_area_mouse_exited():
	$DropIcon.visible = false


func _on_drop_area_mouse_entered():
	$DropIcon.visible = true
