extends Control
class_name InventorySystemUI

@export var inventory_ui : PackedScene
var ui_inventories : Array
var selected_slot : SlotUI
var drag_slot_path := NodePath("DragSlotUI")
@onready var drag_slot : DragSlotUI = get_node(drag_slot_path)

func add_inventory_ui(inventory : Inventory):
	var inv_ui = inventory_ui.instantiate()
	add_child(inv_ui)
	inv_ui.set_inventory(inventory)
	ui_inventories.append(ui_inventories)
	
	
# Specific Inventories
@export_node_path(InventoryUI) var player_inventory_ui_path := NodePath("Player Inventory UI") 
@onready var player_inventory_ui : InventoryUI = get_node(player_inventory_ui_path)
@export_node_path(InventoryUI) var loot_inventory_ui_path := NodePath("Loot Inventory UI") 
@onready var loot_inventory_ui : InventoryUI = get_node(loot_inventory_ui_path)
@onready var player_inventory_handler: InventoryHandler = get_node("../../InventoryHandler")
@onready var loot_inventory: Inventory = get_node("../../LootInventory")


func _ready():
	player_inventory_ui.visible = false
	drag_slot.clear_info()
#	dropArea.gameObject.SetActive(false);
#	hotBarUI.gameObject.SetActive(false);
	player_inventory_ui.slot_point_down.connect(slot_point_down.bind());
	drag_slot.inventory_handler = player_inventory_handler
#	hotbarContainer.OnPointerDownSlotUI += PointerDownSlotUI;
#	lootContainer.OnPointerDownSlotUI += PointerDownSlotUI;

	
func set_player_inventory_handler(handler : InventoryHandler):
	player_inventory_ui.set_inventory(handler.inventory)
	player_inventory_handler.opened.connect(open_player_inventory)
	player_inventory_handler.closed.connect(close_player_inventory)
	player_inventory_handler.updated_transaction_slot.connect(_updated_transaction_slot)


# Open Inventory of player	
func open_player_inventory(inventory : Inventory):
	if inventory == player_inventory_handler.inventory:
		player_inventory_ui.visible = true


func close_player_inventory(inventory : Inventory):
	if inventory == player_inventory_handler.inventory:
		player_inventory_ui.visible = false


func slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory):
	if event.button_index == 3:
		return
	if player_inventory_handler.is_transaction_active():
		player_inventory_handler.transaction_to(slot_index, inventory)
	else:
		if inventory.is_empty_slot(slot_index):
			return
		var slot = inventory.slots[slot_index]
		var amount = slot.amount
		if event.button_index == 2:
			amount = ceili(slot.amount/2.0)
		drag_slot.offset = -event.position
		player_inventory_handler.to_transaction(slot_index, inventory, amount)	


func drop_slot(slot_index : int, inventory : Inventory, amount := 1):
	swap_inventories(slot_index, inventory, amount)


func _updated_transaction_slot(item : Item, amount : int):
	drag_slot.update_data(item, amount)


func swap_inventories(slot_index : int, inventory : Inventory, amount := 1):
	if drag_slot.has_slot():
		player_inventory_handler.swap_between_containers(drag_slot.inventory, drag_slot.slot_index, inventory, slot_index, amount)
	drag_slot.clear_info()
