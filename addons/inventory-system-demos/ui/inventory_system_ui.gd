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
@onready var player_inventory_ui : GridInventoryPanel = %PlayerGridInventoryUI

## Loot [InventoryUI], Typically an inventory that has been opened
@onready var loot_inventory_ui : GridInventoryPanel = %LootGridInventoryUI

## Hotbar [HotbarUI]
@onready var hotbar_ui : HotbarUI = get_node(NodePath("HotbarUI"))

## Control that identifies area where a transaction slot can call the handler to drop items
@onready var drop_area: DropArea = get_node(NodePath("DropArea"))


@onready var player_craft_station_ui : CraftStationUI = %PlayerCraftStationUI

@onready var other_craft_station_ui : CraftStationUI = %OtherCraftStationUI

@onready var interactor_ui : InteractorUI = get_node(NodePath("InteractorUI"))

@onready var stack_popup_menu: PopupMenu = $StackPopupMenu


var alternative_inventory : Inventory

func _ready():
	stack_popup_menu.id_pressed.connect(_on_stack_popup_menu_id_pressed)
	# TODO connect
	# _setup_inventory_system_connection()
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	drop_area.visible = false
	hotbar_ui.visible = true
	player_craft_station_ui.close()
	other_craft_station_ui.close()
	drop_area.request_drop.connect(_request_drop)
	
	player_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	loot_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	
	player_inventory_ui.request_fast_transfer.connect(_request_fast_transfer)
	loot_inventory_ui.request_fast_transfer.connect(_request_fast_transfer)
	
	player_inventory_ui.request_split.connect(_request_split)
	loot_inventory_ui.request_split.connect(_request_split)
	#player_inventory_ui.request_sort.connect(_request_sort)
	#loot_inventory_ui.request_sort.connect(_request_sort)
	
	player_inventory_ui.inventory_stack_context_activated.connect(_inventory_stack_context)
	loot_inventory_ui.inventory_stack_context_activated.connect(_inventory_stack_context)
	
	other_craft_station_ui.input_inventory_ui.inventory_stack_context_activated.connect(_inventory_stack_context)
	
	#player_craft_station_ui.input_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	other_craft_station_ui.input_inventory_ui.request_transfer_to.connect(_request_transfer_to)
	other_craft_station_ui.input_inventory_ui.request_fast_transfer.connect(_request_fast_transfer)
	other_craft_station_ui.input_inventory_ui.request_split.connect(_request_split)
	
	player_craft_station_ui.on_craft.connect(_on_craft)
	other_craft_station_ui.on_craft.connect(_on_craft)


## Setup inventory handler and connect all signals
func setup(_character : CharacterInventorySystem):
	self.character = _character
	
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
	hotbar_ui.set_hotbar(character.hotbar)
	


## Setup player inventories
func set_player_inventory(player_inventory : GridInventory):
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
		alternative_inventory = loot_inventory_ui.inventory
	else:
		_open_player_inventory()
		

# Open Craft Station	
func _on_open_craft_station(craft_station : CraftStation):
	# TODO #42 Different skins for different types of craft stations
	if craft_station == character.main_station:
		player_craft_station_ui.open(craft_station)
	else:
		other_craft_station_ui.open(craft_station)
		alternative_inventory = craft_station.get_input_inventory()
	hotbar_ui.visible = false
	_open_player_inventory()


func _on_close_craft_station(craft_station : CraftStation):
	if craft_station == character.main_station:
		player_craft_station_ui.close()
	else:
		other_craft_station_ui.close()
		alternative_inventory = null
	hotbar_ui.visible = true
	_close_player_inventory()


func _on_close_inventory(inventory : Inventory):
	if character.main_inventory != inventory:
		alternative_inventory = null
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


func _request_transfer_to(inventory: GridInventory, origin_pos: Vector2i, destination: GridInventory, destination_pos: Vector2i, amount: int, is_rotated: bool):
	character.transfer_to(inventory, origin_pos, destination, destination_pos, amount, is_rotated)


func _request_fast_transfer(inventory: GridInventory, origin_pos: Vector2i, amount: int):
	var destination: Inventory
	if inventory == player_inventory_ui.inventory:
		if alternative_inventory == null:
			return
		destination = alternative_inventory
	else:
		destination = player_inventory_ui.inventory
	character.transfer(inventory, origin_pos, destination, amount)


func _request_split(inventory : Inventory, stack_index : int, amount : int):
	character.split(inventory, stack_index, amount)


func _request_drop(stack: ItemStack, inventory: Inventory):
	character.drop(stack, inventory)


func _request_rotate(stack: ItemStack, inventory: Inventory):
	character.rotate(stack, inventory)


func _request_equip(stack: ItemStack, inventory: Inventory, slot_index: int):
	character.equip(stack, inventory, slot_index)


func _request_sort(inventory : Inventory):
	character.sort(inventory)


var current_stack: ItemStack
var current_inventory: GridInventory
const STACK_MENU_ID_SPLIT = 0
const STACK_MENU_ID_DROP = 1
const STACK_MENU_ID_EQUIP = 2
const STACK_MENU_ID_ROTATE = 3
const STACK_MENU_ID_MOVE_TO = 4
const STACK_MENU_ID_SORT = 9

func _inventory_stack_context(event: InputEvent, inventory: GridInventory, stack: ItemStack):
	current_stack = stack
	current_inventory = inventory
	if !(event is InputEventMouseButton):
		return

	var mb_event: InputEventMouseButton = event
	
	#var def = inventory.database.get_item(stack.item_id)
	#var stack_name = str(stack.amount) + " x "
	#if def == null:
		#stack_name += stack.item_id
	#else:
		#stack_name += def.name
	
	stack_popup_menu.clear()
	stack_popup_menu.add_item("Split", STACK_MENU_ID_SPLIT)
	stack_popup_menu.add_item("Rotate", STACK_MENU_ID_ROTATE)
	stack_popup_menu.add_item("Drop", STACK_MENU_ID_DROP)
	
	
	
	var equip_menu : PopupMenu = PopupMenu.new()
	for i in 4:
		if hotbar_ui.hotbar.is_active_slot(i):
			equip_menu.add_item(str("Equip on ",(i+1)), i)
	equip_menu.id_pressed.connect(_on_equip_menu_id_pressed)
	
	
	stack_popup_menu.add_submenu_node_item("Equip", equip_menu)
	#stack_popup_menu.add_item("Move to ", STACK_MENU_ID_MOVE_TO)
	stack_popup_menu.add_separator()
	stack_popup_menu.add_item("Sort Inventory", STACK_MENU_ID_SORT)
	
	stack_popup_menu.set_item_disabled(0, stack.amount == 1)
	
	stack_popup_menu.position = mb_event.global_position
	stack_popup_menu.popup()

func _on_stack_popup_menu_id_pressed(id: int):
	if current_stack == null:
		return
	if current_inventory == null:
		return
	match id:
		STACK_MENU_ID_SPLIT:
			var stack_index = current_inventory.stacks.find(current_stack)
			if stack_index == -1:
				return
			_request_split(current_inventory, stack_index, (int)(current_stack.amount/2.0))
		STACK_MENU_ID_DROP:
			_request_drop(current_stack, current_inventory)
		STACK_MENU_ID_ROTATE:
			_request_rotate(current_stack, current_inventory)
		STACK_MENU_ID_SORT:
			_request_sort(current_inventory)
		STACK_MENU_ID_MOVE_TO:
			_request_sort(current_inventory)


func _on_equip_menu_id_pressed(id: int):
	if current_stack == null:
		return
	if current_inventory == null:
		return
	_request_equip(current_stack, current_inventory, id)
