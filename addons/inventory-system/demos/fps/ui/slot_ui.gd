extends Control

var item_icon_path := NodePath("Item Icon")
var amount_label_path := NodePath("Amount")

@onready var item_icon: TextureRect = get_node(item_icon_path)
@onready var amount_label: Label = get_node(amount_label_path)

func update_info(item : Item, amount := 0):
	item_icon.texture = item.icon
	amount_label.text = str(amount)
