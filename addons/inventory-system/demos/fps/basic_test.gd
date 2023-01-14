extends Node3D

var player_inventory_handler_path := NodePath("InventoryHandler")
@onready var player_inventory_handler: InventoryHandler = get_node(player_inventory_handler_path)

var inventory_system_ui_path := NodePath("UI/Inventory System UI")
@onready var inventory_system_ui: Control = get_node(inventory_system_ui_path)

@export var item_wood : Item
@export var item_metal : Item

func _ready():
	inventory_system_ui.set_player_inventory_handler(player_inventory_handler)
	player_inventory_handler.inventory.emptied.connect(_on_empty.bind())
	player_inventory_handler.inventory.filled.connect(_on_filled.bind())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	if Input.is_action_just_released("Add item"):
		player_inventory_handler.inventory.add_at(1,item_wood)
	if Input.is_action_just_released("Remove item"):
		player_inventory_handler.inventory.remove(item_wood, 2)
		
	if Input.is_action_just_released("Add item B"):
		player_inventory_handler.inventory.add(item_metal)
	if Input.is_action_just_released("Remove item B"):
		player_inventory_handler.inventory.remove(item_metal, 2)
	
	if Input.is_action_just_released("Toggle Inventory"):
		if player_inventory_handler.is_open_personal_inventory():
			player_inventory_handler.close_all_containers()
		else:
			player_inventory_handler.open_personal_inventory()
			
	if Input.is_action_just_released("Open Loot"):
		if not player_inventory_handler.is_open($LootInventory):
			player_inventory_handler.open($LootInventory)
		if not player_inventory_handler.is_open_personal_inventory():
			player_inventory_handler.open_personal_inventory()

	if Input.is_action_just_released("Has"):
		print(player_inventory_handler.inventory.contains(item_metal))
		$"CanvasLayer/Inventory System UI/Inventory UI"._update_slots()
	if Input.is_action_just_released("GetAmountOf"):
		print(player_inventory_handler.inventory.get_amount_of(item_metal))
		
func _on_empty():
	print("EMPTY")
	
			
func _on_filled():
	print("FILLED")

