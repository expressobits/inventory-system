@tool
@icon("res://addons/inventory-system/icons/character_inventory_system.svg")
class_name CharacterInventorySystem
extends NodeInventories

signal dropped(node : Node)

@export_group("🗃️ Inventory Nodes")
@export_node_path("InventoryHandler") var inventory_handler_path := NodePath("InventoryHandler")
@onready var inventory_handler : InventoryHandler = get_node(inventory_handler_path)
@export_node_path("Hotbar") var hotbar_path := NodePath("Hotbar")
@onready var hotbar : Hotbar = get_node(hotbar_path)
@export_node_path("Crafter") var crafter_path := NodePath("Crafter")
@onready var crafter : Crafter = get_node(crafter_path)
@export_node_path("Interactor") var interactor_path := NodePath("Interactor")
@onready var interactor : Interactor = get_node(interactor_path)
@export_node_path var drop_parent_path := NodePath("../..");
@onready var drop_parent : Node = get_node(drop_parent_path)
@export_node_path var drop_parent_position_path := NodePath("..");
@onready var drop_parent_position : Node = get_node(drop_parent_position_path)


@export_group("⌨️ Inputs")
## Change mouse state based on inventory status
@export var change_mouse_state : bool = true
@export var toggle_inventory_input : String = "toggle_inventory"
@export var exit_inventory_and_craft_panel_input : String = "escape"
@export var toggle_craft_panel_input : String = "toggle_craft_panel"


@export_group("🫴 Interact")
@export var can_interact : bool = true
@export var raycast : RayCast3D:
	set(value):
		raycast = value
		var interactor = get_node(interactor_path)
		if interactor != null and value != null:
			interactor.raycast_path = interactor.get_path_to(value)
@export var camera_3d : Camera3D:
	set(value):
		camera_3d = value
		var interactor = get_node(interactor_path)
		if interactor != null and value != null:
			interactor.camera_path = interactor.get_path_to(value)


func _ready():
	if Engine.is_editor_hint():
		return
	inventory_handler.request_drop_obj.connect(_on_request_drop_obj.bind())
	
	# Setup for enabled/disabled mouse 🖱️😀
	if change_mouse_state:
		inventory_handler.opened.connect(_update_opened_inventories.bind())
		inventory_handler.closed.connect(_update_opened_inventories.bind())
		crafter.opened.connect(_update_opened_stations.bind())
		crafter.closed.connect(_update_opened_stations.bind())
		_update_opened_inventories(inventory_handler.get_inventory(0))


func _input(event : InputEvent) -> void:
	if Engine.is_editor_hint():
		return
	hot_bar_inputs(event)
	inventory_inputs()


func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	interactor.try_interact()


func _update_opened_inventories(_inventory : Inventory):
	if inventory_handler.is_open_main_inventory():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _update_opened_stations(_craft_station : CraftStation):
	if crafter.is_open_any_station():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func inventory_inputs():
	if Input.is_action_just_released(toggle_inventory_input):
		if not inventory_handler.is_open_any_inventory() and not crafter.is_open_any_station():
			open_main_inventory()
	
	if Input.is_action_just_released(exit_inventory_and_craft_panel_input):
		close_inventories()
		close_craft_stations()
			
	if Input.is_action_just_released(toggle_craft_panel_input):
		if not inventory_handler.is_open_any_inventory() and not crafter.is_open_any_station():
			open_main_craft_station()


## Inventories/Handler
func open_main_inventory():
	inventory_handler.open_main_inventory()


func open_inventory(inventory : Inventory):
	if not inventory_handler.is_open(inventory):
		inventory_handler.open(inventory)
		if not inventory_handler.is_open_main_inventory():
			inventory_handler.open_main_inventory()


func close_inventories():
	inventory_handler.close_all_inventories()


func move_between_inventories_at(from : Inventory, from_slot_index : int, amount : int, to : Inventory, to_slot_index : int):
	inventory_handler.move_between_inventories_at(from, from_slot_index, amount, to, to_slot_index)


func to_transaction(slot_index : int, inventory : Inventory, amount : int):
	inventory_handler.to_transaction(slot_index, inventory, amount)


func transaction_to(inventory : Inventory):
	inventory_handler.transaction_to(inventory)


func transaction_to_at(slot_index : int, inventory : Inventory, amount_to_move : int = -1):
	inventory_handler.transaction_to_at(slot_index, inventory, amount_to_move)


func pick_to_inventory(node : Node):
	inventory_handler.pick_to_inventory(node)

## Crafter
func craft(craft_station : CraftStation, recipe_index : int):
	craft_station.craft(recipe_index)


func open_main_craft_station():
	inventory_handler.open_main_inventory()


func close_craft_stations():
	if crafter.is_open_any_station():
		crafter.close_all_craft_stations()


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
	hotbar.change_selection(index)


func hotbar_previous_item():
	hotbar.previous_item()
	

func hotbar_next_item():
	hotbar.next_item()


func drop_transaction():
	inventory_handler.drop_transaction()


func _on_request_drop_obj(dropped_item : String, item : Item):
	var packed_scene : PackedScene = load(dropped_item)
	var node = packed_scene.instantiate()
	drop_parent.add_child(node)
	node.set("item", item)
	node.set("position", drop_parent_position.get("position"))
	node.set("rotation", drop_parent_position.get("position"))
	dropped.emit(node)


func open_station(craft_station : CraftStation):
	if not crafter.is_open(craft_station):
		crafter.open(craft_station)
