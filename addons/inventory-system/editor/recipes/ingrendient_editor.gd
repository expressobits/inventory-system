@tool
extends HBoxContainer
class_name IngredientEditor

signal request_remove

@onready var slot_selector = $SlotSelector
@onready var delete_button = $DeleteButton
@onready var ingredient_remove_confirmation_dialog = $IngredientRemoveConfirmationDialog

var slot : Slot
var database : InventoryDatabase
var ids_list : Array[int]


func _ready():
	slot_selector.slot_changed.connect(_on_slot_selector_slot_changed.bind())
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.tooltip_text = "Delete"


func setup(slot : Slot, database : InventoryDatabase, tooltip_text : String):
	self.slot = slot
	self.database = database
	var item_id = database.get_id_from_item(slot.item)
	slot.id = item_id
	slot_selector.setup(slot, database)
	delete_button.tooltip_text = tooltip_text
	ingredient_remove_confirmation_dialog.dialog_text = tooltip_text+"?"


func _on_slot_selector_slot_changed(slot : Slot):
	var item = database.get_item(slot.id)
	slot.item = item


func _on_delete_button_pressed():
	ingredient_remove_confirmation_dialog.popup_centered()


func _on_ingredient_remove_confirmation_dialog_confirmed():
	emit_signal("request_remove")
