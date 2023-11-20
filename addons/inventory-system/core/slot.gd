@tool
extends Resource
class_name Slot

@export var item : InventoryItem
@export var amount := 0
@export var max_stack := -1


func get_item_id() -> int:
	if item == null:
		return InventoryItem.NONE
	else:
		return item.id


func add(item : InventoryItem, amount : int) -> int:
	if item == null:
		return amount
	if amount <= 0 or (self.item != item and self.item != null):
		return amount
	var max_stack = get_max_stack_for_item(item)
	var amount_to_add = min(amount, max_stack - self.amount)
	self.amount = self.amount + amount_to_add
	if amount_to_add > 0 and self.item == null:
		self.item = item
		updated.emit()
	return amount - amount_to_add


func remove(item : InventoryItem, amount : int) -> int:
	if item == null:
		return amount
	if amount <= 0 or (self.item != item && self.item != null):
		return amount
	var amount_to_remove = min(amount, self.amount)
	self.amount = self.amount - amount_to_remove
	updated.emit()
	if self.amount <= 0:
		self.item = null
	return amount - amount_to_remove


## TODO Remove calls in inventory, passed responsability to slot.gd
func move(to_slot : Slot, amount_to_move : int = -1):
	if not has_valid():
		return
	if to_slot.is_empty() or to_slot.item == item:
		if amount_to_move >= 0:
			amount = amount_to_move
		var amount_no_add = to_slot.add(item, amount)
		remove(item, amount - amount_no_add)
		updated.emit()
		to_slot.updated.emit()
	else:
		if to_slot is CategorizedSlot:
			var c_slot = to_slot as CategorizedSlot
			if not c_slot.is_accept_category(item):
				return
		var new_item = item
		var new_amount = amount
		item = to_slot.item
		amount = to_slot.amount
		to_slot.item = new_item
		to_slot.amount = new_amount
		updated.emit()
		to_slot.updated.emit()


func get_max_stack_for_item(item : InventoryItem) -> int:
	if max_stack == -1:
		return item.max_stack
	else:
		return max_stack


func get_max_stack() -> int:
	return get_max_stack_for_item(item)


func is_full() -> bool:
	if item == null:
		return false
	if amount < get_max_stack():
		return false
	return true


func is_empty() -> bool:
	return amount <= 0


func has_valid() -> bool:
	return item != null and amount > 0
