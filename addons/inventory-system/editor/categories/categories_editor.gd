@tool
extends InventoryTabEditor
class_name CategoriesEditor

@onready var item_categories_item_list = $HSplitContainer/ItemCategoriesItemList
@onready var search_icon = $HSplitContainer/ItemCategoriesItemList/Control/SearchIcon
@onready var item_category_popup_menu = $ItemCategoryPopupMenu
@onready var item_category_editor = $HSplitContainer/ItemCategoryEditor


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	item_category_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func _apply_theme():
	super._apply_theme()
	if not is_instance_valid(search_icon):
		return
	search_icon.texture = get_theme_icon("Search", "EditorIcons")

func on_load_database() -> void:
	item_category_editor.load_category(null)
	load_item_categories()
	
	
func select(category : ItemCategory):
	item_category_editor.load_category(category)


func load_item_categories():
	item_categories_item_list.load_categories_types(database)


func remove_current_data():
	remove_category(current_data)


func remove_category(category : ItemCategory):
	database.remove_category(category)
	ResourceSaver.save(database, database.resource_path)
	load_item_categories()
	data_changed.emit()


func _on_new_resource_dialog_file_selected(path):
	var item : ItemCategory = ItemCategory.new()
	var err = ResourceSaver.save(item, path)
	if err == OK:
		var res : ItemCategory = load(path)
		res.name = get_name_of_resource_path(path)
		database.add_new_category(res)
		ResourceSaver.save(database, database.resource_path)
		load_item_categories()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	else:
		print(err)


func _on_item_category_popup_menu_id_pressed(id):
	match id:
		ITEM_COPY_RESOURCE_PATH:
			DisplayServer.clipboard_set(current_data.resource_path)
		ITEM_REMOVE:
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Item Category \""+current_data.name+"\"?"
		ITEM_REMOVE_AND_DELETE:
			remove_and_delete_confirmation_dialog.popup_centered()
			remove_and_delete_confirmation_dialog.dialog_text = "Remove Item Category \""+current_data.name+"\" And Delete Resource \""+current_data.resource_path+"\"?"


func _on_item_categories_item_list_item_popup_menu_requested(at_position):
	item_category_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	var copy = get_theme_icon("CopyNodePath", "EditorIcons")
	item_category_popup_menu.add_icon_item(copy, "Copy Resource Path", ITEM_COPY_RESOURCE_PATH)
	item_category_popup_menu.add_separator()
	item_category_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)
	item_category_popup_menu.add_icon_item(icon, "Remove and Delete Resource", ITEM_REMOVE_AND_DELETE)
	item_category_popup_menu.set_item_disabled(3, true)
	
	var a = item_categories_item_list.get_global_mouse_position()
	item_category_popup_menu.position = Vector2(get_viewport().position) + a
	item_category_popup_menu.popup()


func _on_open_resource_dialog_file_selected(path):
	var res = load(path)
	if res is ItemCategory:
		var category : ItemCategory = res as ItemCategory
		if database.item_categories.has(category):
			push_warning("The item category type \""+ category.name +"\" already exists in the list of item categories in the database!")
			return
		database.add_new_category(category)
		ResourceSaver.save(database, database.resource_path)
		load_item_categories()
		editor_plugin.get_editor_interface().get_resource_filesystem().scan()
		data_changed.emit()


func _on_item_categories_item_list_category_selected(category):
	current_data = category
	select(category)


func _on_item_category_editor_changed(category):
	var index = item_categories_item_list.get_index_of_category(category)
	if index > -1:
		item_categories_item_list.update_item(index)
		data_changed.emit()
