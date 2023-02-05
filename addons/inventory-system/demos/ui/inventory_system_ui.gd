extends Control
class_name InventorySystemUI

## This script manages inventory system UI information
## Contains drag slot information, UI inventories and item drop area

## Stores [InventoryHandler] information to connect all signals and callbacks
var inventory_handler: InventoryHandler

## SlotUI special that stores inventory transaction information
@onready var transaction_slot_ui : TransactionSlotUI = get_node(NodePath("TransactionSlotUI"))

## Player [InventoryUI], Typically the main usage inventory
@onready var player_inventory_ui : InventoryUI = get_node(NodePath("PlayerInventoryUI") )

## Loot [InventoryUI], Typically an inventory that has been opened
@onready var loot_inventory_ui : InventoryUI = get_node(NodePath("LootInventoryUI"))

## Control that identifies area where a transaction slot can call the handler to drop items
@onready var drop_area: Control = get_node(NodePath("DropArea"))


func _ready():
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	transaction_slot_ui.clear_info()
	drop_area.visible = false
#	hotBarUI.gameObject.SetActive(false);
	player_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
	player_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
#	hotbarContainer.OnPointerDownSlotUI += PointerDownSlotUI;
	loot_inventory_ui.slot_point_down.connect(_slot_point_down.bind())
	loot_inventory_ui.inventory_point_down.connect(_inventory_point_down.bind())
	drop_area.gui_input.connect(_drop_area_input.bind())


## Setup inventory handler and connect all signals
func set_player_inventory_handler(handler : InventoryHandler):
	inventory_handler = handler
	set_player_inventory(handler.inventory)
	inventory_handler.opened.connect(_on_open_inventory)
	inventory_handler.closed.connect(_on_close_inventory)
	inventory_handler.updated_transaction_slot.connect(_updated_transaction_slot)

## Setup player [Inventory]
func set_player_inventory(player_inventory : Inventory):
	player_inventory_ui.set_inventory(player_inventory)


func _drop_area_input(event : InputEvent):
	if event is InputEventMouseButton:
		if event.pressed:
			inventory_handler.drop_transaction()


func _open_player_inventory():
	player_inventory_ui.visible = true;
#	hotbarContainer.gameObject.SetActive(true);
	drop_area.visible = true
#	ClearSelected();
#	hotBarUI.gameObject.SetActive(false);


# Open Inventory of player	
func _on_open_inventory(inventory : Inventory):
	if inventory != inventory_handler.inventory:
		loot_inventory_ui.set_inventory(inventory)
		loot_inventory_ui.visible = true
	else:
		_open_player_inventory()


func _on_close_inventory(inventory : Inventory):
	if inventory == inventory_handler.inventory:
		_close_player_inventory(inventory)


func _close_player_inventory(inventory : Inventory):
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
#    hotbarContainer.gameObject.SetActive(false);
	drop_area.visible = false
#    hotBarUI.gameObject.SetActive(true);


func _slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory):
	if inventory_handler.is_transaction_active():
		inventory_handler.transaction_to_at(slot_index, inventory)
	else:
		if inventory.is_empty_slot(slot_index):
			return
		var slot = inventory.slots[slot_index]
		var amount = slot.amount
		if event.button_index == 2:
			amount = ceili(slot.amount/2.0)
		inventory_handler.to_transaction(slot_index, inventory, amount)	
		

func _inventory_point_down(event : InputEvent, inventory : Inventory):
	if event.button_index == 3:
		return
	if inventory_handler.is_transaction_active():
		inventory_handler.transaction_to(inventory)


func _updated_transaction_slot(item : InventoryItem, amount : int):
	transaction_slot_ui.update_info_with_item(item, amount)
