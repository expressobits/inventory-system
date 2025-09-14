@tool
@icon("res://addons/inventory-system-demos/icons/character_inventory_system.svg")
class_name CharacterInventorySystem
extends NodeInventories

signal opened_station(station : CraftStation)
signal closed_station(station : CraftStation)
signal opened_inventory(inventory : Inventory)
signal closed_inventory(inventory : Inventory)
signal picked(obj : Node)

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export_group("🗃️ Inventory Nodes")
@export_node_path var main_inventory_path := NodePath("InventoryHandler/Inventory")
@onready var main_inventory : GridInventory = get_node(main_inventory_path)
@export_node_path var equipment_inventory_path := NodePath("InventoryHandler/EquipmentInventory")
@onready var equipment_inventory : GridInventory = get_node(equipment_inventory_path)
@export_node_path("Hotbar") var hotbar_path := NodePath("Hotbar")
@onready var hotbar : Hotbar = get_node(hotbar_path)
@export_node_path("CraftStation") var main_station_path := NodePath("CraftStation")
@onready var main_station : CraftStation = get_node(main_station_path)
@export_node_path var interactor_path := NodePath("Interactor")
@onready var interactor : Interactor = get_node(interactor_path)
@export_node_path var drop_parent_path := NodePath("../..");
@onready var drop_parent : Node = get_node(drop_parent_path)
@export_node_path var drop_parent_position_path := NodePath("..");
@onready var drop_parent_position : Node = get_node(drop_parent_position_path)

var opened_stations : Array[CraftStation]
var opened_inventories : Array[Inventory]

@export_group("⌨️ Inputs")
## Change mouse state based on inventory status
@export var change_mouse_state : bool = true
@export var check_inputs : bool = true
@export var toggle_inventory_input : String = "toggle_inventory"
@export var exit_inventory_and_craft_panel_input : String = "escape"
@export var toggle_craft_panel_input : String = "toggle_craft_panel"


@export_group("🫴 Interact")
@export var can_interact : bool = true
@export var raycast : RayCast3D:
	set(value):
		raycast = value
		var _interactor = get_node(interactor_path)
		if _interactor != null and value != null:
			_interactor.raycast = value
@export var camera_3d : Camera3D:
	set(value):
		camera_3d = value
		var _interactor = get_node(interactor_path)
		if _interactor != null and value != null:
			_interactor.camera = value


func _ready():
	if Engine.is_editor_hint():
		return
	hotbar.active_slot(0)
	hotbar.active_slot(1)
	hotbar.active_slot(2)
	hotbar.active_slot(3)
	hotbar.active_slot(4)
	hotbar.active_slot(5)
	hotbar.active_slot(6)
	hotbar.active_slot(7)
	## Setup for enabled/disabled mouse 🖱️😀
	if change_mouse_state:
		opened_inventory.connect(_update_opened_inventories)
		closed_inventory.connect(_update_opened_inventories)
		opened_station.connect(_update_opened_stations)
		closed_station.connect(_update_opened_stations)
		_update_opened_inventories(main_inventory)


func _input(event : InputEvent) -> void:
	if Engine.is_editor_hint():
		return
	if check_inputs:
		hot_bar_inputs(event)
		inventory_inputs()


func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	interactor.try_interact()


func is_any_station_or_inventory_opened() -> bool:
	return is_open_any_station() or is_open_main_inventory()


func _update_opened_inventories(_inventory : Inventory):
	_check_inputs()


func _update_opened_stations(_craft_station : CraftStation):
	_craft_station.load_valid_recipes()
	_check_inputs()


func _check_inputs():
	if is_any_station_or_inventory_opened():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func inventory_inputs():
	if Input.is_action_just_released(toggle_inventory_input):
		if not is_any_station_or_inventory_opened():
			open_main_inventory()
	
	if Input.is_action_just_released(exit_inventory_and_craft_panel_input):
		close_inventories()
		close_craft_stations()
			
	if Input.is_action_just_released(toggle_craft_panel_input):
		if not is_any_station_or_inventory_opened():
			open_main_craft_station()

func pick_to_inventory(node : Node):
	if main_inventory == null:
		return
	
	if node == null:
		return

	if !node.get("is_pickable"):
		return
		
	var item_id = node.item_id
	var item_properties = node.item_properties
	var amount = node.amount
	
	if main_inventory.add(item_id, amount, item_properties, true) == 0:
		picked.emit(node)
		node.queue_free();
		return
		
	printerr("pick_to_inventory return false");

func transfer(inventory: GridInventory, origin_pos: Vector2i, destination: GridInventory, amount: int):
	var stack_index = inventory.get_stack_index_at(origin_pos)
	if stack_index == -1:
		return
	inventory.transfer(stack_index, destination, amount)


func transfer_to(inventory: GridInventory, origin_pos: Vector2i, destination: GridInventory, destination_pos: Vector2i, amount: int, is_rotated: bool):
	inventory.transfer_to(origin_pos, destination, destination_pos, amount, is_rotated)


func split(inventory : Inventory, stack_index : int, amount : int):
	inventory.split(stack_index, amount)


func equip(stack: ItemStack, _inventory : Inventory, slot_index: int):
	hotbar.equip(stack, slot_index)


func rotate(stack: ItemStack, inventory : Inventory):
	inventory.rotate(stack)


func sort(inventory : Inventory):
	inventory.sort()


func drop(stack: ItemStack, inventory: Inventory):
	var stack_index = inventory.stacks.find(stack)
	if stack_index == -1:
		return
	
	inventory.drop_from_inventory(stack_index, stack.amount, stack.properties)
	
	
func drop_all_items():
	main_inventory.drop_all_stacks()
	equipment_inventory.drop_all_stacks()

#endregion

#region Crafter
func craft(craft_station : CraftStation, recipe_index : int):
	craft_station.craft(recipe_index)

#endregion

#region Hotbar
func hot_bar_inputs(event : InputEvent):
	if event is InputEventMouseButton:
		if event.is_pressed():
			if event.button_index == MOUSE_BUTTON_WHEEL_UP:
				hotbar_previous_item()
			if event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
				hotbar_next_item()
	if event is InputEventKey:
		var input_key_event = event as InputEventKey
		if event.is_pressed() and not event.is_echo():
			if input_key_event.keycode > KEY_0 and input_key_event.keycode < KEY_9:
				hotbar_change_selection(input_key_event.keycode - KEY_1)


func hotbar_change_selection(index : int):
	if hotbar.selection_index == index:
		index = -1
	hotbar.selection_index = index


func hotbar_previous_item():
	hotbar.previous_item()
	

func hotbar_next_item():
	hotbar.next_item()

#endregion

#region Open Inventories
func is_open_inventory(inventory : Inventory):
	return opened_inventories.find(inventory) != -1

func open_inventory(inventory : Inventory):
	if is_open_inventory(inventory):
		return
	add_open_inventory(inventory)


func add_open_inventory(inventory : Inventory):
	opened_inventories.append(inventory)
	opened_inventory.emit(inventory)
	if not is_open_main_inventory():
		#inventory.request_drop_obj.connect(_on_request_drop_obj)
		open_main_inventory()
	
func open_main_inventory():
	open_inventory(main_inventory)
	
	
func close_inventory(inventory : Inventory):
	if main_inventory != inventory:
		inventory.get_parent().close(get_parent())
	remove_open_inventory(inventory)


func remove_open_inventory(inventory : Inventory):
	var index = opened_inventories.find(inventory)
	opened_inventories.remove_at(index)
	closed_inventory.emit(inventory)
	#if main_inventory != inventory:
		#inventory.request_drop_obj.disconnect(_on_request_drop_obj)


func close_inventories():
	for index in range(opened_inventories.size() - 1, -1, -1):
		close_inventory(opened_inventories[index])


func is_open_any_inventory():
	return !opened_inventories.is_empty()
	
func is_open_main_inventory():
	return is_open_inventory(main_inventory)
#endregion

#region Open Craft Stations
func is_open_station(station : CraftStation):
	return opened_stations.find(station) != -1


func open_station(station : CraftStation):
	if is_open_station(station):
		return
	add_open_station(station)


func add_open_station(station : CraftStation):
	opened_stations.append(station)
	opened_station.emit(station)


func close_station(station : CraftStation):
	if not is_open_station(station):
		return
	remove_open_station(station)


func remove_open_station(station : CraftStation):
	var index = opened_stations.find(station)
	opened_stations.remove_at(index)
	closed_station.emit(station)
	if main_station != station:
		station.get_parent().close(get_parent())


func open_main_craft_station():
	open_station(main_station)


func close_craft_stations():
	for index in range(opened_stations.size() - 1, -1, -1):
		close_station(opened_stations[index])

func is_open_any_station():
	return !opened_stations.is_empty()
#endregion
