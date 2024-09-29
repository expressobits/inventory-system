@tool
@icon("res://addons/inventory-system-demos/icons/character_inventory_system.svg")
class_name CharacterInventorySystem
extends NodeInventories

signal dropped(node : Node)
signal opened_station(station : CraftStation)
signal closed_station(station : CraftStation)
signal opened_inventory(inventory : Inventory)
signal closed_inventory(inventory : Inventory)
signal picked(obj : Node)

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export_group("🗃️ Inventory Nodes")
@export_node_path var main_inventory_path := NodePath("InventoryHandler/Inventory")
@onready var main_inventory : Inventory = get_node(main_inventory_path)
@export_node_path var equipment_inventory_path := NodePath("InventoryHandler/EquipmentInventory")
@onready var equipment_inventory : Inventory = get_node(equipment_inventory_path)
@export_node_path("Hotbar") var hotbar_path := NodePath("Hotbar")
@onready var hotbar : Hotbar = get_node(hotbar_path)
@export_node_path("CraftStation") var main_station_path := NodePath("CraftStation")
@onready var main_station : CraftStation = get_node(main_station_path)
@export_node_path var interactor_path := NodePath("Interactor")
@onready var interactor : Interactor = get_node(interactor_path)
@export_node_path var drop_parent_path := NodePath("../..");
@onready var drop_parent : Node = get_node(drop_parent_path)
@export_node_path var drop_parent_position_path := NodePath("..");
@onready var drop_parent_position : Node = get_node(drop_parent_position_path)

var opened_stations : Array[CraftStation]
var opened_inventories : Array[Inventory]
var slot_holder : Slot

@export_group("⌨️ Inputs")
## Change mouse state based on inventory status
@export var change_mouse_state : bool = true
@export var check_inputs : bool = true
@export var toggle_inventory_input : String = "toggle_inventory"
@export var exit_inventory_and_craft_panel_input : String = "escape"
@export var toggle_craft_panel_input : String = "toggle_craft_panel"


@export_group("🫴 Interact")
@export var can_interact : bool = true
@export var raycast : RayCast3D:
	set(value):
		raycast = value
		var interactor = get_node(interactor_path)
		if interactor != null and value != null:
			interactor.raycast = value
@export var camera_3d : Camera3D:
	set(value):
		camera_3d = value
		var interactor = get_node(interactor_path)
		if interactor != null and value != null:
			interactor.camera = value


func _ready():
	if Engine.is_editor_hint():
		return
	slot_holder = Slot.new()
	main_inventory.request_drop_obj.connect(_on_request_drop_obj)
	equipment_inventory.request_drop_obj.connect(_on_request_drop_obj)
	
	# Setup for enabled/disabled mouse 🖱️😀
	if change_mouse_state:
		opened_inventory.connect(_update_opened_inventories)
		closed_inventory.connect(_update_opened_inventories)
		opened_station.connect(_update_opened_stations)
		closed_station.connect(_update_opened_stations)
		_update_opened_inventories(main_inventory)


func _input(event : InputEvent) -> void:
	if Engine.is_editor_hint():
		return
	if check_inputs:
		hot_bar_inputs(event)
		inventory_inputs()


func _physics_process(_delta : float):
	if Engine.is_editor_hint():
		return
	if not can_interact:
		return
	interactor.try_interact()


func is_any_station_or_inventory_opened() -> bool:
	return is_open_any_station() or is_open_main_inventory()


func _update_opened_inventories(_inventory : Inventory):
	_check_inputs()


func _update_opened_stations(_craft_station : CraftStation):
	_check_inputs()


func _check_inputs():
	if is_any_station_or_inventory_opened():
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func inventory_inputs():
	if Input.is_action_just_released(toggle_inventory_input):
		if not is_any_station_or_inventory_opened():
			open_main_inventory()
	
	if Input.is_action_just_released(exit_inventory_and_craft_panel_input):
		close_inventories()
		close_craft_stations()
			
	if Input.is_action_just_released(toggle_craft_panel_input):
		if not is_any_station_or_inventory_opened():
			open_main_craft_station()


#region Slot Holder
func change_holder(item : Item, amount : int):
	slot_holder.amount = amount
	if amount > 0 and item != null:
		slot_holder.item = item
	else:
		slot_holder.item = Item.new()


func to_holder(slot_index : int, inventory : Inventory, amount : int):
	if slot_holder.has_valid():
		return
	var slot : Slot = inventory.slots[slot_index]
	var item : Item = slot.item.duplicate()
	if not slot.has_valid():
		return
	var amount_no_removed = inventory.remove_at(slot_index, item, amount)
	change_holder(item, amount - amount_no_removed)


func holder_to(inventory : Inventory):
	if not slot_holder.has_valid():
		return
	if slot_holder.item == null:
		return
	var amount_no_add : int = inventory.add(slot_holder.item, slot_holder.amount)
	change_holder(slot_holder.item, amount_no_add)


func holder_to_at(slot_index : int, inventory : Inventory, amount_to_move : int = -1):
	if not slot_holder.has_valid():
		return
	var slot : Slot = inventory.slots[slot_index];
	var item : Item = Item.new()
	item.definition = slot_holder.item.definition
	item.properties = slot_holder.item.properties.duplicate()
	if inventory.is_empty_slot(slot_index) or item.is_stack_with(slot.item):
		var amount = slot_holder.amount
		if amount_to_move >= 0:
			amount = amount_to_move
		var amount_no_add = inventory.add_at(slot_index, item, amount)
		change_holder(item, slot_holder.amount - amount + amount_no_add)
	else:
		# Different items in slot and other_slot
		# Check if slot_holder amount is equal of origin_slot amount
		if slot.categorized and not slot.is_accept_any_categories_of_item(item.definition):
			return
		var temp_item : Item = Item.new()
		temp_item.definition = slot.item.definition
		temp_item.properties = slot.item.properties.duplicate()
		var new_amount = slot.amount
		inventory.set_slot_content(slot_index, item.definition, item.properties, slot_holder.amount)
		change_holder(temp_item, new_amount)
		


func pick_to_inventory(node : Node):
	if main_inventory == null:
		return

	if !node.get("is_pickable"):
		return
		
	var item_id = node.item_id
	
	var item_definition = database.get_item(item_id)
	
	if item_definition == null:
		return
		
	var item = Item.new()
	item.definition = item_definition

	if main_inventory.add(item, 1, true) == 0:
		emit_signal("picked", node)
		node.queue_free();
		return
		
	printerr("pick_to_inventory return false");


func drop_holder():
	if not slot_holder.has_valid():
		return
	main_inventory.drop(slot_holder.item, slot_holder.amount)
	change_holder(null, 0)


func _on_request_drop_obj(dropped_item : String, item : Item):
	var packed_scene : PackedScene = load(dropped_item)
	var node = packed_scene.instantiate()
	drop_parent.add_child(node)
	node.set("item", item)
	node.set("position", drop_parent_position.get("position"))
	node.set("rotation", drop_parent_position.get("position"))
	dropped.emit(node)
#endregion

#region Crafter
func craft(craft_station : CraftStation, recipe_index : int):
	craft_station.craft(recipe_index)

#endregion

#region Hotbar
func hot_bar_inputs(event : InputEvent):
	if event is InputEventMouseButton:
		if event.is_pressed():
			if event.button_index == MOUSE_BUTTON_WHEEL_UP:
				hotbar_previous_item()
			if event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
				hotbar_next_item()
	if event is InputEventKey:
		var input_key_event = event as InputEventKey
		if event.is_pressed() and not event.is_echo():
			if input_key_event.keycode > KEY_0 and input_key_event.keycode < KEY_9:
				hotbar_change_selection(input_key_event.keycode - KEY_1)


func hotbar_change_selection(index : int):
	hotbar.selection_index = index


func hotbar_previous_item():
	hotbar.previous_item()
	

func hotbar_next_item():
	hotbar.next_item()

#endregion

#region Open Inventories
func is_open_inventory(inventory : Inventory):
	return opened_inventories.find(inventory) != -1

func open_inventory(inventory : Inventory):
	if is_open_inventory(inventory):
		return
	add_open_inventory(inventory)


func add_open_inventory(inventory : Inventory):
	opened_inventories.append(inventory)
	opened_inventory.emit(inventory)
	if not is_open_main_inventory():
		open_main_inventory()
	
func open_main_inventory():
	open_inventory(main_inventory)
	
	
func close_inventory(inventory : Inventory):
	if main_inventory != inventory:
		inventory.get_parent().close(get_parent())
	remove_open_inventory(inventory)
	if slot_holder.has_valid():
		drop_holder()


func remove_open_inventory(inventory : Inventory):
	var index = opened_inventories.find(inventory)
	opened_inventories.remove_at(index)
	closed_inventory.emit(inventory)


func close_inventories():
	for index in range(opened_inventories.size() - 1, -1, -1):
		close_inventory(opened_inventories[index])


func is_open_any_inventory():
	return !opened_inventories.is_empty()
	
func is_open_main_inventory():
	return is_open_inventory(main_inventory)
#endregion

#region Open Craft Stations
func is_open_station(station : CraftStation):
	return opened_stations.find(station) != -1


func open_station(station : CraftStation):
	if is_open_station(station):
		return
	add_open_station(station)


func add_open_station(station : CraftStation):
	opened_stations.append(station)
	opened_station.emit(station)


func close_station(station : CraftStation):
	if not is_open_station(station):
		return
	remove_open_station(station)


func remove_open_station(station : CraftStation):
	var index = opened_stations.find(station)
	opened_stations.remove_at(index)
	closed_station.emit(station)
	if main_station != station:
		station.get_parent().close(get_parent())


func open_main_craft_station():
	open_station(main_station)


func close_craft_stations():
	for index in range(opened_stations.size() - 1, -1, -1):
		close_station(opened_stations[index])

func is_open_any_station():
	return !opened_stations.is_empty()
	
	
#endregion
