class_name NetworkedCharacterInventorySystem
extends "../character/character_inventory_system.gd"


func _ready():
	if Engine.is_editor_hint():
		return
	if is_multiplayer_authority():
		# Setup for enabled/disabled mouse 🖱️😀
		opened_inventory.connect(_update_opened_inventories)
		closed_inventory.connect(_update_opened_inventories)
		opened_station.connect(_update_opened_stations)
		closed_station.connect(_update_opened_stations)
		_update_opened_inventories(main_inventory)
	else:
		picked.connect(_on_picked)
	hotbar.active_slot(0)
	hotbar.active_slot(1)


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


func pick_to_inventory(node : Node):
	if multiplayer.is_server():
		super.pick_to_inventory(node)
	else:
		pick_to_inventory_rpc.rpc_id(1, node.get_path())


func transfer_to(inventory: GridInventory, origin_pos: Vector2i, destination: GridInventory, destination_pos: Vector2i, amount: int, is_rotated: bool):
	if multiplayer.is_server():
		super.transfer_to(inventory, origin_pos, destination, destination_pos, amount, is_rotated)
	else:
		transfer_to_rpc.rpc_id(1, inventory.get_path(), origin_pos, destination.get_path(), destination_pos, amount, is_rotated)


func rotate(stack: ItemStack, inventory: Inventory):
	if multiplayer.is_server():
		super.rotate(stack, inventory)
	else:
		var stack_index = inventory.stacks.find(stack)
		if stack_index != -1:
			rotate_rpc.rpc_id(1, stack_index, inventory.get_path())


func split(inventory : Inventory, stack_index : int, amount : int):
	if multiplayer.is_server():
		super.split(inventory, stack_index, amount)
	else:
		split_rpc.rpc_id(1, inventory.get_path(), stack_index, amount)


func sort(inventory : Inventory):
	if multiplayer.is_server():
		super.sort(inventory)
	else:
		sort_rpc.rpc_id(1, inventory.get_path())


func drop(stack: ItemStack, inventory: Inventory):
	if multiplayer.is_server():
		super.drop(stack, inventory)
	else:
		var stack_index = inventory.stacks.find(stack)
		if stack_index != -1:
			drop_rpc.rpc_id(1, stack_index, inventory.get_path())


func equip(stack: ItemStack, inventory: Inventory, slot_index: int):
	if multiplayer.is_server():
		super.equip(stack, inventory, slot_index)
	else:
		var stack_index = inventory.stacks.find(stack)
		if stack_index != -1:
			equip_rpc.rpc_id(1, stack_index, inventory.get_path(), slot_index)


func hotbar_change_selection(index : int):
	if multiplayer.is_server():
		super.hotbar_change_selection(index)
	else:
		hotbar_change_selection_rpc.rpc_id(1, index)


func hotbar_previous_item():
	if multiplayer.is_server():
		super.hotbar_previous_item()
	else:
		hotbar_previous_item_rpc.rpc_id(1)
	

func hotbar_next_item():
	if multiplayer.is_server():
		super.hotbar_next_item()
	else:
		hotbar_next_item_rpc.rpc_id(1)


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
func pick_to_inventory_rpc(node_path: NodePath):
	super.pick_to_inventory(get_node(node_path))


@rpc
func transfer_to_rpc(inventory_path: NodePath, origin_pos: Vector2i, destination_path: NodePath, destination_pos: Vector2i, amount: int, is_rotated: bool):
	var inv = get_node(inventory_path)
	var dest_inv = get_node(destination_path)
	if inv == null or dest_inv == null:
		return
	super.transfer_to(inv, origin_pos, dest_inv, destination_pos, amount, is_rotated)


@rpc
func split_rpc(inventory_path: NodePath, stack_index: int, amount: int):
	var inv = get_node(inventory_path)
	if inv == null:
		return
	super.split(inv, stack_index, amount)


@rpc
func rotate_rpc(stack_index: int, inventory_path: NodePath):
	var inv = get_node(inventory_path)
	if inv == null:
		return
	var stack = inv.stacks[stack_index]
	super.rotate(stack, inv)


@rpc
func sort_rpc(inventory_path: NodePath):
	var inv = get_node(inventory_path)
	if inv == null:
		return
	super.sort(inv)


@rpc
func drop_rpc(stack_index: int, inventory_path: NodePath):
	var inv = get_node(inventory_path)
	if inv == null:
		return
	var stack = inv.stacks[stack_index]
	super.drop(stack, inv)


@rpc
func equip_rpc(stack_index: int, inventory_path: NodePath, slot_index: int):
	var inv = get_node(inventory_path)
	if inv == null:
		return
	var stack = inv.stacks[stack_index]
	super.equip(stack, inv, slot_index)


@rpc
func hotbar_change_selection_rpc(selection_index: int):
	if not multiplayer.is_server():
		return
	super.hotbar_change_selection(selection_index)


@rpc
func hotbar_previous_item_rpc():
	if not multiplayer.is_server():
		return
	super.hotbar_previous_item()


@rpc
func hotbar_next_item_rpc():
	if not multiplayer.is_server():
		return
	super.hotbar_next_item()

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
	

func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	if multiplayer.multiplayer_peer != null and is_multiplayer_authority():
		interactor.try_interact()
