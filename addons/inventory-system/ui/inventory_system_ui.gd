extends Control
class_name InventorySystemUI

var ui_inventories : Array
var selected_slot : SlotUI
var drag_slot_path := NodePath("DragSlotUI")

@export_node_path("InventoryUI") var player_inventory_ui_path := NodePath("Player Inventory UI") 
@export_node_path("InventoryUI") var loot_inventory_ui_path := NodePath("Loot Inventory UI") 
@export_node_path("Control") var drop_area_path := NodePath("DropArea")
@onready var drag_slot : DragSlotUI = get_node(drag_slot_path)
@export var inventory_ui : PackedScene

@onready var player_inventory_ui : InventoryUI = get_node(player_inventory_ui_path)
@onready var loot_inventory_ui : InventoryUI = get_node(loot_inventory_ui_path)
@onready var player_inventory_handler: InventoryHandler = get_node("../../Player/InventoryHandler")
@onready var drop_area: Control = get_node(drop_area_path)


func _ready():
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
	drag_slot.clear_info()
	drop_area.visible = false
#	hotBarUI.gameObject.SetActive(false);
	player_inventory_ui.slot_point_down.connect(slot_point_down.bind())
	player_inventory_ui.inventory_point_down.connect(inventory_point_down.bind())
	drag_slot.inventory_handler = player_inventory_handler
#	hotbarContainer.OnPointerDownSlotUI += PointerDownSlotUI;
	loot_inventory_ui.slot_point_down.connect(slot_point_down.bind())
	loot_inventory_ui.inventory_point_down.connect(inventory_point_down.bind())
	drop_area.gui_input.connect(drop_area_input.bind())

func drop_area_input(event : InputEvent):
	if event is InputEventMouseMotion:
		drag_slot.enter_in_drop_area()
	if event is InputEventMouseButton:
		if event.pressed:
			player_inventory_handler.drop_transaction()

func add_inventory_ui(inventory : Inventory):
	var inv_ui = inventory_ui.instantiate()
	add_child(inv_ui)
	inv_ui.set_inventory(inventory)
	ui_inventories.append(ui_inventories)


func set_player_inventory_handler(handler : InventoryHandler):
	player_inventory_ui.set_inventory(handler.inventory)
	player_inventory_handler.opened.connect(_on_open_inventory)
	player_inventory_handler.closed.connect(_on_close_inventory)
	player_inventory_handler.updated_transaction_slot.connect(_updated_transaction_slot)

func _open_player_inventory():
	player_inventory_ui.visible = true;
#	hotbarContainer.gameObject.SetActive(true);
	drop_area.visible = true
#	ClearSelected();
#	hotBarUI.gameObject.SetActive(false);


# Open Inventory of player	
func _on_open_inventory(inventory : Inventory):
	if inventory != player_inventory_handler.inventory:
		loot_inventory_ui.set_inventory(inventory)
		loot_inventory_ui.visible = true
	else:
		_open_player_inventory()


func _on_close_inventory(inventory : Inventory):
	if inventory == player_inventory_handler.inventory:
		close_player_inventory(inventory)


func close_player_inventory(inventory : Inventory):
	player_inventory_ui.visible = false
	loot_inventory_ui.visible = false
#    hotbarContainer.gameObject.SetActive(false);
	drop_area.visible = false
#    hotBarUI.gameObject.SetActive(true);


func slot_point_down(event : InputEvent, slot_index : int, inventory : Inventory):
	if event.button_index == 3:
		return
	if player_inventory_handler.is_transaction_active():
		player_inventory_handler.transaction_to_at(slot_index, inventory)
	else:
		if inventory.is_empty_slot(slot_index):
			return
		var slot = inventory.slots[slot_index]
		var amount = slot.amount
		if event.button_index == 2:
			amount = ceili(slot.amount/2.0)
		drag_slot.offset = -event.position
		player_inventory_handler.to_transaction(slot_index, inventory, amount)	
		

func inventory_point_down(event : InputEvent, inventory : Inventory):
	if event.button_index == 3:
		return
	if player_inventory_handler.is_transaction_active():
		player_inventory_handler.transaction_to(inventory)


func drop_slot(slot_index : int, inventory : Inventory, amount := 1):
	swap_inventories(slot_index, inventory, amount)


func _updated_transaction_slot(item : InventoryItem, amount : int):
	drag_slot.update_data(item, amount)


func swap_inventories(slot_index : int, inventory : Inventory, amount := 1):
	if drag_slot.has_slot():
		player_inventory_handler.swap_between_containers(drag_slot.inventory, drag_slot.slot_index, inventory, slot_index, amount)
	drag_slot.clear_info()
