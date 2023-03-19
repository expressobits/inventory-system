extends VBoxContainer
class_name CraftingsUI

@export var crafting_ui_scene : PackedScene
@export var craft_station : CraftStation

var craftings : Array[CraftingUI]


func set_craft_station(craft_station : CraftStation):
	clear()
	if self.craft_station != null and self.craft_station != craft_station:
		craft_station.crafting_added.disconnect(_on_add_crafting_at.bind())
		craft_station.crafting_removed.disconnect(_on_remove_crafting_at.bind())
	if self.craft_station != craft_station:
		craft_station.crafting_added.connect(_on_add_crafting_at.bind())
		craft_station.crafting_removed.connect(_on_remove_crafting_at.bind())
	self.craft_station = craft_station
	
	for i in craft_station.craftings.size():
		_on_add_crafting_at(i)


func clear():
	for crafting in craftings:
		crafting.queue_free()	
	craftings.clear()


func _on_add_crafting_at(crafting_index : int):
	var crafting_obj = crafting_ui_scene.instantiate()
	add_child(crafting_obj)
	move_child(crafting_obj, 0)
	craftings.insert(crafting_index, crafting_obj)
	crafting_obj.set_crafting(craft_station, crafting_index)


func _on_remove_crafting_at(crafting_index : int):
	craftings[crafting_index].queue_free()
	craftings.remove_at(crafting_index)
