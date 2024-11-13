extends Node
class_name SyncCraftStation

@export var craft_station : CraftStation
@export var sync_input_inventories : bool
var craftings_data : Array


func _ready() -> void:
	craft_station.can_finish_craftings = is_multiplayer_authority()
	multiplayer.peer_connected.connect(_on_connected)
	craft_station.crafting_added.connect(_on_crafting_added)
	craft_station.crafting_removed.connect(_on_crafting_removed)
	if sync_input_inventories:
		craft_station.input_inventory_added.connect(_on_input_inventory_added)
		craft_station.input_inventory_removed.connect(_on_input_inventory_removed)


func _on_connected(peer_id : int):
	craft_station.can_finish_craftings = is_multiplayer_authority()
	if not multiplayer.is_server():
		return
	_update_craftings_rpc.rpc_id(peer_id, craftings_data)
	

func _on_crafting_added(crafting_index : int):
	if not multiplayer.is_server():
		return
	var crafting = craft_station.craftings[crafting_index]
	crafting_added_rpc.rpc(crafting.recipe_index)
	craftings_data.append(crafting.serialize())
	


func _on_crafting_removed(crafting_index : int):
	if not multiplayer.is_server():
		return
	crafting_removed_rpc.rpc(crafting_index)
	craftings_data.remove_at(crafting_index)


func _on_input_inventory_added(inventory_path : NodePath):
	if not multiplayer.is_server():
		return
	input_inventory_added_rpc.rpc(inventory_path)
	
	
func _on_input_inventory_removed(inventory_path : NodePath):
	if not multiplayer.is_server():
		return
	input_inventory_removed_rpc.rpc(inventory_path)

	
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
	craft_station.can_finish_craftings = is_multiplayer_authority()
	for data in craftings_data:
		var crafting = Crafting.new()
		crafting.deserialize(data)
		craft_station.craftings.append(crafting)
		
@rpc
func input_inventory_added_rpc(inventory_path : NodePath):
	craft_station.add_input_inventory(craft_station.get_node(inventory_path))

@rpc
func input_inventory_removed_rpc(inventory_path : NodePath):
	craft_station.remove_input_inventory(craft_station.get_node(inventory_path))
