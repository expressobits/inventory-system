class_name NetworkedCharacterInventorySystem
extends CharacterInventorySystem

func _ready():
	inventory_handler.picked.connect(_on_inventory_handler_picked.bind())
	inventory_handler.dropped.connect(_on_inventory_handler_dropped.bind())
	inventory_handler.get_inventory(0).opened.connect(_on_player_inventory_opened.bind())
	inventory_handler.get_inventory(0).closed.connect(_on_player_inventory_closed.bind())
	if is_multiplayer_authority():
		#InventorySystem.setup_inventory_handler(inventory_handler)
		#InventorySystem.setup_hotbar(hotbar)
		#InventorySystem.setup_crafter(crafter)
		#InventorySystem.setup_interactor(interactor)
		
		# Setup for enabled/disabled mouse 🖱️😀
		inventory_handler.opened.connect(_update_opened_inventories.bind())
		inventory_handler.closed.connect(_update_opened_inventories.bind())
		crafter.opened.connect(_update_opened_stations.bind())
		crafter.closed.connect(_update_opened_stations.bind())
		_update_opened_inventories(inventory_handler.get_inventory(0))


func close_craft_stations():
	if multiplayer.is_server():
		super.close_craft_stations()
	else:
		close_stations_rpc.rpc_id(1)

func _input(event : InputEvent):
	if Engine.is_editor_hint():
		return
	if is_multiplayer_authority():
		hot_bar_inputs(event)
		inventory_inputs()


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
