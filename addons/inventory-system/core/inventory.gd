extends Inventory

signal inventory_changed

@export var slots = [{ "item" = 0, "amount" = 0, "capacity" = 16}]

func add(item : int, amount := 1) -> int:
	var amount_in_interact = amount
	for slot in slots:
		amount_in_interact = _add_to_slot(slot, item, amount_in_interact)
	if amount_in_interact > 0 and not fixed_size:
		var slot = { "item" = 0, "amount" = 0, "capacity" = 16}
		slots.append(slot)
		amount_in_interact = _add_to_slot(slot, item, amount_in_interact)
	_check_if_inventory_changed(amount, amount_in_interact)
	return amount_in_interact


func remove(item : int, amount := 1) -> int:
	var amount_in_interact = amount
	var index := 0
	for slot in slots:
		amount_in_interact = _remove_from_slot(slot, item, amount_in_interact)
		if slot.amount == 0 and not fixed_size:
			slots.remove_at(index)
		index += 1
	_check_if_inventory_changed(amount, amount_in_interact)
	return amount_in_interact


func has(item : int, amount := 1) -> bool:
	var amount_in_inventory := 0
	for slot in slots:
		if slot.item == item:
			amount_in_inventory += slot.amount
			if amount_in_inventory > amount:
				return true
	return false


func get_amount_of(item : int) -> int:
	var amount := 0
	for slot in slots:
		if slot.item == item:
			amount += slot.amount
	return amount


func clear():
	slots.clear()


#Signals
func _check_if_inventory_changed(amount, amount_in_interact):
	if amount != 0 and amount != amount_in_interact:
		emit_signal("inventory_changed")


#Slots
func _add_to_slot(slot : Dictionary, item : int, amount := 1) -> int:
	if amount <= 0 or (slot.item != item and slot.item != 0):
		return amount
	var amount_to_add = min(amount, slot.capacity - slot.amount)
	slot.amount += amount_to_add
	# TODO Check if remove item information
	if amount_to_add > 0 and slot.item == 0:
		slot.item = item
	return amount - amount_to_add


func _remove_from_slot(slot : Dictionary, item : int, amount := 1) -> int:
	if amount <= 0 or slot.item != item:
		return amount
	var amount_to_remove = min(amount, slot.amount)
	slot.amount -= amount_to_remove
	# TODO Check if remove item information
	if slot.amount <= 0:
		slot.item = 0
	return amount - amount_to_remove
