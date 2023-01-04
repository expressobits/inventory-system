extends Control
class_name SlotUI

var item_icon_path := NodePath("Item Icon")
var amount_label_path := NodePath("Amount")

@onready var item_icon: TextureRect = get_node(item_icon_path)
@onready var amount_label: Label = get_node(amount_label_path)

func update_info(slot : Dictionary):
	if slot.has("item") and slot.item != null:
		update_info_data(slot.item, slot.amount)
	else:
		item_icon.texture = null
		amount_label.visible = false

func update_info_data(item : Item, amount := 1):
	if item != null:
		item_icon.texture = item.icon
	else:
		item_icon.texture = null
	amount_label.text = str(amount)
	amount_label.visible = amount > 1
		
func clear_info():
		item_icon.texture = null
		amount_label.visible = false
