extends Node
class_name SyncCrafter

@export 
var crafter : Crafter

func _ready() -> void:
	if multiplayer.is_server():
		crafter.opened.connect(_on_opened.bind())
		crafter.closed.connect(_on_closed.bind())


func _on_opened(craft_station : CraftStation):
	open_rpc.rpc(crafter.get_path_to(craft_station))
	
func _on_closed(craft_station : CraftStation):
	close_rpc.rpc(crafter.get_path_to(craft_station))

@rpc
func open_rpc(craft_station_path : NodePath):
	var station : CraftStation = crafter.get_node(craft_station_path)
	crafter.opened_stations.append(craft_station_path)
	crafter.opened.emit(station)

@rpc
func close_rpc(craft_station_path : NodePath):
	var station : CraftStation = crafter.get_node(craft_station_path)
	var index = crafter.opened_stations.find(craft_station_path)
	if index == -1:
		return
	crafter.opened_stations.remove_at(index)
	crafter.closed.emit(station)
