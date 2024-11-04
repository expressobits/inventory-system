extends TestSuite

@export var inventory_3x3 : GridInventory
@export  var inventory_3x3_2 : GridInventory
# Item 1 x 1 (Stackable 16)
@export var wood : String = "wood"
# Item 2 x 2 (Stackable 1)
@export var stone_axe : String = "stone_axe"
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
	]


func test_has_place_for() -> void:
	# Empty inventory
	assert(inventory_3x3.has_space_for(wood))
	assert(inventory_3x3.has_space_for(stone_axe))
	
	# Inventory containing 1x1 item
	assert(inventory_3x3.add(wood) == 0)
	assert(inventory_3x3.has_space_for(stone_axe))
	#
	## Inventory containing 2x2 item
	##InventoryGridStacked.set_item_max_stack_size(item_2x2, 1)
	assert(inventory_3x3.add(campfire) == 0)
	assert(!inventory_3x3.has_space_for(stone_axe))
#
	## Inventory containing 2x2 item with extended max_stack_size
	##InventoryGridStacked.set_item_max_stack_size(item_2x2, 10)
	assert(inventory_3x3.has_space_for(campfire))
	
	inventory_3x3.clear()
	
	assert(inventory_3x3.items.size() == 0)


func test_add_item_automerge() -> void:
	## Inventory containing 2x2 item
	assert(inventory_3x3.add(stone_axe) == 0)
	assert(inventory_3x3.items.size() == 1)
	
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
	assert(inventory_3x3.items.size() == 2)
	assert(inventory_3x3.contains(wood, 2))
	inventory_3x3.clear()


func test_stack_cant_split() -> void:
	assert(inventory_3x3.add(campfire) == 0)
	assert(!inventory_3x3.split(0, 1))
	assert(inventory_3x3.items.size() == 1)
	inventory_3x3.clear()


func test_stack_join() -> void:
	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.add_on_new_stack(wood) == 0)
	assert(inventory_3x3.items.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 0)
	assert(inventory_3x3.items.size() == 1)
	assert(inventory_3x3.items[0].amount == 2)
	inventory_3x3.clear()


func test_stack_cant_join() -> void:
	assert(inventory_3x3.add(wood, 16) == 0)
	assert(inventory_3x3.add(wood, 1) == 0)
	assert(inventory_3x3.items.size() == 2)
	assert(inventory_3x3.transfer_at(1, inventory_3x3, 0, 1) == 1)
	assert(inventory_3x3.items.size() == 2)
	inventory_3x3.clear()


func test_automerge() -> void:
	assert(inventory_3x3.add(stone_axe) == 0)
	assert(inventory_3x3_2.add(campfire) == 0)
	assert(inventory_3x3_2.add(wood) == 0)
	assert(inventory_3x3.items.size() == 1)
	assert(inventory_3x3_2.items.size() == 2)
	
	# Not enough space
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 1)
	assert(inventory_3x3_2.items.size() == 2)
	assert(inventory_3x3.items.size() == 1)

	# Enough space
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.items.size() == 2)
	assert(inventory_3x3_2.items.size() == 1)
	
	inventory_3x3.clear()
	inventory_3x3_2.clear()


func test_autosplitmerge() -> void:
	assert(inventory_3x3.add(campfire, 6) == 0)
	assert(inventory_3x3_2.add(campfire, 4) == 0)
	assert(inventory_3x3_2.items[0].amount == 4)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 4) == 2)
	assert(inventory_3x3.items.size() == 1)
	assert(inventory_3x3_2.items.size() == 1)
	assert(inventory_3x3.items[0].amount == 8)
	assert(inventory_3x3_2.items[0].amount == 2)
	
	inventory_3x3.clear()
	inventory_3x3_2.clear()


func test_wrong_stack_type() -> void:
	assert(inventory_3x3.add(wood, 1, { "ok" = "2" }) == 0)
	assert(inventory_3x3_2.add(wood, 1, { "teste" = "3" }) == 0)
	assert(inventory_3x3.items.size() == 1)
	assert(inventory_3x3_2.items.size() == 1)
	assert(inventory_3x3_2.transfer(0, inventory_3x3, 1) == 0)
	assert(inventory_3x3.items.size() == 2)
	assert(inventory_3x3_2.items.size() == 0)
