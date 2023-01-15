extends Node
class_name InventoryHandler

signal dropped
signal picked
signal added(item : Item, amount : int)
signal opened(inventory : Inventory)
signal closed(inventory : Inventory)
signal updated_transaction_slot(item : Item, amount : int)

@export_node_path(Inventory) var inventory_path := NodePath("Player Inventory")
@onready var inventory = get_node(inventory_path)

var opened_inventories : Array
var transaction_slot := {
	"item": null,
	"amount": 0
}


# Example: Drop 3D item (For extending?)
func drop(item : Item, amount := 1) -> bool:
	return true


# Add an amount of an item to a inventory, if this addition fails and the drop_excess is true then a drop of the unadded items occurs
func add_to_inventory(inventory : Inventory, item : Item, amount := 1, drop_excess := false) -> int:
	var value_no_added = inventory.add(item, amount)
	emit_signal("added", item, amount - value_no_added)
	if (drop_excess):
		drop(item, value_no_added)
		return 0
	return value_no_added


# Drops a amount of items from a inventory
func drop_from_inventory(inventory : Inventory, slot_index : int, amount := 1):
	if inventory.slots.size() <= slot_index:
		return
	if inventory.is_empty_slot(slot_index):
		return
	var slot = inventory.slots[slot_index]
	var item = slot["item"]
	var not_removed = inventory.remove_at(slot_index, item, amount)
	var removed = amount - not_removed
	drop(item, removed)


# Pick a item_object to inventory
#func pick_to_inventory(inventory : Inventory, item_object : ItemObject):
#	if (!itemObject.IsPickable):
#		return false;
#		Item item = item_object.item;
#		if add_to_inventory(container, item) == 0:
#			OnPick?.Invoke(itemObject);
#			Destroy(itemObject.gameObject);
#			return true;
#	return false;

#func pick(item_object : ItemObject):
#	return pick_to_inventory(inventory, item_object)

# Exchanges a quantity of an item between inventories.
# First remove from the "from" inventory, then the successfully removed value is added to the "to" inventory,
# if any value is not successfully added, this value is added again to the "from" inventory at the same index,
# if in this last task values are not added with successes they will be dropped
func move_between_inventories(from : Inventory, slot_index : int, amount : int, to : Inventory):
	var slot = from.slots[slot_index];
	var item = slot.item;
	var amount_not_removed = from.remove_at(slot_index, item, amount);
	var amount_for_swap = amount - amount_not_removed;
	var amount_not_swaped = to.add(item, amount_for_swap);
	# TODO Implement Undo Tasks?
	var amount_not_undo = from.add_at(slot_index, item, amount_not_swaped);
	drop(item, amount_not_undo);


# Swap slot information for containers
func swap_between_containers(inventory : Inventory, slot_index : int, other_inventory : Inventory, other_slot_index : int, amount := 1):
	var slot = inventory.slots[slot_index];
	var other_slot = other_inventory.slots[other_slot_index];
	# Same Item in slot and other_slot
	if other_inventory.is_empty_slot(other_slot_index) or slot.item == other_slot.item:
		var item = slot.item
		var for_trade = 0
		if other_inventory.is_empty_slot(other_slot_index):
			for_trade = amount
		else:
			for_trade = min(item.max_stack - other_slot.amount, amount)
		var no_remove = inventory.remove_at(slot_index, item, for_trade)
		other_inventory.add_at(other_slot_index, item, for_trade - no_remove)
	# Different items in slot and other_slot
	else:
		if slot.amount == amount:
			inventory.slots[slot_index] = other_slot;
			other_inventory.slots[other_slot_index] = slot;
			inventory.emit_signal("updated_slot",slot_index)
			other_inventory.emit_signal("updated_slot",other_slot_index)


# Open and Close Interactions
func open(inventory : Inventory) -> bool:
	if opened_inventories.has(inventory):
		return false 
	opened_inventories.append(inventory)
	emit_signal("opened", inventory)
	return true


func close(inventory : Inventory) -> bool:
	var index = opened_inventories.find(inventory)
	if index == -1:
		return false
	opened_inventories.remove_at(index)
	emit_signal("closed", inventory)
	if self.inventory == inventory:
		if is_transaction_active():
			var amount_no_add = inventory.add(transaction_slot.item, transaction_slot.amount)
			if amount_no_add > 0:
				drop(transaction_slot.item, amount_no_add)
			set_transaction_slot(transaction_slot.item, 0)
	return true


func is_open_personal_inventory() -> bool:
	return is_open(inventory)


func is_open_any_inventory() -> bool:
	return opened_inventories.size() > 0
	

func is_open(inventory : Inventory) -> bool:
	return opened_inventories.has(inventory)


func open_personal_inventory() -> bool:
	return open(inventory)


func close_all_containers():
	for i in range(opened_inventories.size() - 1, -1, -1):
		var inv = opened_inventories[0]
		close(inv)


func to_transaction(slot_index : int, inventory : Inventory, amount : int):
	if is_transaction_active():
		return
	var slot = inventory.slots[slot_index]
	var item = slot.item
	var amount_no_removed = inventory.remove_at(slot_index, item, amount)
	set_transaction_slot(item, amount - amount_no_removed)


func transaction_to(slot_index : int, inventory : Inventory):
	if not is_transaction_active():
		return
	var slot = inventory.slots[slot_index]
	var item = transaction_slot.item
	if inventory.is_empty_slot(slot_index) or slot.item == item:
		var amount_no_add = inventory.add_at(slot_index, item, transaction_slot.amount)
		set_transaction_slot(transaction_slot.item, amount_no_add)
	else:
		# Different items in slot and other_slot
		# Check if transaction_slot amount is equal of origin_slot amount
		var new_amount = transaction_slot.amount
		set_transaction_slot(inventory.slots[slot_index].item, inventory.slots[slot_index].amount)
		inventory.set_slot(slot_index, item, new_amount)


func set_transaction_slot(item : Item, amount : int):
	transaction_slot.item = item
	transaction_slot.amount = amount
	emit_signal("updated_transaction_slot", item, amount)
	

func is_transaction_active() -> bool:
	return transaction_slot.amount > 0
	
func drop_transaction():
	if is_transaction_active():
		drop(transaction_slot.item, transaction_slot.amount)
	set_transaction_slot(null, 0)
