@tool
@icon("res://addons/inventory-system/icons/inventory.svg")
extends NodeInventorySystemBase
class_name Inventory

## This script stores an array of slots via [Dictionary].
##
## This slot contains information about an [Item] and its quantity.

## Emitted when a amount or item has changed in any slot
signal inventory_changed

## Emitted when a slot was added.
## This signal is emitted after calling  [code]add[/code]  function and happens only when there is no space available and  [code]create_slot_if_needed[/code]  is set to true.
signal slot_added(slot_index : int)

## Emitted when a slot was removed.
## This signal is emitted after calling the  [code]remove[/code]  function and happens only when the slot is empty and the  [code]remove_slot_if_empty[/code]  is set to true.
signal slot_removed(slot_index : int)

## Emitted when a item was added.
signal item_added(item : SlotItem, amount : int)

## Emitted when a item was removed.
signal item_removed(item : SlotItem, amount : int)

## Emitted when inventory is filled.
## This signal is emitted after the  [code]add[/code]  ,  [code]add_at[/code]  or  [code]set_slot[/code]  function and it only happens when all slots are filled.
signal filled

## Emitted when inventory is empty.
## This signal is emitted after the  [code]remove[/code]  ,  [code]remove_at[/code]  or  [code]set_slot[/code]  function and it only happens when all slots are empty or there are no slots.
signal emptied

## Emitted when a slot has been changed.
## This signal is issued after functions that change the inventory content:  [code]add[/code] ,  [code]add_at[/code] ,  [code]remove[/code] ,  [code]remove-at[/code]  and  [code]set_slot[/code] .
signal updated_slot(slot_index : int)

## Emitted when inventory is opened.
## Called inside the [b]open()[/b] function when the inventory is closed.
signal opened

## Emitted when inventory is closed.
## Called inside the [b]close()[/b] function when the inventory is closed.
signal closed


## Array of [Slot] that stores items and their quantities
## The slot uses [b]"item"[/b] to store item id information
## and [b]"amount"[/b] for your quantity
@export var slots : Array[Slot] = []

## It stores information if this inventory is open or not.
@export var is_open := false

## Creates a slot when use  [code]add[/code]  for adding item to inventory when it is full
@export var create_slot_if_needed := false

## Removes a slot when the slot becomes empty using  [code]remove[/code]  function
@export var remove_slot_if_empty := false

## Sets the initial amount of slots in the inventory in the [code]_ready[/code] function
@export var slot_amount := 16

## The name of the inventory, to be displayed in UI
@export var inventory_name := "Inventory"


@export var recreate_slots_on_ready := true


func _ready():
	super._ready()
	if not Engine.is_editor_hint():
		_load_slots()


## Define slot specific index information
func set_slot(slot_index : int, item : SlotItem, amount : int):
	set_slot_content(slot_index, item.definition, item.properties, amount)


func set_slot_content(slot_index : int, item : InventoryItem, properties : Dictionary, amount : int):
	if slot_index >= slots.size():
		return
	var old_amount = get_amount()
	var slot = slots[slot_index]
	slot.item.definition = item
	slot.amount = amount
	slot.item.properties = properties
	slots[slot_index] = slot
	updated_slot.emit(slot_index)
	_call_events(old_amount)


func update_slot(slot_index : int):
	updated_slot.emit(slot_index)
	_call_events(get_amount())


## Define slot specific index information
func set_slot_with_other_slot(slot_index : int, other_slot : Slot):
	if slot_index >= slots.size():
		return
	set_slot(slot_index, other_slot.item, other_slot.amount)


## Returns true if the slot is empty
func is_empty_slot(slot_index : int) -> bool:
	if slots.size() <= slot_index:
		return true
	var slot = slots[slot_index]
	if slot != null and slot.amount > 0:
		return false
	return true


## Returns true if inventory is empty
func is_empty() -> bool:
	return get_amount() == 0


## Returns true if inventory is full
func is_full() -> bool:
	for slot in slots:
		if not slot.is_full():
			return false
	return true


## Returns true if the inventory contains the quantity of the specified item
func contains(item : SlotItem, amount := 1) -> bool:
	if item == null or item.definition == null:
		return false
	var amount_in_inventory = 0
	for slot in slots:
		if slot.contains(item):
			amount_in_inventory += slot.amount
			if amount_in_inventory >= amount:
				return true
	return false


func contains_at(slot_index : int, item : SlotItem, amount := 1) -> bool:
	if item == null or item.definition == null:
		return false
	if slot_index < slots.size():
		var slot = slots[slot_index]
		if slot.contains(item):
			if slot.amount >= amount:
				return true
	return false


## Returns true if the inventory contains the quantity of the specified category
func contains_category(category : ItemCategory, amount := 1) -> bool:
	if category == null:
		return false
	var amount_in_inventory = 0
	for slot in slots:
		if slot.contains_category(category):
			amount_in_inventory += slot.amount
			if amount_in_inventory >= amount:
				return true
	return false


## Returns the index of the slot that contains the item of the category, returns -1 if there is no item of that category
func get_slot_index_with_an_item_of_category(category : ItemCategory) -> int:
	if category == null:
		return -1
	var amount_in_inventory = 0
	for i in slots.size():
		var slot = slots[i]
		if slot.contains_category(category):
			return i
	return -1


## Returns amount of the specified item in inventory
func get_amount_of(item : SlotItem) -> int:
	if item == null:
		return 0
	var amount_in_inventory = 0
	for slot in slots:
		if slot.contains(item):
			amount_in_inventory += slot.amount
	return amount_in_inventory


## Returns amount of the specified category in inventory
func get_amount_of_category(category : ItemCategory) -> int:
	if category == null:
		return 0
	var amount_in_inventory = 0;
	for slot in slots:
		if slot == null:
			continue
		if slot.contains_category(category):
			amount_in_inventory += slot.amount
	return amount_in_inventory


## Returns amount of items in inventory
func get_amount() -> int:
	var amount_in_inventory = 0
	for slot in slots:
		amount_in_inventory += slot.amount
	return amount_in_inventory


## Adds a amount of the item to the inventory and 
## returns the amount that was left and not added
func add(item : SlotItem, amount : int) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	for i in range(slots.size()):
		amount_in_interact = _add_to_slot(i, item, amount_in_interact)
	if create_slot_if_needed and amount_in_interact > 0:
		_add_slot(slots.size())
		amount_in_interact = _add_to_slot(slots.size() - 1, item, amount_in_interact)
		_call_events(old_amount)
	var added = amount - amount_in_interact
	if added > 0:
		item_added.emit(item, added)
	return amount_in_interact


## Adds a amount of the item to the specified inventory slot index
## and returns the amount left over that was not added
func add_at(slot_index : int, item : SlotItem, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	if slot_index < slots.size():
		amount_in_interact = _add_to_slot(slot_index, item, amount_in_interact)
		_call_events(old_amount)
	var added = amount - amount_in_interact
	if added > 0:
		item_added.emit(item, added)
	return amount_in_interact


## Removes a amount of the item from inventory and 
## returns the amount that was not removed
func remove(item : SlotItem, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	for i in range(slots.size()-1, -1, -1):
		var slot = slots[i]
		amount_in_interact = _remove_from_slot(i, item, amount_in_interact)
		if remove_slot_if_empty and slot.amount == 0:
			_remove_slot(i)
			_call_events(old_amount)
	var removed = amount - amount_in_interact
	if removed > 0:
		item_removed.emit(item, removed)
	return amount_in_interact


## Removes an item quantity to the specified inventory slot index
## and returns the remaining value that was not removed
func remove_at(slot_index : int, item : SlotItem, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	if slot_index < slots.size():
		var slot = slots[slot_index]
		amount_in_interact = _remove_from_slot(slot_index, item, amount_in_interact)
		if remove_slot_if_empty and slot.amount == 0:
			_remove_slot(slot_index)
			_call_events(old_amount)
	var removed = amount - amount_in_interact
	if removed > 0:
		item_removed.emit(item, removed)
	return amount_in_interact


## Opens the inventory and returns true if done successfully.
## Emits the [b]opened[/b] signal if the was previously closed.
func open() -> bool:
	if !is_open:
		is_open = true
		opened.emit()
		return true
	return false


## Closes the inventory and returns true if done successfully.
## Emits the [b]closed[/b] signal if was previously open.
func close() -> bool:
	if is_open:
		is_open = false
		closed.emit()
		return true
	return false


func _load_slots():
	for i in self.slots.size():
		var slot = self.slots[i]
		if slot == null:
			var temp_slot = Slot.new()
			slot = temp_slot
		self.slots[i] = slot
	
	var slots = self.slots.duplicate(true)
	self.slots = []
	for i in slots.size():
		self.slots.append(slots[i].duplicate())
#	if recreate_slots_on_ready:
#		if not create_slot_if_needed:
#			for i in slot_amount:
#				_add_slot(i, false)
	for i in self.slots.size():
		var slot = self.slots[i]
		if slot.item == null:
			slot.item = SlotItem.new()
		self.slots[i] = slot


func _remove_slot(slot_index : int, emit_signal := true):
	slots.remove_at(slot_index)
	slot_removed.emit(slot_index)


func _add_slot(slot_index : int, emit_signal := true):
	var slot = Slot.new()
	slot.item = SlotItem.new()
	slot.amount = 0
	slots.insert(slot_index, slot)
	if emit_signal:
		slot_added.emit(slot_index)


func _call_events(old_amount : int):
	var actual_amount = get_amount()
	if old_amount != actual_amount:
		inventory_changed.emit()
		if is_empty():
			emptied.emit()
		if is_full():
			filled.emit()


func _add_to_slot(slot_index : int, item : SlotItem, amount := 1) -> int:
	var slot = slots[slot_index]
	var remaining_amount = slot.add(item, amount)
	if remaining_amount == amount:
		return amount
	updated_slot.emit(slot_index)
	return remaining_amount


func _remove_from_slot(slot_index : int, item : SlotItem, amount := 1) -> int:
	var slot = slots[slot_index]
	var remaining_amount = slot.remove(item, amount)
	if remaining_amount == amount:
		return amount
	updated_slot.emit(slot_index)
	return remaining_amount


