@tool
class_name IngredientEditor
extends HBoxContainer

signal changed_slot
signal request_remove

var item_stack_selector
var delete_button : Button
var ingredient_remove_confirmation_dialog : ConfirmationDialog

var item_stack : ItemStack
var database : InventoryDatabase
var ids_list : Array[int]


func _ready():
	size_flags_horizontal = SIZE_EXPAND_FILL
	
	item_stack_selector = ItemStackSelector.new()
	item_stack_selector.layout_mode = 2
	item_stack_selector.size_flags_horizontal = 3
	add_child(item_stack_selector)
	
	delete_button = Button.new()
	delete_button.custom_minimum_size = Vector2(32, 0)
	delete_button.layout_mode = 2
	delete_button.tooltip_text = "Delete"
	delete_button.flat = true
	delete_button.pressed.connect(_on_delete_button_pressed)
	add_child(delete_button)
	
	ingredient_remove_confirmation_dialog = ConfirmationDialog.new()
	ingredient_remove_confirmation_dialog.size = Vector2i(400, 100)
	ingredient_remove_confirmation_dialog.dialog_text = "Remove?"
	ingredient_remove_confirmation_dialog.confirmed.connect(_on_ingredient_remove_confirmation_dialog_confirmed)
	add_child(ingredient_remove_confirmation_dialog)
	
	item_stack_selector.changed.connect(_on_item_stack_selector_slot_changed)
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.tooltip_text = "Delete"


func setup(item_stack : ItemStack, database : InventoryDatabase, tooltip_text : String):
	self.item_stack = item_stack
	self.database = database
	item_stack_selector.setup(item_stack, database)
	delete_button.tooltip_text = tooltip_text
	ingredient_remove_confirmation_dialog.dialog_text = tooltip_text+"?"


func _on_item_stack_selector_slot_changed(item_stack : ItemStack):
	changed_slot.emit()


func _on_delete_button_pressed():
	ingredient_remove_confirmation_dialog.popup_centered()


func _on_ingredient_remove_confirmation_dialog_confirmed():
	request_remove.emit()
