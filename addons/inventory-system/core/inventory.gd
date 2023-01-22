extends Node
class_name Inventory

signal inventory_changed
signal slot_added(slot_index : int)
signal slot_removed(slot_index : int)
signal filled
signal emptied
signal updated_slot(slot_index : int)

var slots : Array
@export var create_slot_if_needed := false
@export var remove_slot_if_empty := false


# Slots
func set_slot(slot_index : int, item : Item, amount : int):
	var slot = slots[slot_index]
	slot.item = item
	slot.amount = amount
	emit_signal("updated_slot", slot_index)


func add_to_slot(slot_index : int, item : Item, amount := 1) -> int:
	var slot = slots[slot_index]
	var item_slot = slot.item
	if amount <= 0 or ( item_slot != item and item_slot != null):
		return amount
	var amount_to_add = min(amount, item.max_stack - slot.amount)
	slot.amount = slot.amount + amount_to_add;
	if amount_to_add > 0 and item_slot == null:
		slot.item = item
	emit_signal("updated_slot", slot_index)
	return amount - amount_to_add;


func remove_from_slot(slot_index : int, item : Item, amount := 1) -> int:
	var slot = slots[slot_index]
	var item_slot = slot.item
	if amount <= 0 or (item_slot != item && item_slot != null):
		return amount;
	var amount_to_remove = min(amount, slot.amount);
	slot.amount = slot.amount - amount_to_remove;
	if slot.amount <= 0:
		slot.item = null;
	emit_signal("updated_slot", slot_index);
	return amount - amount_to_remove;


func is_empty_slot(slot_index : int) -> bool:
	if slots.size() <= slot_index:
		return true
	var slot = slots[slot_index]
	if slot.has("amount") and slot.amount > 0:
		return false;
	return true;

# Utils
func is_empty() -> bool:
	return get_amount() == 0

func is_full() -> bool:
	for slot in slots:
		var item = slot.item
		if item == null or slot.amount < item.max_stack:
			return false
	return true


func contains(item : Item, amount := 1) -> bool:
	var amount_in_inventory = 0
	for slot in slots:
		if slot.item == item:
			amount_in_inventory += slot.amount
			if amount_in_inventory > amount:
				return true
	return false


func get_amount_of(item : Item) -> int:
	var amount_in_inventory = 0;
	for slot in slots:
		if slot.item == item:
			amount_in_inventory += slot.amount
	return amount_in_inventory


func get_amount() -> int:
	var amount_in_inventory = 0
	for slot in slots:
		amount_in_inventory += slot.amount
	return amount_in_inventory


# Inventory Basics
func add(item : Item, amount : int) -> int:
	var amount_in_interact = amount;
	var old_amount = get_amount()
	for i in range(slots.size()):
		amount_in_interact = add_to_slot(i, item, amount_in_interact)
	if create_slot_if_needed and amount_in_interact > 0:
		var slot = { "item": null, "amount":0 }
		slots.append(slot)
		emit_signal("slot_added", slots.size() - 1)
		amount_in_interact = add_to_slot(slots.size() - 1, item, amount_in_interact)
		_call_events(old_amount)
	return amount_in_interact


func add_at(slot_index : int, item : Item, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	if slot_index < slots.size():
		amount_in_interact = add_to_slot(slot_index, item, amount_in_interact)
		_call_events(old_amount)
	return amount_in_interact


func remove(item : Item, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	for i in range(slots.size()-1, -1, -1):
		var slot = slots[i]
		amount_in_interact = remove_from_slot(i, item, amount_in_interact)
		if remove_slot_if_empty and slot.amount == 0:
			slots.remove_at(i)
			emit_signal("slot_removed", i)
			_call_events(old_amount)
	return amount_in_interact


func remove_at(slot_index : int, item : Item, amount := 1) -> int:
	var amount_in_interact = amount
	var old_amount = get_amount()
	if slot_index < slots.size():
		var slot = slots[slot_index]
		amount_in_interact = remove_from_slot(slot_index, item, amount_in_interact)
		if remove_slot_if_empty and slot.amount == 0:
			slots.remove_at(slot_index)
			emit_signal("slot_removed", slot_index)
			_call_events(old_amount)
	return amount_in_interact


# Internal calls
func _call_events(old_amount : int):
	var actual_amount = get_amount()
	if old_amount != actual_amount:
		emit_signal("inventory_changed")
		if is_empty():
			emit_signal("emptied")
		if is_full():
			emit_signal("filled")
	
