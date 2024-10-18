class_name SlotUI
extends Control

## Represents a [Slot] visually with item [Texture2D] and amount [Label]

var database : InventoryDatabase

@onready var item_icon : TextureRect = get_node(NodePath("Item Icon"))
@onready var amount_label : Label = get_node(NodePath("Amount"))
@onready var selection_background : Panel = get_node(NodePath("Selected"))
@onready var category_icon : TextureRect = $"Category Icon"
@onready var panel = $Panel
@onready var durability : ProgressBar = $Durability

## Color when mouse enter
@export var highlight_color = Color.WHITE


## Update information with [Dictionary] slot. 
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_stack(database : InventoryDatabase, stack : ItemStack):
	self.database = database
	category_icon.visible = stack.amount == 0
	#setup_category(database, stack)
	if stack != null and stack.item_id != "":
		update_info_with_item(stack)
		return
	item_icon.texture = null
	amount_label.visible = false
	durability.visible = false

#func setup_category(database : InventoryDatabase, stack : ItemStack):
	#if is_categorized_slot_and_have_category(slot):
		#var category_id = slot.accepted_categories[0]
		#var category = database.get_category_from_id(category_id)
		#if category != null:
			#category_icon.texture = category.icon
			#panel.modulate = category.color
		#else:
			#category_icon.texture = null
			#panel.modulate = Color.WHITE
	#else:
		#category_icon.texture = null
		#panel.modulate = Color.WHITE

func is_categorized_slot_and_have_category(slot : Slot):
	if slot.categorized:
		if slot.accepted_categories.size() > 0:
			return true
	return false


## Update information with [ItemDefinition] and amount.
## If the item is null, the slot does not display its information, useful for fixed [Inventory].
## The amount label is only displayed if amount is greater than 1
func update_info_with_item(stack : ItemStack):
	if stack.item_id != "":
		var item_id = stack.item_id
		var definition = database.get_item(item_id)
		item_icon.texture = definition.icon
		tooltip_text = definition.name
		if stack.properties.has("durability") and definition.properties.has("durability"):
			durability.visible = true
			durability.value = stack.properties.durability
			durability.max_value = definition.properties.durability
		else:
			durability.visible = false
	else:
		category_icon.texture = null
		tooltip_text = ""
		durability.visible = false
	amount_label.text = str(stack.amount)
	amount_label.visible = stack.amount > 1


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
	$Panel.self_modulate = Color(Color.WHITE, 0.5)
