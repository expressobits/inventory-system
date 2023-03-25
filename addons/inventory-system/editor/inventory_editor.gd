@tool
extends Control

#var _option_database = $VBoxContainer/MarginContainer/HBoxContainer/OptionButton

var _default_database = preload("res://addons/inventory-system/demos/base/database.tres")

@export var items_editor : ItemsEditor

func _ready():
	pass

func _process(delta):
	pass


func load_defaults():
	items_editor.load_database(_default_database)


func load_database(database : InventoryDatabase):
	items_editor.load_database(database)


func save_database(database : InventoryDatabase):
	pass
