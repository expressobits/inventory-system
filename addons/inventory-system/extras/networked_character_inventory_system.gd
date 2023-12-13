class_name NetworkedCharacterInventorySystem
extends CharacterInventorySystem

func _ready():
	inventory_handler.picked.connect(_on_inventory_handler_picked.bind())
	inventory_handler.dropped.connect(_on_inventory_handler_dropped.bind())
	inventory_handler.inventories[0].opened.connect(_on_player_inventory_opened.bind())
	inventory_handler.inventories[0].closed.connect(_on_player_inventory_closed.bind())
	if is_multiplayer_authority():
		InventorySystem.setup_inventory_handler(inventory_handler)
		InventorySystem.setup_hotbar(hotbar)
		InventorySystem.setup_crafter(crafter)
		InventorySystem.setup_interactor(interactor)
		
		# Setup for enabled/disabled mouse 🖱️😀
		inventory_handler.opened.connect(_update_opened_inventories.bind())
		inventory_handler.closed.connect(_update_opened_inventories.bind())
		crafter.opened.connect(_update_opened_stations.bind())
		crafter.closed.connect(_update_opened_stations.bind())
		_update_opened_inventories(inventory_handler.inventories[0])


func _input(event : InputEvent):
	if Engine.is_editor_hint():
		return
	if is_multiplayer_authority():
		hot_bar_inputs(event)
		inventory_inputs()


func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	if is_multiplayer_authority():
		interactor.try_interact()
