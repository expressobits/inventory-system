class_name NetworkedCharacterInventorySystem
extends "../character/character_inventory_system.gd"

@export_node_path var drop_item_spawner_path := NodePath("../../DroppedItemSpawner")
@onready var drop_item_spawner = get_node(drop_item_spawner_path)


func _ready():
	slot_holder = Slot.new()
	main_inventory.request_drop_obj.connect(_on_request_drop_obj)
	equipment_inventory.request_drop_obj.connect(_on_request_drop_obj)
	slot_holder.updated.connect(_on_slot_holder_updated)
	if is_multiplayer_authority():
		# Setup for enabled/disabled mouse 🖱️😀
		opened_inventory.connect(_update_opened_inventories)
		closed_inventory.connect(_update_opened_inventories)
		opened_station.connect(_update_opened_stations)
		closed_station.connect(_update_opened_stations)
		_update_opened_inventories(main_inventory)
		
	else:
		picked.connect(_on_picked)


func _on_picked(obj : Node):
	picked_rpc.rpc(obj.get_path())


func _input(event : InputEvent):
	if Engine.is_editor_hint():
		return
	if check_inputs and is_multiplayer_authority():
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
		open_inventory_rpc.rpc_id(1, inventory.get_path())


func add_open_inventory(inventory : Inventory):
	if multiplayer.is_server():
		add_open_inventory_rpc.rpc(inventory.get_path())
	super.add_open_inventory(inventory)


func remove_open_inventory(inventory : Inventory):
	if multiplayer.is_server():
		remove_open_inventory_rpc.rpc(inventory.get_path())
	super.remove_open_inventory(inventory)


func close_inventories():
	if multiplayer.is_server():
		super.close_inventories()
	else:
		close_inventories_rpc.rpc_id(1)


func to_holder(slot_index : int, inventory : Inventory, amount : int):
	if multiplayer.is_server():
		super.to_holder(slot_index, inventory, amount)
	else:
		to_holder_rpc.rpc_id(1, slot_index, get_path_to(inventory), amount)


func holder_to(inventory : Inventory):
	if multiplayer.is_server():
		super.holder_to(inventory)
	else:
		holder_to_rpc.rpc_id(1, get_path_to(inventory))


func holder_to_at(slot_index : int, inventory : Inventory, amount_to_move : int = -1):
	if multiplayer.is_server():
		super.holder_to_at(slot_index, inventory, amount_to_move)
	else:
		holder_to_at_rpc.rpc_id(1, slot_index, get_path_to(inventory), amount_to_move)


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
		open_station_rpc(get_path_to(craft_station))
	else:
		open_station_rpc.rpc(get_path_to(craft_station))


func add_open_station(craft_station : CraftStation):
	if multiplayer.is_server():
		add_open_station_rpc.rpc(craft_station.get_path())
	super.add_open_station(craft_station)


func remove_open_station(craft_station : CraftStation):
	if multiplayer.is_server():
		remove_open_station_rpc.rpc(craft_station.get_path())
	super.remove_open_station(craft_station)


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


func drop_holder():
	if multiplayer.is_server():
		drop_holder_rpc()
	else:
		drop_holder_rpc.rpc()


func _on_request_drop_obj(dropped_item : String, item_id : String):
	if(drop_item_spawner == null):
		printerr("Spawner is null!")
		return
	var packed_scene : PackedScene = load(dropped_item)
	var data : Array
	data.append(drop_parent_position.position)
	data.append(drop_parent_position.rotation)
	data.append(packed_scene.resource_path)
	data.append(Dictionary())
	var obj = drop_item_spawner.spawn(data)
	dropped.emit(obj)


func _on_slot_holder_updated():
	if not multiplayer.is_server():
		return
	var item_id = slot_holder.get_item_id()
	var amount = slot_holder.get_amount()
	slot_holder_updated_rpc.rpc(item_id, amount)


@rpc("any_peer")
func picked_rpc(obj_path : NodePath):
	var obj = get_node(obj_path)
	picked.emit(obj)


@rpc("any_peer")
func open_main_inventory_rpc():
	super.open_main_inventory()


@rpc
func open_inventory_rpc(inventory_path : NodePath):
	var inventory = get_node(inventory_path)
	super.open_inventory(inventory)


@rpc("any_peer")
func add_open_inventory_rpc(inventory_path : NodePath):
	var inventory = get_node(inventory_path)
	super.add_open_inventory(inventory)


@rpc("any_peer")
func remove_open_inventory_rpc(inventory_path : NodePath):
	var inventory = get_node(inventory_path)
	super.remove_open_inventory(inventory)


@rpc("any_peer")
func add_open_station_rpc(station_path : NodePath):
	var station = get_node(station_path)
	super.add_open_station(station)


@rpc("any_peer")
func remove_open_station_rpc(station_path : NodePath):
	var station = get_node(station_path)
	super.remove_open_station(station)


@rpc
func close_inventories_rpc():
	if multiplayer.is_server():
		super.close_inventories()


@rpc
func to_holder_rpc(slot_index : int, inventory_path : NodePath, amount : int):
	var inventory : Inventory = get_node(inventory_path)
	super.to_holder(slot_index, inventory, amount)


@rpc
func holder_to_rpc(inventory_path : NodePath):
	var inventory : Inventory = get_node(inventory_path)
	super.holder_to(inventory)


@rpc
func holder_to_at_rpc(slot_index : int, inventory_path : NodePath, amount_to_move : int):
	var inventory : Inventory = get_node(inventory_path)
	super.holder_to_at(slot_index, inventory, amount_to_move)


@rpc("any_peer")
func slot_holder_updated_rpc(item_id : String, amount : int):
	change_holder(item_id, amount)


@rpc
func pick_to_inventory_rpc(node_path : NodePath):
	super.pick_to_inventory(get_node(node_path))


@rpc
func open_main_craft_station_rpc():
	super.open_main_craft_station()


@rpc
func open_station_rpc(craft_station_path : NodePath):
	var station = get_node(craft_station_path)
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
	hotbar.selection_index = index

@rpc
func hotbar_previous_item_rpc():
	hotbar.previous_item()
	
@rpc	
func hotbar_next_item_rpc():
	hotbar.next_item()

@rpc
func drop_holder_rpc():
	super.drop_holder()
	

func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	if multiplayer.multiplayer_peer != null and is_multiplayer_authority():
		interactor.try_interact()
