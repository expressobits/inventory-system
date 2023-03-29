@tool
extends Control
class_name ItemsEditor


@export var item_in_list_scene : PackedScene

var _item_list : Array[ItemInList]
var _database : InventoryDatabase
@onready var _items_container : Control = $HSplitContainer/VBoxContainer/Items/MarginContainer/VBoxContainer
@onready var item_editor : ItemEditor = $HSplitContainer/ItemEditor

func load_items_from_database(database : InventoryDatabase) -> void:
	self._database = database
	load_items()


func load_items() -> void:
	clear_items()
	var str = ""
	for item_database in _database.items:
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
