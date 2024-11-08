class_name InventorySystemUI
extends Control

const Interactor = preload("../interaction_system/inventory_interactor.gd")

## This script manages inventory system UI information
## Contains drag slot information, UI inventories and item drop area

@export var console_mode : bool = false:
	set(value):
		player_inventory_ui.console_mode = console_mode
		loot_inventory_ui.console_mode = console_mode
		console_mode = value
		
@export var character : CharacterInventorySystem

## Stores [Crafter] information to connect all signals and callbacks
@export var interactor : Interactor

## Player [InventoryUI], Typically the main usage inventory
@onready var player_inventory_ui : GridInventoryUI = %PlayerGridInventoryUI

## Loot [InventoryUI], Typically an inventory that has been opened
@onready var loot_inventory_ui : GridInventoryUI = %LootGridInventoryUI

## Hotbar [HotbarUI]
@onready var hotbar_ui : HotbarUI = get_node(NodePath("HotbarUI"))

## Control that identifies area where a transaction slot can call the handler to drop items
@onready var drop_area: Control = get_node(NodePath("DropArea"))


@onready var player_craft_station_ui : CraftStationUI = get_node(NodePath("PlayerCraftStationUI"))

@onready var other_craft_station_ui : CraftStationUI = get_node(NodePath("OtherCraftStationUI"))

@onready var interactor_ui : InteractorUI = get_node(NodePath("InteractorUI"))


func _ready():
	# TODO connect
	# _setup_inventory_system_connection()
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	drop_area.visible = false
	hotbar_ui.visible = true
	player_craft_station_ui.close()
	other_craft_station_ui.close()
	drop_area.gui_input.connect(_drop_area_input)
	
	player_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	loot_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	player_inventory_ui.request_split.connect(_request_split)
	loot_inventory_ui.request_split.connect(_request_split)
	
	#player_craft_station_ui.input_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	other_craft_station_ui.input_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	other_craft_station_ui.input_inventory_ui.request_split.connect(_request_split)
	
	player_craft_station_ui.on_craft.connect(_on_craft)
	other_craft_station_ui.on_craft.connect(_on_craft)


## Setup inventory handler and connect all signals
func setup(character : CharacterInventorySystem):
	self.character = character
	
	var inventories : Array[Inventory]
	inventories.append(character.main_inventory)
	#inventories.append(character.equipment_inventory)
	set_player_inventory(character.main_inventory)
	character.opened_inventory.connect(_on_open_inventory)
	character.closed_inventory.connect(_on_close_inventory)
	
	# Stations
	character.opened_station.connect(_on_open_craft_station)
	character.closed_station.connect(_on_close_craft_station)
	# Interactor
	self.interactor = interactor
	interactor_ui.setup(character.interactor)
	# Hotbar
	#hotbar_ui.set_hotbar(character.hotbar)
	


## Setup player inventories
func set_player_inventory(player_inventory : GridInventory):
	print(player_inventory)
	player_inventory_ui.inventory = player_inventory


func _drop_area_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			pass
			#character.drop_holder()


func _open_player_inventory():
	player_inventory_ui.visible = true
		
	hotbar_ui.visible = false
	drop_area.visible = true
#	TODO console mode?
	#if console_mode and not player_inventories_ui.is_empty():
		#if player_inventories_ui[0].items.is_empty():
			#player_inventories_ui[0].items[0].grab_focus()


# Open Inventory of player	
func _on_open_inventory(inventory : Inventory):
	if character.main_inventory != inventory:
		loot_inventory_ui.inventory = inventory
		loot_inventory_ui.visible = true
	else:
		_open_player_inventory()
		

# Open Craft Station	
func _on_open_craft_station(craft_station : CraftStation):
	# TODO #42 Different skins for different types of craft stations
	if craft_station == character.main_station:
		player_craft_station_ui.open(craft_station)
	else:
		other_craft_station_ui.open(craft_station)
	hotbar_ui.visible = false
	_open_player_inventory()


func _on_close_craft_station(craft_station : CraftStation):
	if craft_station == character.main_station:
		player_craft_station_ui.close()
	else:
		other_craft_station_ui.close()
	hotbar_ui.visible = true
	_close_player_inventory()


func _on_close_inventory(inventory : Inventory):
	_close_player_inventory()


func _close_player_inventory():
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	#if loot_inventory_ui.inventory != null:
		#loot_inventory_ui.disconnect_old_inventory()
	drop_area.visible = false
	hotbar_ui.visible = true


func _on_craft(craft_station : CraftStation, recipe_index : int):
	character.craft(craft_station, recipe_index)


func _request_transfer_to(inventory: GridInventory, origin_pos: Vector2i, destination: GridInventory, destination_pos: Vector2i, amount: int):
	character.transfer_to(inventory, origin_pos, destination, destination_pos, amount)


func _request_split(inventory : Inventory, stack_index : int, amount : int):
	character.split(inventory, stack_index, amount)
