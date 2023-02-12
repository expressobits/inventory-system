extends Control
class_name SlotUI

## Represents a [Slot] visually with item [Texture2D] and amount [Label]

@onready var item_icon: TextureRect = get_node(NodePath("Item Icon"))
@onready var amount_label: Label = get_node(NodePath("Amount"))

## Color when mouse enter
@export var highlight_color = Color.ORANGE


## Update information with [Dictionary] slot. 
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_slot(slot : Dictionary):
	if slot.has("item") and slot.item != null:
		update_info_with_item(slot.item, slot.amount)
	else:
		item_icon.texture = null
		amount_label.visible = false


## Update information with [InventoryItem] and amount.
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_item(item : InventoryItem, amount := 1):
	if item != null:
		item_icon.texture = item.icon
		tooltip_text = item.name
	else:
		item_icon.texture = null
		tooltip_text = ""
	amount_label.text = str(amount)
	amount_label.visible = amount > 1


## Clear info slot information
func clear_info():
		item_icon.texture = null
		amount_label.visible = false


func _on_mouse_entered():
	$Panel.self_modulate = highlight_color


func _on_mouse_exited():
	$Panel.self_modulate = Color.WHITE


func _on_hidden():
	$Panel.self_modulate = Color.WHITE
