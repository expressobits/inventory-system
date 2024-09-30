@tool
class_name ItemsEditor
extends InventoryTabEditor

@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var inventory_item_list  = $HSplitContainer/InventoryItemList
@onready var items_popup_menu : PopupMenu = $HSplitContainer/InventoryItemList/ItemsPopupMenu
@onready var search_icon = $HSplitContainer/InventoryItemList/Control/SearchIcon


func _ready():
	super._ready()
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


func remove_current_data():
	remove_item(current_data)


func remove_item(item : ItemDefinition):
	if item == null:
		return
	database.remove_item(item)
	load_items()
	data_changed.emit()


func select(id : String):
	for item_in_list in database.items:
		if item_in_list.id == id:
			item_editor.load_item(database.get_item(id), database)


func _apply_theme():
	super._apply_theme()
	if is_instance_valid(search_icon):
		search_icon.texture = get_theme_icon("Search", "EditorIcons")


func _on_theme_changed():
	_apply_theme()


func _on_inventory_item_list_item_selected(item, index):
	current_data = item
	item_editor.load_item(item, database)


func _on_inventory_item_list_item_popup_menu_requested(at_position):
	items_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	var copy = get_theme_icon("CopyNodePath", "EditorIcons")
	items_popup_menu.add_icon_item(copy, "Copy Resource Path", ITEM_COPY_RESOURCE_PATH)
	items_popup_menu.add_separator()
	items_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)
	
	var a = inventory_item_list.get_global_mouse_position()
	items_popup_menu.position = Vector2(get_viewport().position) + a
	items_popup_menu.popup()


func _on_items_popup_menu_id_pressed(id: int) -> void:
	match id:
		ITEM_COPY_RESOURCE_PATH:
			if current_data == null:
				return
			DisplayServer.clipboard_set(current_data.resource_path)
		ITEM_REMOVE:
			if current_data == null:
				return
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Item \""+current_data.name+"\"?"


func _on_item_editor_changed(id):
	var index = inventory_item_list.get_index_of_item_id(id)
	if index > -1:
		inventory_item_list.update_item(index)
		data_changed.emit()
