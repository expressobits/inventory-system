extends FPSDemo
class_name MPDemo

@export var player_scene : PackedScene

var connected_peer_ids : Array
var players : Array
var peer : ENetMultiplayerPeer

func _ready():
	multiplayer.peer_connected.connect(_player_connected)
	multiplayer.peer_disconnected.connect(_player_disconnected)

func _process(delta: float) -> void:
	pass

func create_player(peer_id : int):
	connected_peer_ids.append(peer_id)
	var player = player_scene.instantiate()
	player.name = str(peer_id)
	players.append(player)
	player.position = Vector3(0,2,0)
	add_child(player)
	if multiplayer.get_unique_id() == peer_id:
		$"UI/Inventory System UI".setup(player.get_node("CharacterInventorySystem"))


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
	await get_tree().create_timer(1).timeout
	make_scene_objects_to_network()


func make_scene_objects_to_network():
	var items = $Level/Items
	var spawner = get_node("DroppedItemSpawner")
	for i in items.get_child_count():
		var item_dropped = items.get_child(i) as DroppedItem3D
		var item_id : String = item_dropped.item_id
		var definition = database.get_item(item_id)
		var position = item_dropped.position
		var rotation = item_dropped.rotation
		var amount = item_dropped.amount
		item_dropped.queue_free()
		var dropped_item_path = definition.properties["dropped_item"]
		var _obj = spawner.spawn([position, rotation, dropped_item_path, amount])


func connect_to_ip(ip):
	peer = ENetMultiplayerPeer.new()
	peer.create_client(ip, 4242)
	multiplayer.set_multiplayer_peer(peer)
	$"UI/Connect Panel".visible = false
	var items = $Level/Items
	for i in items.get_child_count():
		var item = items.get_child(i) as DroppedItem3D
		item.queue_free()


func _player_connected(new_peer_id : int):
	if multiplayer.is_server():
		add_newly_connected_player_character.rpc(new_peer_id)
		add_previously_connected_player_characters.rpc_id(new_peer_id, connected_peer_ids)
		create_player(new_peer_id)


func _player_disconnected(peer_id : int):
	var pos = connected_peer_ids.find(peer_id)
	if pos > 0 and pos < connected_peer_ids.size():
		connected_peer_ids.remove_at(pos)
		var player = players[pos]
		players.remove_at(pos)
		player.queue_free()


func _on_host_button_button_down():
	host_game()


func _on_connect_button_button_down():
	connect_to_ip("localhost")
