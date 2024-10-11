extends Node

@export var database : InventoryDatabase
@export var modding_file_path : String

func _ready() -> void:
	database.import_json_file(modding_file_path)
