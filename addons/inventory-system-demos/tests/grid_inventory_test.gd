extends TestSuite
# Grid Inventory Tests
# This script tests the functionality of the GridInventory class.
# It includes tests for adding, removing, splitting, and merging stacks,
# as well as checking for available space and serialization.
# The tests are designed to ensure that the GridInventory behaves correctly
# according to the expected behavior defined in the InventoryDatabase.

@export var wood: String = "wood"
@export var stone_pickaxe: String = "stone_pickaxe"
@export var campfire: String = "campfire"
@export var database: InventoryDatabase


func init_suite():
	tests = [
		"test_has_place_for",
		"test_add_item_automerge",
		"test_stack_split",
		"test_stack_cant_split",
		"test_stack_join",
		"test_stack_cant_join",
		"test_automerge",
		"test_autosplitmerge",
		"test_wrong_stack_type",
		"test_clear",
		"test_serialize",
		"test_full",
		"test_get_stack_at",
		"test_transfer_to_with_stack",
		"test_has_space_for"
	]


func test_has_place_for() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.has_space_for(wood))
	assert(inventory_3x3.has_space_for(stone_pickaxe))

	assert(inventory_3x3.add(wood) == 0)
	assert(inventory_3x3.has_space_for(stone_pickaxe))

	assert(inventory_3x3.add(campfire) == 0)
	assert(!inventory_3x3.has_space_for(stone_pickaxe))

	inventory_3x3.clear()
	assert(inventory_3x3.stacks.size() == 0)
	inventory_3x3.free()


func test_add_item_automerge() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add(stone_pickaxe) == 0)
	assert(inventory_3x3.stacks.size() == 1)

	inventory_3x3.clear()
	assert(inventory_3x3.add(campfire, 8) == 0)
	assert(inventory_3x3.add(campfire, 1) == 1)
	assert(inventory_3x3.add(wood) == 0)

	inventory_3x3.free()


func test_stack_split() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add(wood, 2) == 0)
	assert(inventory_3x3.split(0, 1))
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.contains(wood, 2))
	inventory_3x3.free()


func test_stack_cant_split() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add(campfire) == 0)
	assert(!inventory_3x3.split(0, 1))
	assert(inventory_3x3.stacks.size() == 1)
	inventory_3x3.free()


func test_stack_join() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3.stacks[0].amount == 2)
	inventory_3x3.free()


func test_stack_cant_join() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 1)
	assert(inventory_3x3.stacks.size() == 2)
	inventory_3x3.free()


func test_automerge() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database
	var inventory_3x3_2 := GridInventory.new()
	inventory_3x3_2.set_size(Vector2i(3, 3))
	inventory_3x3_2.database = database

	assert(inventory_3x3.add(stone_pickaxe) == 0)
	assert(inventory_3x3_2.add(campfire) == 0)
	assert(inventory_3x3_2.add(wood) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 2)

	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 1)
	assert(inventory_3x3_2.stacks.size() == 2)
	assert(inventory_3x3.stacks.size() == 1)

	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3_2.stacks.size() == 1)

	inventory_3x3.free()
	inventory_3x3_2.free()


func test_autosplitmerge() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database
	var inventory_3x3_2 := GridInventory.new()
	inventory_3x3_2.set_size(Vector2i(3, 3))
	inventory_3x3_2.database = database

	assert(inventory_3x3.add(campfire, 6) == 0)
	assert(inventory_3x3_2.add(campfire, 4) == 0)
	assert(inventory_3x3_2.stacks[0].amount == 4)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 4) == 2)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 1)
	assert(inventory_3x3.stacks[0].amount == 8)
	assert(inventory_3x3_2.stacks[0].amount == 2)

	inventory_3x3.free()
	inventory_3x3_2.free()


func test_get_stack_at() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add_at_position(Vector2i(1, 1), "wood", 4) == 0)
	assert(inventory_3x3.add_at_position(Vector2i(2, 1), "stone", 3) == 0)
	var stack = inventory_3x3.get_stack_at(Vector2i(1, 1))
	assert(stack != null)
	var index = inventory_3x3.stacks.find(stack)
	assert( ! (index < 0 or index >= inventory_3x3.stacks.size()))
	inventory_3x3.free()


func test_transfer_to_with_stack() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add_at_position(Vector2i(1, 1), "wood", 4) == 0)
	assert(inventory_3x3.add_at_position(Vector2i(2, 1), "stone", 3) == 0)
	assert(inventory_3x3.transfer_to(Vector2i(1, 1), inventory_3x3, Vector2i(2, 1), 4) == 0)
	assert(inventory_3x3.get_stack_at(Vector2i(1, 1)).item_id == "stone")
	inventory_3x3.free()


func test_wrong_stack_type() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database
	var inventory_3x3_2 := GridInventory.new()
	inventory_3x3_2.set_size(Vector2i(3, 3))
	inventory_3x3_2.database = database

	assert(inventory_3x3.add(wood, 1, { "ok" = "2" }) == 0)
	assert(inventory_3x3_2.add(wood, 1, { "teste" = "3" }) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 1)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3_2.stacks.size() == 0)

	inventory_3x3.free()
	inventory_3x3_2.free()


func test_clear() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.add(wood, 1) == 0)
	assert(inventory_3x3.get_quad_tree().get_first(Vector2i(0, 0)) != null)
	assert(!inventory_3x3.get_quad_tree().is_empty())
	assert(inventory_3x3.stack_positions.size() == 1)
	inventory_3x3.clear()
	assert(inventory_3x3.stacks.size() == 0)
	assert(inventory_3x3.stacks.is_empty())
	assert(inventory_3x3.stacks.is_empty())
	assert(inventory_3x3.get_quad_tree().get_first(Vector2i(0, 0)) == null)
	assert(inventory_3x3.get_quad_tree().is_empty())
	assert(inventory_3x3.stack_positions.size() == 0)
	inventory_3x3.free()


func test_full() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database

	assert(inventory_3x3.has_free_place(Vector2i(1, 1)))
	assert(!inventory_3x3.is_full())
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(!inventory_3x3.is_full())
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(!inventory_3x3.is_full())
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.is_full())
	inventory_3x3.free()


func test_serialize() -> void:
	var inventory_3x3 := GridInventory.new()
	inventory_3x3.set_size(Vector2i(3, 3))
	inventory_3x3.database = database
	var inventory_3x3_2 := GridInventory.new()
	inventory_3x3_2.set_size(Vector2i(3, 3))
	inventory_3x3_2.database = database

	assert(inventory_3x3.add_at_position(Vector2i(0, 1), wood, 1) == 0)
	assert(inventory_3x3.stack_positions.size() == 1)
	assert(inventory_3x3.stack_positions[0] == Vector2i(0, 1))
	var data = inventory_3x3.serialize()
	inventory_3x3.clear()
	inventory_3x3_2.deserialize(data)
	inventory_3x3.deserialize(data)
	assert(inventory_3x3_2.contains(wood, 1))
	assert(inventory_3x3_2.stack_positions.size() == 1)
	assert(inventory_3x3_2.get_quad_tree().get_first(Vector2i(0, 1)) != null)
	inventory_3x3.free()
	inventory_3x3_2.free()


func test_move_workbench() -> void:
	var inventory_8x1 := GridInventory.new()
	inventory_8x1.set_size(Vector2i(8, 1))
	inventory_8x1.database = database
	var inventory_8x5 := GridInventory.new()
	inventory_8x5.set_size(Vector2i(8, 5))
	inventory_8x5.database = database

	inventory_8x5.add("workbench", 1)
	var stack_index = inventory_8x5.get_stack_index_at(Vector2i(0, 0))
	if stack_index == -1:
		inventory_8x1.free()
		inventory_8x5.free()
		return
	print(stack_index)
	var amount = inventory_8x5.transfer(stack_index, inventory_8x1, 1)
	assert(amount == 0)
	inventory_8x1.free()
	inventory_8x5.free()


func test_has_space_for() -> void:
	var inventory_8x1 := GridInventory.new()
	inventory_8x1.set_size(Vector2i(8, 1))
	inventory_8x1.database = database
	var inventory_8x5 := GridInventory.new()
	inventory_8x5.set_size(Vector2i(8, 5))
	inventory_8x5.database = database

	assert(!inventory_8x1.has_space_for("workbench"))
	assert(inventory_8x5.has_space_for("workbench"))

	inventory_8x1.free()
	inventory_8x5.free()
