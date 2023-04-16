@tool
extends Control
class_name ItemsEditor

signal items_changed

var database : InventoryDatabase
var editor_plugin : EditorPlugin
@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var new_item_dialog : FileDialog = $NewItemResourceDialog
@onready var open_item_dialog : FileDialog = $OpenItemDialog
@onready var inventory_item_list  = $HSplitContainer/InventoryItemList
@onready var items_popup_menu : PopupMenu = $HSplitContainer/InventoryItemList/ItemsPopupMenu
@onready var item_remove_confirmation_dialog = %ItemRemoveConfirmationDialog
@onready var item_remove_and_delete_confirmation_dialog = %ItemRemoveAndDeleteConfirmationDialog
@onready var search_icon = $HSplitContainer/InventoryItemList/Control/SearchIcon

var current_id_item : int = -1

const ITEM_COPY_RESOURCE_PATH = 100
const ITEM_REMOVE = 105
const ITEM_REMOVE_AND_DELETE = 106


func _ready():
	_apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	item_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func load_from_database(database : InventoryDatabase) -> void:
	self.database = database
	item_editor.load_item(null)
	load_items()


func load_items() -> void:
	inventory_item_list.load_items(database)


func remove_item(item_id : int):
	var item = database.get_item(item_id)
	if item == null:
		return
	var index = database.items.find(item)
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
	open_item_dialog.min_size = Vector2(600, 500) * scale
	new_item_dialog.min_size = Vector2(600, 500) * scale
	
	search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_new_item_resource_dialog_file_selected(path):
	var item : InventoryItem = InventoryItem.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : InventoryItem = load(path)
		res.name = "New Item"
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		res.id = database.get_valid_id()
		res.item = res
		database.items.append(res)
		load_items()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	else:
		print(err)


func _on_theme_changed():
	_apply_theme()


func _on_inventory_item_list_item_selected(item, index):
	current_id_item = item.id
	item_editor.load_item(item)


func _on_inventory_item_list_item_popup_menu_requested(at_position):
	items_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	var copy = get_theme_icon("CopyNodePath", "EditorIcons")
	items_popup_menu.add_icon_item(copy, "Copy Resource Path", ITEM_COPY_RESOURCE_PATH)
	items_popup_menu.add_separator()
	items_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)
	items_popup_menu.add_icon_item(icon, "Remove and Delete Resource", ITEM_REMOVE_AND_DELETE)
	
	var a = inventory_item_list.get_global_mouse_position()
	items_popup_menu.position = Vector2(get_viewport().position) + a
	items_popup_menu.popup()


func _on_items_popup_menu_id_pressed(id: int) -> void:
	match id:
		ITEM_COPY_RESOURCE_PATH:
			var item = database.get_item(current_id_item)
			if item == null:
				return
			DisplayServer.clipboard_set(item.resource_path)
		ITEM_REMOVE:
			item_remove_confirmation_dialog.popup_centered()
			var item = database.get_item(current_id_item)
			if item == null:
				return
			item_remove_confirmation_dialog.dialog_text = "Remove Item \""+item.name+"\"?"
		ITEM_REMOVE_AND_DELETE:
			item_remove_and_delete_confirmation_dialog.popup_centered()
			var item = database.get_item(current_id_item)
			if item == null:
				return
			item_remove_and_delete_confirmation_dialog.popup_centered()
			item_remove_and_delete_confirmation_dialog.dialog_text = "Remove Item \""+item.name+"\" And Delete Resource \""+item.resource_path+"\"?"


func new_item_pressed():
	if not is_instance_valid(database):
		return
	
	new_item_dialog.popup_centered()


func new_item_from_resource_pressed():
	if not is_instance_valid(database):
		return
	
	open_item_dialog.popup_centered()


func _on_item_editor_changed(id):
	var index = inventory_item_list.get_index_of_item_id(id)
	if index > -1:
		inventory_item_list.update_item(index)


func _on_item_remove_confirmation_dialog_confirmed():
	remove_item(current_id_item)


func _on_item_remove_and_delete_confirmation_dialog_confirmed():
	var dir = DirAccess.open(".")
	var item = database.get_item(current_id_item)
	if item == null:
		return
	var code = dir.remove_absolute(item.resource_path)
	if code == OK:
		remove_item(current_id_item)
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		emit_signal("items_changed")


func _on_open_item_dialog_file_selected(path):
	# TODO Check if resource exist
	var res = load(path)
	if res is InventoryItem:
		var item : InventoryItem = res as InventoryItem
		database.items.append(item)
		load_items()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		emit_signal("items_changed")
