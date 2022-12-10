extends Node
class_name Inventory

@export var fixed_size := false
@export var slots = [{ "id" = 0, "amount" = 0, "capacity" = 16}]


func add(id : int, amount := 1) -> int:
	var amount_to_add = amount
	for slot in slots:
		amount_to_add = _add_to_slot(slot, id, amount)
#	if amount_to_add > 0 and not fixed_size:
#		slots.append({ "id" = 0 })
	return amount_to_add

func remove(id : int, amount := 1) -> int:
	var amount_to_remove = amount
	for slot in slots:
		amount_to_remove = _remove_from_slot(slot, id, amount)
	return amount_to_remove


#Slots
func _add_to_slot(slot : Dictionary, item : int, amount := 1) -> int:
	if amount <= 0:
		return amount
	var amount_to_add = min(amount, slot.capacity - slot.amount)
	slot.amount += amount_to_add
	# TODO Check if remove item information
	if amount_to_add > 0 and slot.item == 0:
		slot.item = item
	return amount - amount_to_add


func _remove_from_slot(slot : Dictionary, item : int, amount := 1) -> int:
	if amount <= 0:
		return amount
	var amount_to_remove = min(amount, slot.amount)
	slot.amount -= amount_to_remove
	# TODO Check if remove item information
	if slot.amount <= 0:
		slot.item = 0
	return amount - amount_to_remove
