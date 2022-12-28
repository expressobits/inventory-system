extends Inventory

@export var database := InventoryDatabase


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


#Signals
func _check_if_inventory_changed(amount, amount_in_interact):
	if amount != 0 and amount != amount_in_interact:
		emit_signal("inventory_changed")
