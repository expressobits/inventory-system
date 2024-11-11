extends TestSuite

@export var inventory1: Inventory
@export var inventory2: Inventory
@export var item: String


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
	]

func cleanup_test() -> void:
	inventory1.clear()
	inventory2.clear()


func test_size() -> void:
	assert(inventory1.add(item) == 0)
	assert(inventory1.stacks.size() == 1)
	assert(inventory1.remove(item) == 0)
	assert(inventory1.stacks.size() == 0)
	
	


func test_add_remove() -> void:
	assert(inventory1.add(item) == 0)
	assert(inventory1.remove(item) == 0)
	assert(inventory1.stacks.size() == 0)
	assert(inventory1.remove(item) == 1)

	assert(inventory1.add(item) == 0)
	assert(inventory1.stacks.size() == 1)
	assert(inventory1.add(item) == 0)


func test_has_item() -> void:
	assert(inventory1.add(item) == 0)
	assert(inventory1.contains(item))
	assert(!inventory1.contains(item, 2))
	assert(inventory1.remove(item) == 0)
	assert(!inventory1.contains(item))
	assert(!inventory1.contains(item, 2))


func test_transfer() -> void:
	assert(inventory1.add(item) == 0)
	assert(inventory1.transfer(0, inventory2) == 0)
	assert(!inventory1.contains(item))
	assert(inventory2.contains(item))


func test_remove_item() -> void:
	assert(inventory1.add(item) == 0)
	assert(inventory1.contains(item))
	assert(inventory1.remove(item) == 0)
	assert(!inventory1.contains(item))


func test_clear() -> void:
	inventory1.clear()
	assert(inventory1.stacks.size() == 0)
	assert(inventory1.stacks.is_empty())


func test_serialize() -> void:
	assert(inventory1.add(item) == 0)
	var inventory_data = inventory1.serialize()
	inventory1.clear()
	assert(inventory1.is_empty())
	inventory1.deserialize(inventory_data)
	assert(inventory1.stacks.size() == 1)
	assert(inventory1.stacks[0].item_id == item)
	assert(inventory1.stacks[0].amount == 1)


func test_serialize_json() -> void:
	assert(inventory1.add(item) == 0)
	var inventory_data: Dictionary = inventory1.serialize()

	## To and from JSON serialization
	var json_string: String = JSON.stringify(inventory_data)
	var test_json_conv: JSON = JSON.new()
	assert(test_json_conv.parse(json_string) == OK)
	inventory_data = test_json_conv.data

	inventory1.clear()
	assert(inventory1.is_empty())
	inventory1.deserialize(inventory_data)
	assert(!inventory1.is_empty())
	assert(inventory1.stacks.size() == 1)
	assert(inventory1.stacks[0].item_id == item)
	assert(inventory1.stacks[0].amount == 1)


func test_has_space_item() -> void:
	assert(inventory1.has_space_for(item))
	assert(inventory1.add(item, 16) == 0)
	assert(inventory1.add(item, 16) == 0)
	assert(inventory1.add(item, 16) == 0)
