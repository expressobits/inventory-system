class_name CharacterInventorySystem
extends Node

@export_node_path("InventoryHandler") var inventory_handler_path = NodePath("InventoryHandler")
@export_node_path("Hotbar") var hotbar_path = NodePath("InventoryHandler/Hotbar")
@export_node_path("Crafter") var crafter_path = NodePath("Crafter")
@onready var inventory_handler : InventoryHandler = get_node(inventory_handler_path)
@onready var hotbar : Hotbar = get_node(hotbar_path)
@onready var crafter : Crafter = get_node(crafter_path)


func _ready():
	inventory_handler.picked.connect(_on_inventory_handler_picked.bind())
	inventory_handler.dropped.connect(_on_inventory_handler_dropped.bind())
	inventory_handler.inventory.opened.connect(_on_player_inventory_opened.bind())
	inventory_handler.inventory.closed.connect(_on_player_inventory_closed.bind())
	hotbar.on_change_selection.connect(_on_hotbar_changed.bind())
	InventorySystem.setup_inventory_handler(inventory_handler)
	InventorySystem.setup_crafter(crafter)
	
	# Setup for enabled/disabled mouse 🖱️😀
	inventory_handler.opened.connect(_update_opened_inventories.bind())
	inventory_handler.closed.connect(_update_opened_inventories.bind())
	crafter.opened.connect(_update_opened_stations.bind())
	crafter.closed.connect(_update_opened_stations.bind())
	_update_opened_inventories(inventory_handler.inventory)


func _update_opened_inventories(_inventory : Inventory):
	if InventorySystem.inventory_handler.is_open_main_inventory():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _update_opened_stations(_craft_station : CraftStation):
	if InventorySystem.crafter.is_open_any_station():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _process(_delta):
	if Input.is_action_just_released("toggle_inventory"):
		if inventory_handler.is_open_main_inventory():
			inventory_handler.close_main_inventory()
			inventory_handler.close_all_inventories()
		else:
			inventory_handler.open_main_inventory()
	
	if Input.is_action_just_released("escape"):
		if inventory_handler.is_open_main_inventory():
			inventory_handler.close_main_inventory()
			inventory_handler.close_all_inventories()
		if crafter.is_open_any_station():
			crafter.close_all_craft_stations()
			
	if Input.is_action_just_released("toggle_craft_panel"):
		if crafter.is_open_main_craft_station():
			crafter.close_main_craft_station()
			crafter.close_all_craft_stations()
		else:
			crafter.open_main_craft_station()


func _input(event: InputEvent) -> void:
	if event is InputEventMouseButton:
		if event.is_pressed():
			if event.button_index == MOUSE_BUTTON_WHEEL_UP:
				hotbar.next_item()
			if event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
				hotbar.previous_item()
	if event is InputEventKey:
		var input_key_event = event as InputEventKey
		if event.is_pressed() and not event.is_echo():
			if input_key_event.keycode > KEY_0 and input_key_event.keycode < KEY_9:
				hotbar.set_selection_index(input_key_event.keycode - KEY_1)


func open_inventory(inventory : Inventory):
	if not inventory_handler.is_open(inventory):
		inventory_handler.open(inventory)
		if not inventory_handler.is_open_main_inventory():
			inventory_handler.open_main_inventory()


func open_station(craft_station : CraftStation):
	if not crafter.is_open(craft_station):
		crafter.open(craft_station)


func _on_inventory_handler_picked(_dropped_item):
	$Pickup.play()


func _on_inventory_handler_dropped(_dropped_item):
	$Drop.play()
	

func _on_player_inventory_opened():
	$PlayerInventoryOpen.play()


func _on_player_inventory_closed():
	$PlayerInventoryClose.play()


func _on_hotbar_changed(_selection : int):
	$HotbarChange.play()
