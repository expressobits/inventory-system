@tool
extends Control
class_name ItemsEditor


@export var item_in_list_scene : PackedScene

var _item_list : Array[ItemInList]
var database : InventoryDatabase
@onready var _items_container : Control = $HSplitContainer/VBoxContainer/Items/MarginContainer/VBoxContainer
@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor
@onready var new_item_dialog : FileDialog = $NewItemDialog
@onready var open_item_dialog : FileDialog = $OpenItemDialog


func _ready():
	item_editor.changed.connect(_item_editor_changed.bind())


func load_items_from_database(database : InventoryDatabase) -> void:
	self.database = database
	load_items()


func load_items() -> void:
	clear_items()
	var str = ""
	for item_database in database.items:
		var item_in_list_obj = item_in_list_scene.instantiate()
		_items_container.add_child(item_in_list_obj)
		var item_in_list = item_in_list_obj as ItemInList
		_item_list.append(item_in_list)
		item_in_list.set_item(item_database)
		item_in_list.selected.connect(select.bind())


func select(id : int):
	for item_in_list in _item_list:
		if item_in_list.item_database.id != id:
			item_in_list.unselect()
		else:
			item_editor.load_item(item_in_list.item_database)


func clear_items():
	for item_in_list in _item_list:
		item_in_list.queue_free()
	_item_list.clear()


func update_item_in_list(id : int):
	for item_in_list in _item_list:
		if item_in_list.item_database.id == id:
			item_in_list.update_item()


func _on_new_item_button_pressed():
	if not is_instance_valid(database):
		return
	var new_database_item : InventoryDatabaseItem = InventoryDatabaseItem.new()
	new_database_item.id = database.get_valid_id()
	database.items.append(new_database_item)
	load_items()
#	new_item_dialog.popup_centered()


func _item_editor_changed(id : int):
	update_item_in_list(id)
