extends Node
class_name FPSDemo


var inventory_system_ui_path := NodePath("UI/Inventory System UI")
@onready var inventory_system_ui: Control = get_node(inventory_system_ui_path)

@export var item_wood : InventoryItem
@export var item_stone : InventoryItem
@export var item_grass : InventoryItem
@export var database : InventoryDatabase

var player_inventory_handler : InventoryHandler
var player_crafter : Crafter


func _ready():
	setup_inventory_handler(get_node(NodePath("Player/InventoryHandler")))
	setup_crafter(get_node(NodePath("Player/Crafter")))


func setup_inventory_handler(inventory_handler : InventoryHandler):
	player_inventory_handler = inventory_handler
	inventory_system_ui.set_player_inventory_handler(inventory_handler)
	inventory_system_ui.set_hotbar(inventory_handler.get_node("Hotbar"))
	inventory_handler.opened.connect(_update_opened_inventories.bind())
	inventory_handler.closed.connect(_update_opened_inventories.bind())
	_update_opened_inventories(inventory_handler.inventory)
	

func setup_crafter(crafter : Crafter):
	player_crafter = crafter
	inventory_system_ui.set_crafter(player_crafter)
	player_crafter.opened.connect(_update_opened_stations.bind())
	player_crafter.closed.connect(_update_opened_stations.bind())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	if Input.is_action_just_released("add_item_a"):
		player_inventory_handler.add_to_inventory(player_inventory_handler.inventory, item_wood, 1)
	if Input.is_action_just_released("remove_item_a"):
		player_inventory_handler.inventory.remove(item_wood, 1)
		
	if Input.is_action_just_released("add_item_b"):
		player_inventory_handler.add_to_inventory(player_inventory_handler.inventory, item_stone, 1)
	if Input.is_action_just_released("remove_item_b"):
		player_inventory_handler.inventory.remove(item_stone, 1)
		
	if Input.is_action_just_released("add_item_c"):
		player_inventory_handler.add_to_inventory(player_inventory_handler.inventory, item_grass, 1)
	if Input.is_action_just_released("remove_item_c"):
		player_inventory_handler.inventory.remove(item_grass, 1)


func _update_opened_inventories(_inventory : Inventory):
	if player_inventory_handler.is_open_main_inventory():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _update_opened_stations(_craft_station : CraftStation):
	if player_crafter.is_open_any_station():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
