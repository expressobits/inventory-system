extends Node
class_name SyncCraftStation

@export var craft_station : CraftStation
var craftings_data : Array

func _ready() -> void:
	multiplayer.peer_connected.connect(_on_connected.bind())
	craft_station.opened.connect(_on_opened)
	craft_station.closed.connect(_on_closed)
	craft_station.crafting_added.connect(_on_crafting_added)
	craft_station.crafting_removed.connect(_on_crafting_removed)


func _on_connected(peer_id : int):
	if not multiplayer.is_server():
		craft_station.can_finish_craftings = false
		return
	if craft_station.is_open:
		open_rpc.rpc_id(peer_id)
	_update_craftings_rpc.rpc_id(peer_id, craftings_data)


func _on_opened():
	if not multiplayer.is_server():
		return
	open_rpc.rpc()
	
func _on_closed():
	if not multiplayer.is_server():
		return
	close_rpc.rpc()
	

func _on_crafting_added(crafting_index : int):
	if not multiplayer.is_server():
		return
	var crafting = craft_station.craftings[crafting_index]
	crafting_added_rpc.rpc(crafting.recipe_index)
	craftings_data.append(crafting.to_data())


func _on_crafting_removed(crafting_index : int):
	if not multiplayer.is_server():
		return
	crafting_removed_rpc.rpc(crafting_index)
	craftings_data.remove_at(crafting_index)


@rpc
func open_rpc():
	craft_station.open()

@rpc
func close_rpc():
	craft_station.close()
	
@rpc
func crafting_added_rpc(recipe_index : int):
	if multiplayer.is_server():
		return
	var recipe = craft_station.database.recipes[recipe_index]
	craft_station.add_crafting(recipe_index, recipe)
	
@rpc
func crafting_removed_rpc(crafting_index : int):
	if multiplayer.is_server():
		return
	craft_station.remove_crafting(crafting_index)

@rpc
func _update_craftings_rpc(craftings_data : Array):
	for data in craftings_data:
		var crafting = Crafting.new()
		crafting.from_data(data)
		craft_station.craftings.append(crafting)
