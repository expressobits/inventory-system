class_name CraftingsUI
extends VBoxContainer

## Represents the active craftings of [CraftStation] with a user interface.

## Scene to be instantiated that represents a crafting from the [CraftStation]'s crafting list
@export var crafting_ui_scene : PackedScene


var _craft_station : CraftStation
var _craftings : Array[CraftingUI]


## Configure crafting list for this interface
func set_craft_station(craft_station : CraftStation):
	_clear()
	if _craft_station != null and _craft_station != craft_station:
		_craft_station.crafting_added.disconnect(_on_add_crafting_at)
		_craft_station.crafting_removed.disconnect(_on_remove_crafting_at)
	if _craft_station != craft_station:
		craft_station.crafting_added.connect(_on_add_crafting_at)
		craft_station.crafting_removed.connect(_on_remove_crafting_at)
	_craft_station = craft_station
	
	for i in craft_station.craftings.size():
		_on_add_crafting_at(i)


func _clear():
	for crafting in _craftings:
		crafting.queue_free()	
	_craftings.clear()


func _on_add_crafting_at(crafting_index : int):
	var crafting_obj = crafting_ui_scene.instantiate()
	add_child(crafting_obj)
	move_child(crafting_obj, 0)
	_craftings.insert(crafting_index, crafting_obj)
	crafting_obj.set_crafting(_craft_station, crafting_index)


func _on_remove_crafting_at(crafting_index : int):
	_craftings[crafting_index].queue_free()
	_craftings.remove_at(crafting_index)
