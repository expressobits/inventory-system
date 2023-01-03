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
@onready var player_inventory: Inventory = get_node("../../Player Inventory")
	
func set_player_inventory(inventory : Inventory):
	player_inventory_ui.set_inventory(inventory)
	
func _ready():
#	dragSlot.gameObject.SetActive(false);
#	dropArea.gameObject.SetActive(false);
#	hotBarUI.gameObject.SetActive(false);
	player_inventory_ui.slot_point_down.connect(slot_point_down.bind());
	player_inventory_ui.slot_point_up.connect(slot_point_up.bind());
#	hotbarContainer.OnPointerDownSlotUI += PointerDownSlotUI;
#	lootContainer.OnPointerDownSlotUI += PointerDownSlotUI;

func slot_point_down(slot_index : int, inventory : Inventory):
	if drag_slot.visible:
		var slot = inventory.slots[slot_index]
		drag_slot.clear_info()
	else:
		select(slot_index, inventory)

func slot_point_up(slot_index : int, inv : Inventory):
	print(slot_index)
	if drag_slot.visible:
		drag_slot.clear_info()
	
func select(slot_index : int, inventory : Inventory):
	drag_slot.update_drag_info(slot_index, inventory)
