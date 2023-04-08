@tool
extends Control
class_name ItemsEditor

var database : InventoryDatabase
var editor_plugin : EditorPlugin
@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var new_item_dialog : FileDialog = $NewItemResourceDialog
@onready var open_item_dialog : FileDialog = $OpenItemDialog
@onready var inventory_item_list  = $HSplitContainer/InventoryItemList
@onready var items_popup_menu : PopupMenu = $HSplitContainer/InventoryItemList/ItemsPopupMenu
@onready var item_remove_confirmation_dialog = %ItemRemoveConfirmationDialog

var current_id_item : int = -1

const ITEM_REMOVE = 100


func _ready():
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	item_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func load_items_from_database(database : InventoryDatabase) -> void:
	self.database = database
	load_items()


func load_items() -> void:
	inventory_item_list.load_items(database)


func remove_item(item_id : int):
	var item_database = database.get_item_database(item_id)
	if item_database == null:
		return
	var index = database.items.find(item_database)
	database.items.remove_at(index)
	load_items()


func select(id : int):
	for item_in_list in database.items:
		if item_in_list.id == id:
			item_editor.load_item(database.items[id])


func _apply_theme():
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_item_dialog):
		return
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_item_dialog.min_size = Vector2(600, 500) * scale


func _on_new_item_resource_dialog_file_selected(path):
	var item : InventoryItem = InventoryItem.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : InventoryItem = load(path)
		res.name = "New Item"
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		var new_database_item : InventoryDatabaseItem = InventoryDatabaseItem.new()
		new_database_item.id = database.get_valid_id()
		new_database_item.item = res
		database.items.append(new_database_item)
		load_items()
	else:
		print(err)


func _on_theme_changed():
	_apply_theme()


func _on_inventory_item_list_item_selected(item, index):
	current_id_item = item.id
	item_editor.load_item(item)


func _on_inventory_item_list_item_popup_menu_requested(at_position):
	var add = at_position + Vector2(0, items_popup_menu.size.y) + inventory_item_list.global_position
	items_popup_menu.position = Vector2(get_viewport().position) + add
	items_popup_menu.popup()


func _on_items_popup_menu_about_to_popup() -> void:
	items_popup_menu.clear()
	items_popup_menu.add_item("Remove", ITEM_REMOVE)


func _on_items_popup_menu_id_pressed(id: int) -> void:
	match id:
		ITEM_REMOVE:
			item_remove_confirmation_dialog.popup_centered()
			var item_database = database.get_item_database(current_id_item)
			if item_database == null or item_database.item == null:
				return
			item_remove_confirmation_dialog.dialog_text = "Remove Item \""+item_database.item.name+"\"?"


func _on_inventory_item_list_new_item_pressed():
	if not is_instance_valid(database):
		return
	
	new_item_dialog.popup_centered()


func _on_item_editor_changed(id):
	var index = inventory_item_list.get_index_of_item_id(id)
	if index > -1:
		inventory_item_list.update_item(index)


func _on_item_remove_confirmation_dialog_confirmed():
	remove_item(current_id_item)
