extends Node3D

var player_inventory_handler_path := NodePath("Player/InventoryHandler")
@onready var player_inventory_handler: InventoryHandler = get_node(player_inventory_handler_path)

var inventory_system_ui_path := NodePath("UI/Inventory System UI")
@onready var inventory_system_ui: Control = get_node(inventory_system_ui_path)

@export var item_wood : InventoryItem
@export var item_metal : InventoryItem
@export var database : InventoryDatabase

func _ready():
	inventory_system_ui.set_player_inventory_handler(player_inventory_handler)
	player_inventory_handler.inventory.emptied.connect(_on_empty.bind())
	player_inventory_handler.inventory.filled.connect(_on_filled.bind())
	player_inventory_handler.opened.connect(_update_opened_inventories.bind())
	player_inventory_handler.closed.connect(_update_opened_inventories.bind())
	_update_opened_inventories(player_inventory_handler.inventory)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_released("add_item_a"):
		player_inventory_handler.add_to_inventory(player_inventory_handler.inventory, item_wood, 1)
	if Input.is_action_just_released("remove_item_a"):
		player_inventory_handler.inventory.remove(item_wood, 2)
		
	if Input.is_action_just_released("add_item_b"):
		player_inventory_handler.add_to_inventory(player_inventory_handler.inventory, item_metal, 1)
	if Input.is_action_just_released("remove_item_b"):
		player_inventory_handler.inventory.remove(item_metal, 2)


func _on_empty():
	print("EMPTY")

func _on_filled():
	print("FILLED")

func _update_opened_inventories(inventory : Inventory):
	if player_inventory_handler.is_open_main_inventory():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
