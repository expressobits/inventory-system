extends Node
class_name FPSDemo

@export var item_wood : InventoryItem
@export var item_stone : InventoryItem
@export var item_grass : InventoryItem
@export var database : InventoryDatabase


func _ready():
	InventorySystem.inventory_handler_changed.connect(_on_inventory_handler_changed.bind())
	InventorySystem.crafter_changed.connect(_on_crafter_changed.bind())
	setup_inventory_handler(get_node(NodePath("Player/InventoryHandler")))
	setup_crafter(get_node(NodePath("Player/Crafter")))


func setup_inventory_handler(inventory_handler : InventoryHandler):
	InventorySystem.setup_inventory_handler(inventory_handler)
	

func setup_crafter(crafter : Crafter):
	InventorySystem.setup_crafter(crafter)


func _process(_delta):
	if Input.is_action_just_released("add_item_a"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_wood, 1)
	if Input.is_action_just_released("remove_item_a"):
		InventorySystem.inventory_handler.inventory.remove(item_wood, 1)
		
	if Input.is_action_just_released("add_item_b"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_stone, 1)
	if Input.is_action_just_released("remove_item_b"):
		InventorySystem.inventory_handler.inventory.remove(item_stone, 1)
		
	if Input.is_action_just_released("add_item_c"):
		InventorySystem.inventory_handler.add_to_inventory(InventorySystem.inventory_handler.inventory, item_grass, 1)
	if Input.is_action_just_released("remove_item_c"):
		InventorySystem.inventory_handler.inventory.remove(item_grass, 1)


## Functions for disabled/enabled mouse input
func _on_inventory_handler_changed():
	InventorySystem.inventory_handler.opened.connect(_update_opened_inventories.bind())
	InventorySystem.inventory_handler.closed.connect(_update_opened_inventories.bind())
	_update_opened_inventories(InventorySystem.inventory_handler.inventory)


func _on_crafter_changed():
	InventorySystem.crafter.opened.connect(_update_opened_stations.bind())
	InventorySystem.crafter.closed.connect(_update_opened_stations.bind())
	_update_opened_inventories(InventorySystem.inventory_handler.inventory)


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
