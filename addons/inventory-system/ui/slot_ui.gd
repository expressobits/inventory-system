extends Control
class_name SlotUI

## Represents a [Slot] visually with item [Texture2D] and amount [Label]

@onready var item_icon : TextureRect = get_node(NodePath("Item Icon"))
@onready var amount_label : Label = get_node(NodePath("Amount"))
@onready var selection_background : Panel = get_node(NodePath("Selected"))
@onready var category_icon : TextureRect = $"Category Icon"
@onready var panel = $Panel

## Color when mouse enter
@export var highlight_color = Color.ORANGE


## Update information with [Dictionary] slot. 
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_slot(slot : Slot):
	if slot != null and slot.item != null:
		# TODO Slow call, use cache from node inv base
		if slot.item != null:
			update_info_with_item(slot.item, slot.amount)
			return
	item_icon.texture = null
	amount_label.visible = false
	if slot.accepted_categories.size() > 0:
		category_icon.texture = slot.accepted_categories[0].icon
#		panel.modulate = slot.accepted_categories[0].color
	else:
		category_icon.texture = null
#		panel.modulate = Color.WHITE


## Update information with [InventoryItem] and amount.
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_item(item : InventoryItem, amount := 1):
	if item != null:
		item_icon.texture = item.icon
		tooltip_text = item.name
	else:
		category_icon.texture = null
		tooltip_text = ""
	amount_label.text = str(amount)
	amount_label.visible = amount > 1


## Clear info slot information
func clear_info():
		category_icon.texture = null
		amount_label.visible = false
		

func set_selection(is_selected : bool):
	selection_background.visible = is_selected


func _on_mouse_entered():
	$Panel.self_modulate = highlight_color


func _on_mouse_exited():
	$Panel.self_modulate = Color.WHITE


func _on_hidden():
	$Panel.self_modulate = Color.WHITE
