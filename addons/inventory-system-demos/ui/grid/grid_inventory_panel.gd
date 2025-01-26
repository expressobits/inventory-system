extends PanelContainer
class_name GridInventoryPanel


signal request_split(inventory : Inventory, stack_index : int, amount : int)
signal request_transfer_to(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, destination_position : Vector2i, amount : int, is_rotated: bool)
signal request_fast_transfer(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, amount : int)
#signal request_sort
signal inventory_stack_context_activated(event: InputEvent, inventory : Inventory, stack: ItemStack)

@export var inventory: GridInventory:
	set(new_Inventory):
		inventory = new_Inventory
		if inventory != null:
			if title_label != null:
				title_label.text = inventory.name
			grid_inventory_ui.inventory_path = grid_inventory_ui.get_path_to(inventory)

@export var grid_inventory_ui: GridInventoryUI
@export var title_label: Label
#@onready var sort_button: Button = %SortButton


func _ready() -> void:
	#sort_button.pressed.connect(func():
		#request_sort.emit(inventory)
	#)
	grid_inventory_ui.request_split.connect(func(_inventory: GridInventory, stack_index: int, amount : int):
		request_split.emit(_inventory, stack_index, amount)
	)
	grid_inventory_ui.request_transfer_to.connect(func(origin_inventory: GridInventory, origin_position: Vector2i, destination_inventory : GridInventory, destination_position: Vector2i, amount: int, is_rotated: bool):
		request_transfer_to.emit(origin_inventory, origin_position, destination_inventory, destination_position, amount, is_rotated)
	)
	grid_inventory_ui.request_fast_transfer.connect(func(origin_inventory: GridInventory, origin_position: Vector2i, amount: int):
		request_fast_transfer.emit(origin_inventory, origin_position, amount)
	)
	grid_inventory_ui.inventory_stack_context_activated.connect(func(event: InputEvent,  _inventory: Inventory, stack: ItemStack):
		inventory_stack_context_activated.emit(event, _inventory, stack)
	)

# TODO Console mode (Code from old inventory_ui
#for stack_index in ui_stacks.size():
		#var ui_stack = ui_stacks[stack_index]
		#for neighbor in ["left", "top", "right", "bottom"]:
			#var neighbor_idx: int
			#match neighbor:
				#"left":
					#neighbor_idx = stack_index - 1
					#if stack_index % slots_container.columns == 0:
						#neighbor_idx = -1
				#"top":
					#neighbor_idx = stack_index - slots_container.columns
				#"right":
					#neighbor_idx = stack_index + 1
					#if (stack_index + 1) % slots_container.columns == 0:
						#neighbor_idx = -1
				#"bottom":
					#neighbor_idx = stack_index + slots_container.columns
			#if neighbor_idx >= 0 and neighbor_idx < ui_stacks.size():
				#ui_stack.set("focus_neighbor_"+neighbor, ui_stacks[neighbor_idx].get_path())
