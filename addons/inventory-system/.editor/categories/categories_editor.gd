@tool
class_name CategoriesEditor
extends BaseInventoryEditor


@onready var item_categories_item_list = $HSplitContainer/ItemCategoriesItemList
@onready var item_category_popup_menu = $ItemCategoryPopupMenu
@onready var item_category_editor = $HSplitContainer/ItemCategoryEditor


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	item_category_editor.set_editor_plugin(editor_plugin)
	_apply_theme()


func _apply_theme():
	super._apply_theme()
	if not is_instance_valid(item_categories_item_list.search_icon):
		return
	item_categories_item_list.search_icon.texture = get_theme_icon("Search", "EditorIcons")

func on_load_database() -> void:
	item_category_editor.load_category(database, null)
	load_item_categories()
	
	
func remove_current_data() -> bool:
	var removed = super.remove_current_data()
	if removed:
		load_item_categories()
		data_changed.emit()
		item_category_editor.load_category(database, null)
	return removed
	
	
func select(category : ItemCategory):
	item_category_editor.load_category(database, category)
	item_categories_item_list.select(category)


func load_item_categories():
	item_categories_item_list.load_items(database.item_categories)


func _on_item_category_popup_menu_id_pressed(id):
	match id:
		ITEM_REMOVE:
			remove_confirmation_dialog.popup_centered()
			remove_confirmation_dialog.dialog_text = "Remove Item Category \""+current_data.name+"\"?"
		ITEM_DUPLICATE:
			super.duplicate_current_data()


func _on_item_categories_item_list_item_popup_menu_requested(at_position):
	item_category_popup_menu.clear()
	var icon = get_theme_icon("Remove", "EditorIcons")
	item_category_popup_menu.add_icon_item(icon, "Remove", ITEM_REMOVE)
	
	icon = get_theme_icon("Duplicate", "EditorIcons")
	item_category_popup_menu.add_icon_item(icon, "Duplicate", ITEM_DUPLICATE)

	var a = item_categories_item_list.get_global_mouse_position()
	item_category_popup_menu.position = Vector2(get_viewport().position) + a
	item_category_popup_menu.popup()
	

func _on_item_category_editor_changed(category):
	var index = item_categories_item_list.get_index_of_item_id(category.id)
	if index > -1:
		item_categories_item_list.update_item(index)
		data_changed.emit()


func _on_item_categories_item_list_item_selected(item: Variant, index: int) -> void:
	current_data = item
	select(item)
