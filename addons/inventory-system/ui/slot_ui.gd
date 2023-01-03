extends Control
class_name SlotUI

var item_icon_path := NodePath("Item Icon")
var amount_label_path := NodePath("Amount")

@onready var item_icon: TextureRect = get_node(item_icon_path)
@onready var amount_label: Label = get_node(amount_label_path)

func update_info(slot : Dictionary):
	if slot.has("item") and slot.item != null:
		item_icon.texture = slot.item.icon
	else:
		item_icon.texture = null
	if slot.has("amount") and slot.amount != null:
		amount_label.text = str(slot.amount)
		amount_label.visible = slot.amount > 1
	else:
		amount_label.visible = false
		
func clear_info():
		item_icon.texture = null
		amount_label.visible = false
