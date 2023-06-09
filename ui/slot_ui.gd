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
	category_icon.visible = slot.amount == 0
	
	if is_categorized_slot_and_have_category(slot):
		category_icon.texture = slot.accepted_categories[0].icon
		panel.modulate = slot.accepted_categories[0].color
	else:
		category_icon.texture = null
		panel.modulate = Color.WHITE
		
	if slot != null and slot.item != null:
		# TODO Slow call, use cache from node inv base
		if slot.item != null:
			update_info_with_item(slot.item, slot.amount)
			return
	item_icon.texture = null
	amount_label.visible = false


func is_categorized_slot_and_have_category(slot : Slot):
	if slot is CategorizedSlot:
		var c_slot = slot as CategorizedSlot
		if c_slot.accepted_categories.size() > 0:
			return true
	return false


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
	grab_focus()


func _on_mouse_exited():
	release_focus()


func _on_hidden():
	release_focus()


func _on_focus_entered() -> void:
	$Panel.self_modulate = highlight_color


func _on_focus_exited() -> void:
	$Panel.self_modulate = Color.WHITE
