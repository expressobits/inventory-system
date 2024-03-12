class_name NetworkedCharacterInventorySystem
extends "../extras/character_inventory_system.gd"

@export_node_path var drop_item_spawner_path := NodePath("../../DroppedItemSpawner")
@onready var drop_item_spawner = get_node(drop_item_spawner_path)


func _ready():
	inventory_handler.request_drop_obj.connect(_on_request_drop_obj.bind())
	if is_multiplayer_authority():
		# Setup for enabled/disabled mouse 🖱️😀
		inventory_handler.opened.connect(_update_opened_inventories.bind())
		inventory_handler.closed.connect(_update_opened_inventories.bind())
		crafter.opened.connect(_update_opened_stations.bind())
		crafter.closed.connect(_update_opened_stations.bind())
		_update_opened_inventories(inventory_handler.get_inventory(0))


func _input(event : InputEvent):
	if Engine.is_editor_hint():
		return
	if is_multiplayer_authority():
		hot_bar_inputs(event)
		inventory_inputs()


func open_main_inventory():
	if multiplayer.is_server():
		super.open_main_inventory()
	else:
		open_main_inventory_rpc.rpc_id(1)


func open_inventory(inventory : Inventory):
	if multiplayer.is_server():
		super.open_inventory(inventory)
	else:
		open_inventory_rpc.rpc_id(1, inventory_handler.get_path_to(inventory))


func close_inventories():
	if multiplayer.is_server():
		super.close_inventories()
	else:
		close_inventories_rpc.rpc_id(1)


func move_between_inventories_at(from : Inventory, from_slot_index : int, amount : int, to : Inventory, to_slot_index : int):
	if multiplayer.is_server():
		super.move_between_inventories_at(from, from_slot_index, amount, to, to_slot_index)
	else:
		var from_path = inventory_handler.get_path_to(from)
		var to_path = inventory_handler.get_path_to(to)
		move_between_inventories_at_rpc.rpc_id(1, from_path, from_slot_index, amount, to_path, to_slot_index)


func to_transaction(slot_index : int, inventory : Inventory, amount : int):
	if multiplayer.is_server():
		super.to_transaction(slot_index, inventory, amount)
	else:
		to_transaction_rpc.rpc_id(1, slot_index, inventory_handler.get_path_to(inventory), amount)


func transaction_to(inventory : Inventory):
	if multiplayer.is_server():
		super.transaction_to(inventory)
	else:
		transaction_to_rpc.rpc_id(1, inventory_handler.get_path_to(inventory))


func transaction_to_at(slot_index : int, inventory : Inventory, amount_to_move : int = -1):
	if multiplayer.is_server():
		super.transaction_to_at(slot_index, inventory, amount_to_move)
	else:
		transaction_to_at_rpc.rpc_id(1, slot_index, inventory_handler.get_path_to(inventory), amount_to_move)


func pick_to_inventory(node : Node):
	if multiplayer.is_server():
		super.pick_to_inventory(node)
	else:
		pick_to_inventory_rpc.rpc_id(1, node.get_path())


func open_main_craft_station():
	if multiplayer.is_server():
		super.open_main_craft_station()
	else:
		open_main_craft_station_rpc.rpc_id(1)


func close_craft_stations():
	if multiplayer.is_server():
		super.close_craft_stations()
	else:
		close_stations_rpc.rpc_id(1)


func craft(craft_station : CraftStation, recipe_index : int):
	if multiplayer.is_server():
		craft_rpc(craft_station.get_path(), recipe_index)
	else:
		craft_rpc.rpc(craft_station.get_path(), recipe_index)
		

func open_station(craft_station : CraftStation):
	if multiplayer.is_server():
		open_station_rpc(crafter.get_path_to(craft_station))
	else:
		open_station_rpc.rpc(crafter.get_path_to(craft_station))


func hotbar_change_selection(index : int):
	if multiplayer.is_server():
		hotbar_change_selection_rpc(index)
	else:
		hotbar_change_selection_rpc.rpc(index)


func hotbar_previous_item():
	if multiplayer.is_server():
		hotbar_previous_item_rpc()
	else:
		hotbar_previous_item_rpc.rpc()


func hotbar_next_item():
	if multiplayer.is_server():
		hotbar_next_item_rpc()
	else:
		hotbar_next_item_rpc.rpc()


func drop_transaction():
	if multiplayer.is_server():
		drop_transaction_rpc()
	else:
		drop_transaction_rpc.rpc()


func _on_request_drop_obj(dropped_item : String, item : Item):
	if(drop_item_spawner == null):
		printerr("Spawner is null!")
		return
	var packed_scene : PackedScene = load(dropped_item)
	var data : Array
	data.append(drop_parent_position.position)
	data.append(drop_parent_position.rotation)
	data.append(packed_scene.resource_path)
	data.append(item.properties)
	var obj = drop_item_spawner.spawn(data)
	dropped.emit(obj)


@rpc
func open_main_inventory_rpc():
	super.open_main_inventory()


@rpc
func open_inventory_rpc(inventory_path : NodePath):
	var inventory = inventory_handler.get_node(inventory_path)
	super.open_inventory(inventory)
	

@rpc
func close_inventories_rpc():
	if multiplayer.is_server():
		super.close_inventories()


@rpc
func move_between_inventories_at_rpc(from_path : NodePath, from_slot_index : int, amount : int, to_path : NodePath, to_slot_index : int):
	var from : Inventory = inventory_handler.get_node(from_path)
	var to : Inventory = inventory_handler.get_node(to_path)
	super.move_between_inventories_at(from, from_slot_index, amount, to, to_slot_index)


@rpc
func to_transaction_rpc(slot_index : int, inventory_path : NodePath, amount : int):
	var inventory : Inventory = inventory_handler.get_node(inventory_path)
	super.to_transaction(slot_index, inventory, amount)


@rpc
func transaction_to_rpc(inventory_path : NodePath):
	var inventory : Inventory = inventory_handler.get_node(inventory_path)
	super.transaction_to(inventory)


@rpc
func transaction_to_at_rpc(slot_index : int, inventory_path : NodePath, amount_to_move : int):
	var inventory : Inventory = inventory_handler.get_node(inventory_path)
	super.transaction_to_at(slot_index, inventory, amount_to_move)


@rpc
func pick_to_inventory_rpc(node_path : NodePath):
	super.pick_to_inventory(get_node(node_path))


@rpc
func open_main_craft_station_rpc():
	super.open_main_craft_station()


@rpc
func open_station_rpc(craft_station_path : NodePath):
	var station = crafter.get_node(craft_station_path)
	super.open_station(station)


@rpc
func close_stations_rpc():
	if multiplayer.is_server():
		super.close_craft_stations()


@rpc
func craft_rpc(craft_station_path : NodePath, recipe_index : int):
	var station = get_node(craft_station_path)
	station.craft(recipe_index)
	
@rpc
func hotbar_change_selection_rpc(index : int):
	hotbar.change_selection(index)

@rpc
func hotbar_previous_item_rpc():
	hotbar.previous_item()
	
@rpc	
func hotbar_next_item_rpc():
	hotbar.next_item()

@rpc
func drop_transaction_rpc():
	super.drop_transaction()
	

func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	if is_multiplayer_authority():
		interactor.try_interact()
