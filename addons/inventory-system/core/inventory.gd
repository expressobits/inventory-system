extends Inventory

signal inventory_changed

@export var slots : Array
@export var database := InventoryDatabase

func add(item : Item, amount := 1) -> int:
	var amount_in_interact = amount
	for slot in slots:
		amount_in_interact = slot.add(item, amount_in_interact)
	if amount_in_interact > 0 and not fixed_size:
		var slot = Slot.new()
		slot.amount = 0
		slot.item = null
		slots.append(slot)
		amount_in_interact = slot.add(item, amount_in_interact)
	_check_if_inventory_changed(amount, amount_in_interact)
	return amount_in_interact


func remove(item : Item, amount := 1) -> int:
	var amount_in_interact = amount
	var index := 0
	for slot in slots:
		amount_in_interact = slot.remove(item, amount_in_interact)
		if slot.amount == 0 and not fixed_size:
			slots.remove_at(index)
		index += 1
	_check_if_inventory_changed(amount, amount_in_interact)
	return amount_in_interact


func has(item : Item, amount := 1) -> bool:
	var amount_in_inventory := 0
	for slot in slots:
		if slot.item == item:
			amount_in_inventory += slot.amount
			if amount_in_inventory > amount:
				return true
	return false


func get_amount_of(item : Item) -> int:
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



func _remove_from_slot(slot : Slot, item : Item, amount := 1) -> int:
	if amount <= 0 or slot.item != item:
		return amount
	var amount_to_remove = min(amount, slot.amount)
	slot.amount -= amount_to_remove
	# TODO Check if remove item information
	if slot.amount <= 0:
		slot.item = null
	return amount - amount_to_remove
