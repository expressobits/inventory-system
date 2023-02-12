extends FPSDemo
class_name MPDemo

@export var player_scene : PackedScene

var connected_peer_ids : Array
var players : Array
var peer : ENetMultiplayerPeer

func _ready():
	multiplayer.peer_connected.connect(_player_connected.bind())
#	multiplayer.peer_disconnected.connect(_player_disconnected.bind())
#	multiplayer.connected_to_server.connect(_connected_ok.bind())
#	multiplayer.connection_failed.connect(_connected_fail.bind())
#	multiplayer.server_disconnected.connect(_server_disconnected.bind())

func create_player(peer_id : int):
	connected_peer_ids.append(peer_id)
	var player = player_scene.instantiate()
	player.name = str(peer_id)
	players.append(player)
	player.position = Vector3(0,2,0)
	add_child(player)
	player.set_multiplayer_authority(peer_id)
	if peer_id == multiplayer.get_unique_id():
		setup_inventory_handler(player.get_node("InventoryHandler"))


@rpc	
func add_newly_connected_player_character(new_peer_id):
	create_player(new_peer_id)


@rpc
func add_previously_connected_player_characters(peer_ids):
	for peer_id in peer_ids:
		create_player(peer_id)	


func host_game():
	peer = ENetMultiplayerPeer.new()
	peer.create_server(4242)
	multiplayer.set_multiplayer_peer(peer)
	create_player(multiplayer.get_unique_id())
	$"UI/Connect Panel".visible = false


func connect_to_ip(ip):
	peer = ENetMultiplayerPeer.new()
	peer.create_client(ip, 4242)
	multiplayer.set_multiplayer_peer(peer)
	$"UI/Connect Panel".visible = false


func _player_connected(new_peer_id : int):
	if multiplayer.is_server():
		add_newly_connected_player_character.rpc(new_peer_id)
		add_previously_connected_player_characters.rpc_id(new_peer_id, connected_peer_ids)
		create_player(new_peer_id)


func _on_host_button_button_down():
	host_game()


func _on_connect_button_button_down():
	connect_to_ip("localhost")
