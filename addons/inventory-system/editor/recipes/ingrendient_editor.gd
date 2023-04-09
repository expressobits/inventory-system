@tool
extends HBoxContainer
class_name IngredientEditor

signal request_remove

@onready var id_spin_box = $IDSpinBox
@onready var ingredient_option_button = $IngredientOptionButton
@onready var amount_spin_box = $AmountSpinBox
@onready var delete_button = $DeleteButton
@onready var ingredient_remove_confirmation_dialog = $IngredientRemoveConfirmationDialog

var slot : Slot
var database : InventoryDatabase
var ids_list : Array[int]


func _ready():
	id_spin_box.value_changed.connect(_on_id_spin_box_value_changed.bind())
	amount_spin_box.value_changed.connect(_on_amount_spin_box_value_changed.bind())
	ingredient_option_button.item_selected.connect(_on_ingredient_option_button_item_selected.bind())
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.tooltip_text = "Delete Ingredient"


func setup(slot : Slot, database : InventoryDatabase):
	self.slot = slot
	self.database = database
	var item_id = database.get_id_from_item(slot.item)
	id_spin_box.value = item_id
	amount_spin_box.value = slot.amount
	
	for i in database.items.size():
		var item_database = database.items[i]
		ingredient_option_button.add_icon_item(item_database.item.icon ,item_database.item.name)
		ids_list.append(item_database.id)
		if item_database.item == slot.item:
			ingredient_option_button.select(i)


func _on_id_spin_box_value_changed(value):
	var item = database.get_item(value)
	slot.item = item
	var index = ids_list.find(int(value))
	if index != -1:
		if ingredient_option_button.selected != index:
			ingredient_option_button.select(index)


func _on_amount_spin_box_value_changed(value):
	slot.amount = value


func _on_ingredient_option_button_item_selected(index):
	var id : int = ids_list[index]
	if id_spin_box.value != id:
		id_spin_box.value = id


func _on_delete_button_pressed():
	ingredient_remove_confirmation_dialog.popup_centered()


func _on_ingredient_remove_confirmation_dialog_confirmed():
	emit_signal("request_remove")
