extends TestSuite

@export var inventory_3x3 : GridInventory
@export  var inventory_3x3_2 : GridInventory
# Item 1 x 1 (Stackable 16)
@export var wood : String = "wood"
# Item 2 x 2 (Stackable 1)
@export var stone_pickaxe : String = "stone_pickaxe"
# Item 2 x 2 (Stackable 8)
@export var campfire : String = "campfire"


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
		"test_full"
	]


func test_has_place_for() -> void:
	# Empty inventory
	assert(inventory_3x3.has_space_for(wood))
	assert(inventory_3x3.has_space_for(stone_pickaxe))
	
	# Inventory containing 1x1 item
	assert(inventory_3x3.add(wood) == 0)
	assert(inventory_3x3.has_space_for(stone_pickaxe))
	#
	## Inventory containing 2x2 item
	##InventoryGridStacked.set_item_max_stack_size(item_2x2, 1)
	assert(inventory_3x3.add(campfire) == 0)
	assert(inventory_3x3.has_space_for(stone_pickaxe))
#
	## Inventory containing 2x2 item with extended max_stack_size
	##InventoryGridStacked.set_item_max_stack_size(item_2x2, 10)
	#assert(inventory_3x3.has_space_for(campfire))
	
	inventory_3x3.clear()
	
	assert(inventory_3x3.stacks.size() == 0)


func test_add_item_automerge() -> void:
	## Inventory containing 2x2 item
	assert(inventory_3x3.add(stone_pickaxe) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	
	inventory_3x3.clear()
	## No stack space, no grid space
	assert(inventory_3x3.add(campfire, 8) == 0)
	assert(inventory_3x3.add(campfire, 1) == 1)
#
	## No stack space but grid space available
	assert(inventory_3x3.add(wood) == 0)
	
	inventory_3x3.clear()


func test_stack_split() -> void:
	assert(inventory_3x3.add(wood, 2) == 0)
	assert(inventory_3x3.split(0, 1))
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.contains(wood, 2))
	inventory_3x3.clear()


func test_stack_cant_split() -> void:
	assert(inventory_3x3.add(campfire) == 0)
	assert(!inventory_3x3.split(0, 1))
	assert(inventory_3x3.stacks.size() == 1)
	inventory_3x3.clear()


func test_stack_join() -> void:
	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3.stacks[0].amount == 2)
	inventory_3x3.clear()


func test_stack_cant_join() -> void:
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 1)
	assert(inventory_3x3.stacks.size() == 2)
	inventory_3x3.clear()


func test_automerge() -> void:
	assert(inventory_3x3.add(stone_pickaxe) == 0)
	assert(inventory_3x3_2.add(campfire) == 0)
	assert(inventory_3x3_2.add(wood) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 2)
	
	# Not enough space
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 1)
	assert(inventory_3x3_2.stacks.size() == 2)
	assert(inventory_3x3.stacks.size() == 1)

	# Enough space
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3_2.stacks.size() == 1)
	
	inventory_3x3.clear()
	inventory_3x3_2.clear()


func test_autosplitmerge() -> void:
	assert(inventory_3x3.add(campfire, 6) == 0)
	assert(inventory_3x3_2.add(campfire, 4) == 0)
	assert(inventory_3x3_2.stacks[0].amount == 4)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 4) == 2)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 1)
	assert(inventory_3x3.stacks[0].amount == 8)
	assert(inventory_3x3_2.stacks[0].amount == 2)
	
	inventory_3x3.clear()
	inventory_3x3_2.clear()


func test_wrong_stack_type() -> void:
	assert(inventory_3x3.add(wood, 1, { "ok" = "2" }) == 0)
	assert(inventory_3x3_2.add(wood, 1, { "teste" = "3" }) == 0)
	assert(inventory_3x3.stacks.size() == 1)
	assert(inventory_3x3_2.stacks.size() == 1)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.stacks.size() == 2)
	assert(inventory_3x3_2.stacks.size() == 0)
	
	inventory_3x3.clear()
	inventory_3x3_2.clear()


func test_clear() -> void:
	assert(inventory_3x3.add(wood, 1) == 0)
	assert(inventory_3x3.get_quad_tree().get_first(Vector2i(0,0)) != null)
	assert(!inventory_3x3.get_quad_tree().is_empty())
	assert(inventory_3x3.stack_positions.size() == 1)
	inventory_3x3.clear()
	assert(inventory_3x3.stacks.size() == 0)
	assert(inventory_3x3.stacks.is_empty())
	assert(inventory_3x3.stacks.is_empty())
	assert(inventory_3x3.get_quad_tree().get_first(Vector2i(0,0)) == null)
	assert(inventory_3x3.get_quad_tree().is_empty())
	assert(inventory_3x3.stack_positions.size() == 0)


func test_full() -> void:
	inventory_3x3.clear()
	assert(inventory_3x3.has_free_place(Vector2i(1,1)))
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

func test_serialize() -> void:
	assert(inventory_3x3.add_at_position(Vector2i(0, 1), wood, 1) == 0)
	assert(inventory_3x3.stack_positions.size() == 1)
	assert(inventory_3x3.stack_positions[0] == Vector2i(0, 1))
	var data = inventory_3x3.serialize()
	inventory_3x3.clear()
	inventory_3x3_2.deserialize(data)
	inventory_3x3.deserialize(data)
	assert(inventory_3x3_2.contains(wood, 1))
	assert(inventory_3x3_2.stack_positions.size() == 1)
	assert(inventory_3x3_2.get_quad_tree().get_first(Vector2i(0,1)) != null)
