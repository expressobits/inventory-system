extends Node
class_name InventoryHandler

signal dropped
signal picked
signal added(item : Item, amount : int)
signal opened(inventory : Inventory)
signal closed(inventory : Inventory)
signal started_transaction(item : Item, amount : int)
signal stopped_transaction

@export_node_path(Inventory) var inventory_path := NodePath("Player Inventory")
@onready var inventory = get_node(inventory_path)

var opened_inventories : Array
var transaction_slot := {
	"item": null,
	"amount": 0,
	"inventory": null,
	"last_slot_index": -1
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
	emit_signal("opened", inventory)
	opened_inventories.append(inventory)
	return true
	
func close(inventory : Inventory) -> bool:
	var index = opened_inventories.find(inventory)
	if index == -1:
		return false
	opened_inventories.remove_at(index)
	emit_signal("closed", inventory)
	return true
	
func is_open_personal_inventory() -> bool:
	return opened_inventories.has(inventory)
	
func open_personal_inventory() -> bool:
	return open(inventory)
	
func close_all_containers():
	for i in range(opened_inventories.size() - 1, -1, -1):
		var inv = opened_inventories[0]
		close(inv)
		
# Temp Slot
func add_to_transaction_slot(slot_index : int, inventory : Inventory, amount : int):
	var slot = inventory.slots[slot_index];
	var item = slot.item
	var no_remove = inventory.remove_at(slot_index, item, amount)
	amount = amount - no_remove
	transaction_slot.item = item
	transaction_slot.amount = amount
	transaction_slot.last_slot_index = slot_index
	transaction_slot.inventory = inventory
	emit_signal("started_transaction", item, amount)

# Cancel transaction operation (Drag and drop)
func cancel_transaction_operation():
	if not is_transaction_active():
		return
	var inventory = transaction_slot.inventory
	var slot_index = transaction_slot.last_slot_index
	var item = transaction_slot.item
	var amount = transaction_slot.amount
	var amount_no_add = inventory.add_at(slot_index, item, amount)
	
	# Drop amount no add
	if amount_no_add:
		drop(item, amount_no_add)
	clear_transaction()
	
func clear_transaction():
	transaction_slot.inventory = null
	transaction_slot.last_slot_index = -1
	transaction_slot.amount = 0
	transaction_slot.item = null
	emit_signal("stopped_transaction")
	
func finish_transaction_to(slot_index : int, inventory : Inventory):
	if not is_transaction_active():
		return
	var slot = inventory.slots[slot_index]
	var item = transaction_slot.item
	if inventory.is_empty_slot(slot_index) or slot.item == item:
		var amount_no_add = inventory.add_at(slot_index, item, transaction_slot.amount)
		if amount_no_add > 0:
			print(amount_no_add)
			transaction_slot.amount = amount_no_add
			cancel_transaction_operation()
		else:
			clear_transaction()
	else:
		# Different items in slot and other_slot
		# Check if transaction_slot amount is equal of origin_slot amount
		# TODO See in minecraft
		var origin_slot = transaction_slot.inventory.slots[transaction_slot.last_slot_index]
		if origin_slot.amount == 0:
			inventory.slots[slot_index].item = transaction_slot.item
			inventory.slots[slot_index].amount = transaction_slot.amount
			transaction_slot.item = slot.item
			transaction_slot.amount = slot.amount
			inventory.emit_signal("updated_slot",slot_index)
			transaction_slot.inventory.emit_signal("updated_slot",transaction_slot.last_slot_index)
			cancel_transaction_operation()
	if transaction_slot.amount == 0:
		cancel_transaction_operation()
	

func is_transaction_active() -> bool:
	return transaction_slot.item != null
