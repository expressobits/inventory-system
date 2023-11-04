@tool
extends Crafter
class_name NetworkedCrafter

func open(craft_station : CraftStation) -> bool:
	if not multiplayer.is_server():
		_open_rpc.rpc_id(1, craft_station.get_path())
		opened.emit(craft_station)
	else:
		_open_rpc(craft_station.get_path())
	return true


func close(craft_station : CraftStation) -> bool:
	if not multiplayer.is_server():
		_close_rpc.rpc_id(1, craft_station.get_path())
	else:
		_close_rpc(craft_station.get_path())
	return true


## === CLIENT COMMANDS TO SERVER ===

@rpc("any_peer")
func _open_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var craft_station = object as CraftStation
	if craft_station == null:
		return
	if super.open(craft_station):
		_open_response_rpc.rpc(object_path)


@rpc("any_peer")
func _close_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var craft_station = object as CraftStation
	if craft_station == null:
		return
	if super.close(craft_station):
		_close_response_rpc.rpc(object_path)
	
	
@rpc
func _open_response_rpc(object_path : NodePath):
	if multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var craft_station = object as CraftStation
	if craft_station == null:
		return
	opened_stations.append(craft_station)
	opened.emit(craft_station)


@rpc
func _close_response_rpc(object_path : NodePath):
	if multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var craft_station = object as CraftStation
	if craft_station == null:
		return
	var index = opened_stations.find(craft_station)
	if index == -1:
		return false
	opened_stations.remove_at(index)
	closed.emit(craft_station)
