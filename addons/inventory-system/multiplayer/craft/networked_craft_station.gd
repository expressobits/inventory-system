@tool
extends CraftStation
class_name NetworkedCraftStation

var craftings_data_sync : Array


func _ready():
	super._ready()
	multiplayer.peer_connected.connect(_on_connected.bind())
	crafting_added.connect(_on_crafting_added.bind())
#	updated_crafting.connect(_on_updated_crafting.bind())
	crafting_removed.connect(_on_crafting_removed.bind())
	opened.connect(_on_opened.bind())
	closed.connect(_on_closed.bind())


## === OVERRIDE MAIN COMMANDS ===
## Override all main commands for the client to send to the server through rpc

func craft(recipe_index : int):
	if not multiplayer.is_server():
		_craft_rpc.rpc_id(1, recipe_index)
	else:
		_craft_rpc(recipe_index)
	return true


func cancel_craft(crafting_index : int):
	if not multiplayer.is_server():
		_cancel_craft_rpc.rpc_id(1, crafting_index)
	else:
		_cancel_craft_rpc(crafting_index)
	return true


func _finish_crafting(crafting_index : int):
	if multiplayer.is_server():
		super._finish_crafting(crafting_index)


## === CLIENT COMMANDS TO SERVER ===

@rpc("any_peer")
func _craft_rpc(recipe_index : int):
	if not multiplayer.is_server():
		return
	super.craft(recipe_index)
	

@rpc("any_peer")
func _cancel_craft_rpc(crafting_index : int):
	if not multiplayer.is_server():
		return
	super.cancel_craft(crafting_index)
	

func _on_connected(id):
	if not multiplayer.is_server():
		return
	if is_open:
		_opened_rpc.rpc_id(id)
	_update_craftings_rpc.rpc_id(id, craftings_data_sync)


@rpc
func _update_craftings_rpc(craftings_data : Array):
	for data in craftings_data:
		var crafting = Crafting.new()
		crafting.from_data(data)
		self.craftings.append(crafting)


func _on_opened():
	if not multiplayer.is_server():
		return
	_opened_rpc.rpc()


func _on_closed():
	if not multiplayer.is_server():
		return
	_closed_rpc.rpc()


func _on_crafting_added(crafting_index : int):
	if not multiplayer.is_server():
		return
	var crafting = craftings[crafting_index]
	_crafting_added_rpc.rpc(crafting.recipe_index)
	craftings_data_sync.append(crafting.to_data())


func _on_crafting_removed(crafting_index : int):
	if not multiplayer.is_server():
		return
	_crafting_removed_rpc.rpc(crafting_index)
	craftings_data_sync.remove_at(crafting_index)


## === RESPONSES ===

@rpc
func _crafting_added_rpc(recipe_index : int):
	if multiplayer.is_server():
		return
	var recipe = database.recipes[recipe_index]
	_add_crafting(recipe_index, recipe)


@rpc
func _crafting_removed_rpc(crafting_index : int):
	if multiplayer.is_server():
		return
	_remove_crafting(crafting_index)


@rpc
func _opened_rpc():
	if multiplayer.is_server():
		return
	open()


@rpc
func _closed_rpc():
	if multiplayer.is_server():
		return
	close()
