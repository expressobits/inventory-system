extends TestSuite

@export var item: String = "wood"
@export var database: InventoryDatabase


func init_suite() -> void:
	tests = [
		"test_size",
		"test_has_item",
		"test_add_remove",
		"test_transfer",
		"test_remove_item",
		"test_serialize",
		"test_clear",
		"test_serialize_json",
		"test_has_space_item",
		"test_create_and_add_wood",
		"test_is_empty",
		"test_item_added_signal",
		"test_item_removed_signal",
		"test_stack_added_signal",
		"test_stack_removed_signal",
	]


func cleanup_suite() -> void:
	print("Cleaning up tests suite")
	item = ""
	database = null


func test_size() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	assert(inventory.stacks.size() == 1)
	assert(inventory.remove(item) == 0)
	assert(inventory.stacks.size() == 0)
	inventory.free()


func test_add_remove() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	assert(inventory.remove(item) == 0)
	assert(inventory.stacks.size() == 0)
	assert(inventory.remove(item) == 1)

	assert(inventory.add(item) == 0)
	assert(inventory.stacks.size() == 1)
	assert(inventory.add(item) == 0)
	inventory.free()


func test_has_item() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	assert(inventory.contains(item))
	assert(!inventory.contains(item, 2))
	assert(inventory.remove(item) == 0)
	assert(!inventory.contains(item))
	assert(!inventory.contains(item, 2))
	inventory.free()


func test_transfer() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	var other_inventory = Inventory.new()
	other_inventory.database = database
	assert(inventory.add(item) == 0)
	assert(inventory.transfer(0, other_inventory) == 0)
	assert(!inventory.contains(item))
	assert(other_inventory.contains(item))
	inventory.free()
	other_inventory.free()


func test_remove_item() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	assert(inventory.contains(item))
	assert(inventory.remove(item) == 0)
	assert(!inventory.contains(item))
	inventory.free()


func test_clear() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	inventory.clear()
	assert(inventory.stacks.size() == 0)
	assert(inventory.stacks.is_empty())
	inventory.free()


func test_serialize() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	var inventory_data = inventory.serialize()
	inventory.clear()
	assert(inventory.is_empty())
	inventory.deserialize(inventory_data)
	assert(inventory.stacks.size() == 1)
	assert(inventory.stacks[0].item_id == item)
	assert(inventory.stacks[0].amount == 1)
	inventory.free()


func test_serialize_json() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.add(item) == 0)
	var inventory_data: Dictionary = inventory.serialize()

	## To and from JSON serialization
	var json_string: String = JSON.stringify(inventory_data)
	var test_json_conv: JSON = JSON.new()
	assert(test_json_conv.parse(json_string) == OK)
	inventory_data = test_json_conv.data

	inventory.clear()
	assert(inventory.is_empty())
	inventory.deserialize(inventory_data)
	assert(!inventory.is_empty())
	assert(inventory.stacks.size() == 1)
	assert(inventory.stacks[0].item_id == item)
	assert(inventory.stacks[0].amount == 1)
	inventory.free()


func test_has_space_item() -> void:
	var inventory = Inventory.new()
	inventory.database = database
	assert(inventory.has_space_for(item))
	assert(inventory.add(item, 16) == 0)
	assert(inventory.add(item, 16) == 0)
	assert(inventory.add(item, 16) == 0)
	inventory.free()


func test_is_empty() -> void:
	var inventory = Inventory.new()
	inventory.database = database

	# Initial case: inventory is empty
	assert(inventory.is_empty())

	# Add an item and verify that the inventory is no longer empty
	assert(inventory.add(item) == 0)
	assert(!inventory.is_empty())

	# Remove the item and verify that the inventory is empty again
	assert(inventory.remove(item) == 0)
	assert(inventory.is_empty())

	# Add a null stack and verify that the inventory is still considered empty
	inventory.stacks.append(null)
	assert(inventory.is_empty())

	inventory.free()

# Variable to track if the signal was emitted
var signal_emitted = false


func test_item_added_signal() -> void:
	var inventory = Inventory.new()
	inventory.database = database

	# Connect the signal to a callback function
	inventory.item_added.connect(func(_item_id: String, _amount: int):
		signal_emitted = true
	)

	# Add an item and verify that the signal is emitted
	assert(inventory.add(item) == 0)
	assert(signal_emitted)

	inventory.free()

# Variable to track if the signal was emitted
var signal_removed_emitted = false


func test_item_removed_signal() -> void:
	var inventory = Inventory.new()
	inventory.database = database

	# Connect the signal to a callback function
	inventory.item_removed.connect(func(_item_id: String, _amount: int):
		signal_removed_emitted = true
	)

	# Add an item to the inventory
	assert(inventory.add(item) == 0)

	# Remove the item and verify that the signal is emitted
	assert(inventory.remove(item) == 0)
	assert(signal_removed_emitted)

	inventory.free()

# Variable to track if the stack_added signal was emitted
var stack_added_emitted = false


func test_stack_added_signal() -> void:
	var inventory = Inventory.new()
	inventory.database = database

	# Connect the signal to a callback function
	inventory.stack_added.connect(func(_stack_index: int):
		stack_added_emitted = true
	)

	# Add an item and verify that the stack_added signal is emitted
	assert(inventory.add(item) == 0)
	assert(stack_added_emitted)

	inventory.free()

# Variable to track if the stack_removed signal was emitted
var stack_removed_emitted = false


func test_stack_removed_signal() -> void:
	var inventory = Inventory.new()
	inventory.database = database

	# Connect the signal to a callback function
	inventory.stack_removed.connect(func(_stack_index: int):
		stack_removed_emitted = true
	)

	# Add an item to the inventory
	assert(inventory.add(item) == 0)

	# Remove the item and verify that the stack_removed signal is emitted
	assert(inventory.remove(item) == 0)
	assert(stack_removed_emitted)

	inventory.free()
