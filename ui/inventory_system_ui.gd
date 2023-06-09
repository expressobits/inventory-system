extends Control
class_name InventorySystemUI

## This script manages inventory system UI information
## Contains drag slot information, UI inventories and item drop area

## Stores [InventoryHandler] information to connect all signals and callbacks
var inventory_handler : InventoryHandler

## Stores [Crafter] information to connect all signals and callbacks
var crafter : Crafter

## SlotUI special that stores inventory transaction information
@onready var transaction_slot_ui : TransactionSlotUI = get_node(NodePath("TransactionSlotUI"))

## Player [InventoryUI], Typically the main usage inventory
@onready var player_inventory_ui : InventoryUI = get_node(NodePath("PlayerInventoryUI") )

## Loot [InventoryUI], Typically an inventory that has been opened
@onready var loot_inventory_ui : InventoryUI = get_node(NodePath("LootInventoryUI"))

## Hotbar [HotbarUI]
@onready var hotbar_ui : HotbarUI = get_node(NodePath("HotbarUI"))

## Control that identifies area where a transaction slot can call the handler to drop items
@onready var drop_area: Control = get_node(NodePath("DropArea"))


@onready var player_craft_station_ui : CraftStationUI = get_node(NodePath("PlayerCraftStationUI"))

@onready var other_craft_station_ui : CraftStationUI = get_node(NodePath("OtherCraftStationUI"))

func _ready():
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	transaction_slot_ui.clear_info()
	drop_area.visible = false
	hotbar_ui.visible = true
	player_craft_station_ui.close()
	other_craft_station_ui.close()
	player_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
	player_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
	loot_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
	loot_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
	if other_craft_station_ui.input_inventory_ui != null:
		other_craft_station_ui.input_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
		other_craft_station_ui.input_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
	if other_craft_station_ui.output_inventory_ui != null:
		other_craft_station_ui.output_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
		other_craft_station_ui.output_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
	drop_area.gui_input.connect(_drop_area_input.bind())


## Setup inventory handler and connect all signals
func set_player_inventory_handler(handler : InventoryHandler):
	inventory_handler = handler
	set_player_inventory(handler.inventory)
	inventory_handler.opened.connect(_on_open_inventory)
	inventory_handler.closed.connect(_on_close_inventory)
	inventory_handler.updated_transaction_slot.connect(_updated_transaction_slot)
	

func set_crafter(crafter : Crafter):
	self.crafter = crafter
	crafter.opened.connect(_on_open_craft_station.bind())
	crafter.closed.connect(_on_close_craft_station.bind())


func set_hotbar(hotbar : Hotbar):
	hotbar_ui.set_hotbar(hotbar)


## Setup player [Inventory]
func set_player_inventory(player_inventory : Inventory):
	player_inventory_ui.set_inventory(player_inventory)


func _drop_area_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			inventory_handler.drop_transaction()


func _open_player_inventory():
	player_inventory_ui.visible = true
	hotbar_ui.visible = false
	drop_area.visible = true
	if not player_inventory_ui.slots.is_empty():
		player_inventory_ui.slots[0].grab_focus()


# Open Inventory of player	
func _on_open_inventory(inventory : Inventory):
	if inventory != inventory_handler.inventory:
		loot_inventory_ui.set_inventory(inventory)
		loot_inventory_ui.visible = true
	else:
		_open_player_inventory()
		

# Open Craft Station	
func _on_open_craft_station(craft_station : CraftStation):
	# TODO #42 Different skins for different types of craft stations
	if craft_station == crafter.main_station:
		player_craft_station_ui.open(craft_station)
	else:
		other_craft_station_ui.open(craft_station)
	hotbar_ui.visible = false
	_open_player_inventory()


func _on_close_craft_station(craft_station : CraftStation):
	if craft_station == crafter.main_station:
		player_craft_station_ui.close()
	else:
		other_craft_station_ui.close()
	hotbar_ui.visible = true
	_close_player_inventory()


func _on_close_inventory(inventory : Inventory):
	if inventory == inventory_handler.inventory:
		_close_player_inventory()


func _close_player_inventory():
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	if loot_inventory_ui.inventory != null:
		loot_inventory_ui._disconnect_old_inventory()
#    hotbarContainer.gameObject.SetActive(false);
	drop_area.visible = false
	hotbar_ui.visible = true


func _slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory):
	if inventory_handler.is_transaction_active():
		inventory_handler.transaction_to_at(slot_index, inventory)
		$SlotDrop.play()
	else:
		if inventory.is_empty_slot(slot_index):
			return
		var slot = inventory.slots[slot_index]
		var amount = slot.amount
		if event is InputEventMouseButton and event.button_index == 2:
			amount = ceili(slot.amount/2.0)
		inventory_handler.to_transaction(slot_index, inventory, amount)	
		$SlotClick.play()
		

func _inventory_point_down(event : InputEvent, inventory : Inventory):
	if event.button_index == 3:
		return
	if inventory_handler.is_transaction_active():
		inventory_handler.transaction_to(inventory)
		$SlotDrop.play()


func _updated_transaction_slot(item : InventoryItem, amount : int):
	transaction_slot_ui.update_info_with_item(item, amount)
