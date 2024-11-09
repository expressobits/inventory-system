extends Panel
class_name GridInventoryPanel


signal request_split(inventory : Inventory, stack_index : int, amount : int)
signal request_transfer_to(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, destination_position : Vector2i, amount : int)
signal request_sort


@export var inventory: GridInventory:
	set(new_Inventory):
		inventory = new_Inventory
		if inventory != null:
			if title_label != null:
				title_label.text = inventory.name
			grid_inventory_ui.inventory_path = grid_inventory_ui.get_path_to(inventory)

@export var grid_inventory_ui: GridInventoryUI

@export var title_label: Label

@onready var sort_button: Button = %SortButton


func _ready() -> void:
	if sort_button.icon == null:
		sort_button.icon = get_theme_icon("Load", "EditorIcons")
	sort_button.pressed.connect(func():
		request_sort.emit()
	)
	grid_inventory_ui.request_split.connect(func(inventory: GridInventory, stack_index: int, amount : int):
		request_split.emit(inventory, stack_index, amount)
	)
	grid_inventory_ui.request_transfer_to.connect(func(origin_inventory: GridInventory, origin_position: Vector2i, destination_inventory : GridInventory, destination_position: Vector2i, amount : int):
		request_transfer_to.emit(origin_inventory, origin_position, destination_inventory, destination_position, amount)
	)
