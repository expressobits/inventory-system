extends Panel
class_name InventoryPanel

@export var inventory: GridInventory:
	set(new_Inventory):
		inventory = new_Inventory
		if inventory != null:
			grid_inventory_ui.inventory_path = grid_inventory_ui.get_path_to(inventory)
		
		
@onready var grid_inventory_ui: GridInventoryUI = %GridInventoryUI


func disconnect_old_inventory():
	inventory = null
