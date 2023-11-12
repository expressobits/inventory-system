@tool
@icon("res://addons/inventory-system/icons/character_inventory_system.svg")
class_name CharacterInventorySystem
extends NodeInventorySystemBase


@export_group("🗃️ Inventory Nodes")
@export_node_path("InventoryHandler") var inventory_handler_path := NodePath("InventoryHandler")
@onready var inventory_handler : InventoryHandler = get_node(inventory_handler_path)
@export_node_path("Hotbar") var hotbar_path := NodePath("Hotbar")
@onready var hotbar : Hotbar = get_node(hotbar_path)
@export_node_path("Crafter") var crafter_path := NodePath("Crafter")
@onready var crafter : Crafter = get_node(crafter_path)
@export_node_path("InventoryInteractor") var interactor_path := NodePath("InventoryInteractor")
@onready var interactor : InventoryInteractor = get_node(interactor_path)


@export_group("🔊 Audios")
@export_node_path("AudioStreamPlayer3D") var picked_audio_path := NodePath("PickupAudio")
@onready var picked_audio : AudioStreamPlayer3D = get_node(picked_audio_path)
@export_node_path("AudioStreamPlayer3D") var drop_audio_path := NodePath("DropAudio")
@onready var drop_audio : AudioStreamPlayer3D = get_node(drop_audio_path)
@export_node_path("AudioStreamPlayer3D") var hotbar_change_audio_path := NodePath("HotbarChangeAudio")
@onready var hotbar_change_audio : AudioStreamPlayer3D = get_node(hotbar_change_audio_path)
@export_node_path("AudioStreamPlayer3D") var player_inventory_open_audio_path := NodePath("PlayerInventoryOpenAudio")
@onready var player_inventory_open_audio : AudioStreamPlayer3D = get_node(player_inventory_open_audio_path)
@export_node_path("AudioStreamPlayer3D") var player_inventory_close_audio_path := NodePath("PlayerInventoryCloseAudio")
@onready var player_inventory_close_audio : AudioStreamPlayer3D = get_node(player_inventory_close_audio_path)


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
		get_node(interactor_path).raycast = value
@export var camera_3d : Camera3D:
	set(value):
		camera_3d = value
		get_node(interactor_path).camera_3d = value


func _ready():
	if Engine.is_editor_hint():
		return
	InventorySystem.setup_inventory_handler(inventory_handler)
	InventorySystem.setup_hotbar(hotbar)
	InventorySystem.setup_crafter(crafter)
	InventorySystem.setup_interactor(interactor)
	
	# Setup for audios 🔊
	inventory_handler.picked.connect(_on_inventory_handler_picked.bind())
	inventory_handler.dropped.connect(_on_inventory_handler_dropped.bind())
	inventory_handler.inventories[0].opened.connect(_on_player_inventory_opened.bind())
	inventory_handler.inventories[0].closed.connect(_on_player_inventory_closed.bind())
	hotbar.on_change_selection.connect(_on_hotbar_changed.bind())
	
	# Setup for enabled/disabled mouse 🖱️😀
	if change_mouse_state:
		inventory_handler.opened.connect(_update_opened_inventories.bind())
		inventory_handler.closed.connect(_update_opened_inventories.bind())
		crafter.opened.connect(_update_opened_stations.bind())
		crafter.closed.connect(_update_opened_stations.bind())
		_update_opened_inventories(inventory_handler.inventories[0])


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
	if InventorySystem.inventory_handler.is_open_main_inventory():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _update_opened_stations(_craft_station : CraftStation):
	if InventorySystem.crafter.is_open_any_station():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func inventory_inputs():
	if Input.is_action_just_released(toggle_inventory_input):
		if inventory_handler.is_open_main_inventory():
			inventory_handler.close_main_inventory()
			inventory_handler.close_all_inventories()
		else:
			inventory_handler.open_main_inventory()
	
	if Input.is_action_just_released(exit_inventory_and_craft_panel_input):
		if inventory_handler.is_open_main_inventory():
			inventory_handler.close_main_inventory()
			inventory_handler.close_all_inventories()
		if crafter.is_open_any_station():
			crafter.close_all_craft_stations()
			
	if Input.is_action_just_released(toggle_craft_panel_input):
		if crafter.is_open_main_craft_station():
			crafter.close_main_craft_station()
			crafter.close_all_craft_stations()
		else:
			crafter.open_main_craft_station()


func hot_bar_inputs(event : InputEvent):
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
	picked_audio.play()


func _on_inventory_handler_dropped(_dropped_item):
	drop_audio.play()
	

func _on_player_inventory_opened():
	player_inventory_open_audio.play()


func _on_player_inventory_closed():
	player_inventory_close_audio.play()


func _on_hotbar_changed(_selection : int):
	hotbar_change_audio.play()
