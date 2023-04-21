@tool
extends InventoryTabEditor
class_name ItemsEditor

signal items_changed

@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var inventory_item_list  = $HSplitContainer/InventoryItemList
@onready var items_popup_menu : PopupMenu = $HSplitContainer/InventoryItemList/ItemsPopupMenu
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


func on_load_database() -> void:
	item_editor.load_item(null, database)
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
			item_editor.load_item(database.items[id], database)


func _apply_theme():
	super._apply_theme()
	if is_instance_valid(search_icon):
		search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_theme_changed():
	_apply_theme()


func _on_inventory_item_list_item_selected(item, index):
	current_id_item = item.id
	item_editor.load_item(item, database)


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
			var item = database.get_item(current_id_item)
			if item == null:
				return
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Item \""+item.name+"\"?"
		ITEM_REMOVE_AND_DELETE:
			var item = database.get_item(current_id_item)
			if item == null:
				return
			remove_and_delete_confirmation_dialog.popup_centered()
			remove_and_delete_confirmation_dialog.dialog_text = "Remove Item \""+item.name+"\" And Delete Resource \""+item.resource_path+"\"?"


func _on_item_editor_changed(id):
	var index = inventory_item_list.get_index_of_item_id(id)
	if index > -1:
		inventory_item_list.update_item(index)
		emit_signal("items_changed")


func _on_remove_confirmation_dialog_confirmed():
	remove_item(current_id_item)


func _on_remove_and_delete_confirmation_dialog_confirmed():
	var dir = DirAccess.open(".")
	var item = database.get_item(current_id_item)
	if item == null:
		return
	var code = dir.remove_absolute(item.resource_path)
	if code == OK:
		remove_item(current_id_item)
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		emit_signal("items_changed")


func _on_new_resource_dialog_file_selected(path):
	var item : InventoryItem = InventoryItem.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		item = load(path)
		item.name = "New Item"
		item.id = database.get_new_valid_id()
		database.items.append(item)
		load_items()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		emit_signal("items_changed")
	else:
		print(err)


func _on_open_resource_dialog_file_selected(path):
	# TODO Check if resource exist
	var res = load(path)
	if res is InventoryItem:
		var item : InventoryItem = res as InventoryItem
		if database.items.has(item):
			push_warning("The item "+item.name+"("+ item.resource_path +") already exists in the database!")
		database.items.append(item)
		load_items()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		emit_signal("items_changed")
