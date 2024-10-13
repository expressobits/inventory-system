class_name InventorySystemUI
extends Control

const Interactor = preload("../interaction_system/inventory_interactor.gd")

## This script manages inventory system UI information
## Contains drag slot information, UI inventories and item drop area

@export var console_mode : bool = false:
	set(value):
		transaction_slot_ui.console_mode = console_mode
		for inventory_ui in player_inventories_ui:
			inventory_ui.console_mode = console_mode
		loot_inventory_ui.console_mode = console_mode
		console_mode = value
		
@export var character : CharacterInventorySystem

## Stores [Crafter] information to connect all signals and callbacks
@export var interactor : Interactor

## SlotUI special that stores inventory transaction information
@onready var transaction_slot_ui : TransactionSlotUI = get_node(NodePath("TransactionSlotUI"))

## Player [InventoryUI], Typically the main usage inventory
@export var player_inventories_ui : Array[InventoryUI]

## Loot [InventoryUI], Typically an inventory that has been opened
@onready var loot_inventory_ui : InventoryUI = get_node(NodePath("LootInventoryUI"))

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
	for player_inventory_ui in player_inventories_ui:
		player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	transaction_slot_ui.clear_info()
	drop_area.visible = false
	hotbar_ui.visible = true
	player_craft_station_ui.close()
	other_craft_station_ui.close()
	for player_inventory_ui in player_inventories_ui:
		player_inventory_ui.slot_point_down.connect(_slot_point_down)
		player_inventory_ui.inventory_point_down.connect(_inventory_point_down)
	loot_inventory_ui.slot_point_down.connect(_slot_point_down)
	loot_inventory_ui.inventory_point_down.connect(_inventory_point_down)
	if other_craft_station_ui.input_inventory_ui != null:
		other_craft_station_ui.input_inventory_ui.slot_point_down.connect(_slot_point_down)
		other_craft_station_ui.input_inventory_ui.inventory_point_down.connect(_inventory_point_down)
	if other_craft_station_ui.output_inventory_ui != null:
		other_craft_station_ui.output_inventory_ui.slot_point_down.connect(_slot_point_down)
		other_craft_station_ui.output_inventory_ui.inventory_point_down.connect(_inventory_point_down)
	drop_area.gui_input.connect(_drop_area_input)
	
	player_craft_station_ui.on_craft.connect(_on_craft)
	other_craft_station_ui.on_craft.connect(_on_craft)


## Setup inventory handler and connect all signals
func setup(character : CharacterInventorySystem):
	self.character = character
	
	var inventories : Array[Inventory]
	inventories.append(character.main_inventory)
	inventories.append(character.equipment_inventory)
	set_player_inventories(inventories)
	character.opened_inventory.connect(_on_open_inventory)
	character.closed_inventory.connect(_on_close_inventory)
	character.slot_holder.updated.connect(_updated_transaction_slot.bind(character.main_inventory.database))
	
	# Stations
	character.opened_station.connect(_on_open_craft_station)
	character.closed_station.connect(_on_close_craft_station)
	# Interactor
	self.interactor = interactor
	interactor_ui.setup(character.interactor)
	# Hotbar
	hotbar_ui.set_hotbar(character.hotbar)
	


## Setup player inventories
func set_player_inventories(player_inventories : Array[Inventory]):
	for i in player_inventories.size():
		player_inventories_ui[i].set_inventory(player_inventories[i])


func _drop_area_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			character.drop_holder()


func _open_player_inventory():
	for player_inventory_ui in player_inventories_ui:
		player_inventory_ui.visible = true
		
	hotbar_ui.visible = false
	drop_area.visible = true
	if console_mode and not player_inventories_ui.is_empty():
		if player_inventories_ui[0].slots.is_empty():
			player_inventories_ui[0].slots[0].grab_focus()


# Open Inventory of player	
func _on_open_inventory(inventory : Inventory):
	if character.main_inventory != inventory:
		loot_inventory_ui.set_inventory(inventory)
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
	for player_inventory_ui in player_inventories_ui:
		player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	if loot_inventory_ui.inventory != null:
		loot_inventory_ui._disconnect_old_inventory()
	drop_area.visible = false
	hotbar_ui.visible = true


func _slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory):
	if not event is InputEventMouseButton:
		return
	var mouse_event : InputEventMouseButton = event as InputEventMouseButton
	if character.slot_holder.has_valid():
		var amount = _get_amount_per_mouse_event(mouse_event, character.slot_holder.amount)
		character.holder_to_at(slot_index, inventory, amount)
		$SlotDrop.play()
	else:
		if inventory.is_empty_slot(slot_index):
			return
		var slot = inventory.slots[slot_index]
		var amount = _get_amount_per_mouse_event(mouse_event, slot.amount)
		character.to_holder(slot_index, inventory, amount)	
		$SlotClick.play()


func _get_amount_per_mouse_event(mouse_event : InputEventMouseButton, amount : int) -> int:
	if mouse_event.button_index == MOUSE_BUTTON_LEFT:
		return amount
	if mouse_event.button_index == MOUSE_BUTTON_RIGHT:
		return ceili(amount / 2.0)
	if mouse_event.button_index == MOUSE_BUTTON_MIDDLE:
		return 1
	return 0


func _inventory_point_down(event : InputEvent, inventory : Inventory):
	if event.button_index == 3:
		return
	if character.slot_holder.has_valid():
		character.holder_to(inventory)
		$SlotDrop.play()


func _updated_transaction_slot(database : InventoryDatabase):
	transaction_slot_ui.update_info_with_slot(database, character.slot_holder)

func _on_craft(craft_station : CraftStation, recipe_index : int):
	character.craft(craft_station, recipe_index)
