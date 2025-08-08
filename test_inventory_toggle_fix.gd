extends "res://addons/gut/test.gd"

# Test class for the inventory toggle fix
# This validates the logic changes made to fix issue #131

class MockInventory:
	var is_open: bool = false

class MockCharacterInventorySystem:
	var main_inventory = MockInventory.new()
	var opened_stations = []
	var opened_inventories = []
	
	func is_open_main_inventory() -> bool:
		return opened_inventories.find(main_inventory) != -1
		
	func is_open_any_station() -> bool:
		return !opened_stations.is_empty()
		
	func is_any_station_or_inventory_opened() -> bool:
		return is_open_any_station() or is_open_main_inventory()
		
	func open_main_inventory():
		if main_inventory not in opened_inventories:
			opened_inventories.append(main_inventory)
		main_inventory.is_open = true
		
	func close_inventory(inventory):
		var index = opened_inventories.find(inventory)
		if index != -1:
			opened_inventories.remove_at(index)
		inventory.is_open = false
		
	# This is the FIXED version of inventory_inputs logic
	func handle_toggle_inventory_input():
		# Simulating: if Input.is_action_just_released(toggle_inventory_input):
		if is_open_main_inventory():
			# If inventory is open, close it
			close_inventory(main_inventory)
		elif not is_any_station_or_inventory_opened():
			# If nothing is open, open the inventory
			open_main_inventory()

func test_inventory_toggle_opens_when_nothing_open():
	var system = MockCharacterInventorySystem.new()
	
	# Initially nothing is open
	assert_false(system.is_open_main_inventory(), "Inventory should start closed")
	
	# Press "I" to open
	system.handle_toggle_inventory_input()
	
	# Should be open now
	assert_true(system.is_open_main_inventory(), "Inventory should be open after toggle")

func test_inventory_toggle_closes_when_open():
	var system = MockCharacterInventorySystem.new()
	
	# Open the inventory first
	system.open_main_inventory()
	assert_true(system.is_open_main_inventory(), "Inventory should be open initially")
	
	# Press "I" to close
	system.handle_toggle_inventory_input()
	
	# Should be closed now
	assert_false(system.is_open_main_inventory(), "Inventory should be closed after toggle")

func test_inventory_toggle_does_nothing_when_station_open():
	var system = MockCharacterInventorySystem.new()
	
	# Simulate having a craft station open
	system.opened_stations.append("mock_station")
	
	# Try to open inventory with "I"
	system.handle_toggle_inventory_input()
	
	# Should still be closed because station is open
	assert_false(system.is_open_main_inventory(), "Inventory should remain closed when station is open")

func test_complete_toggle_cycle():
	var system = MockCharacterInventorySystem.new()
	
	# Start closed
	assert_false(system.is_open_main_inventory(), "Should start closed")
	
	# First toggle: open
	system.handle_toggle_inventory_input()
	assert_true(system.is_open_main_inventory(), "Should be open after first toggle")
	
	# Second toggle: close
	system.handle_toggle_inventory_input()
	assert_false(system.is_open_main_inventory(), "Should be closed after second toggle")
	
	# Third toggle: open again
	system.handle_toggle_inventory_input()
	assert_true(system.is_open_main_inventory(), "Should be open after third toggle")