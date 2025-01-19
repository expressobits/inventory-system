@tool
extends Control
class_name GridInventoryContentUI

signal item_dropped(item, offset)
signal selection_changed
signal inventory_item_activated(item)

signal request_transfer_to(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, destination_position: Vector2i, amount: int, is_rotated: bool)
enum SelectMode {SELECT_SINGLE = 0, SELECT_MULTI = 1}

@export var grid_item_stack_ui_scene: PackedScene

@export var field_dimensions: Vector2 = Vector2(32, 32):
	set(new_field_dimensions):
		if new_field_dimensions == field_dimensions:
			return
		field_dimensions = new_field_dimensions
		#_queue_refresh()

@export var item_spacing: int = 0:
	set(new_item_spacing):
		if new_item_spacing == item_spacing:
			return
		item_spacing = new_item_spacing
		#_queue_refresh()

@export var inventory_path: NodePath:
	set(new_inv_path):
		if new_inv_path == inventory_path:
			return
		inventory_path = new_inv_path
		var node: Node = get_node_or_null(inventory_path)

		if node == null:
			return

		if is_inside_tree():
			assert(node is GridInventory)
			
		inventory = node
		update_configuration_warnings()

@export_enum("Single", "Multi") var select_mode: int = SelectMode.SELECT_SINGLE:
	set(new_select_mode):
		if select_mode == new_select_mode:
			return
		select_mode = new_select_mode
		grid_inventory_ui._clear_selection()


var inventory: GridInventory = null:
	set(new_inventory):
		if inventory == new_inventory:
			return

		#grid_inventory_ui._clear_selection()

		inventory = new_inventory

		#_queue_refresh()

var _ctrl_item_container: Control = null
var _grid_drop_zone_ui: GridDropZoneUI = null
var _refresh_queued: bool = false
var grid_inventory_ui : GridInventoryUI


func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE

	_ctrl_item_container = Control.new()
	_ctrl_item_container.size = size
	_ctrl_item_container.mouse_filter = Control.MOUSE_FILTER_IGNORE
	resized.connect(func(): _ctrl_item_container.size = size)
	add_child(_ctrl_item_container)

	_grid_drop_zone_ui = GridDropZoneUI.new()
	_grid_drop_zone_ui.dragable_dropped.connect(grid_inventory_ui._on_dragable_dropped)
	_grid_drop_zone_ui.size = size
	resized.connect(func(): _grid_drop_zone_ui.size = size)
	GridDraggableElementUI.dragable_grabbed.connect(func(dragable: GridDraggableElementUI, grab_position: Vector2):
		_grid_drop_zone_ui.activate()
	)
	GridDraggableElementUI.dragable_dropped.connect(func(dragable: GridDraggableElementUI, zone: GridDropZoneUI, drop_position: Vector2):
		_grid_drop_zone_ui.deactivate()
	)
	add_child(_grid_drop_zone_ui)

	if has_node(inventory_path):
		inventory = get_node_or_null(inventory_path)

	#_queue_refresh()


func _notification(what: int) -> void:
	if what == NOTIFICATION_DRAG_END:
		_grid_drop_zone_ui.deactivate()





func _on_item_grab(offset: Vector2, grid_item_stack_ui: GridItemStackUI) -> void:
	grid_inventory_ui._clear_selection()


func _on_item_drop(zone: GridDropZoneUI, drop_position: Vector2, grid_item_stack_ui: GridItemStackUI) -> void:
	var stack: ItemStack = grid_item_stack_ui.item
	# The item might have been freed in case the item stack has been moved and merged with another
	# stack.
	if is_instance_valid(stack) and inventory.has_stack(stack):
		if zone == null:
			item_dropped.emit(stack, drop_position + grid_item_stack_ui.position)


func get_field_coords(local_pos: Vector2) -> Vector2i:
	# We have to consider the item spacing when calculating field coordinates, thus we expand the
	# size of each field by Vector2(item_spacing, item_spacing).
	var field_dimensions_ex = field_dimensions + Vector2(item_spacing, item_spacing)

	# We also don't want the item spacing to disturb snapping to the closest field, so we add half
	# the spacing to the local coordinates.
	var local_pos_ex = local_pos + (Vector2(item_spacing, item_spacing) / 2)

	var x: int = local_pos_ex.x / (field_dimensions_ex.x)
	var y: int = local_pos_ex.y / (field_dimensions_ex.y)
	return Vector2i(x, y)


func get_selected_inventory_item() -> ItemStack:
	if grid_inventory_ui._selected_stacks.is_empty():
		return null
	return grid_inventory_ui._selected_stacks[0]


func get_selected_inventory_items() -> Array[ItemStack]:
	return grid_inventory_ui._selected_stacks.duplicate()


func deselect_inventory_item() -> void:
	grid_inventory_ui._clear_selection()


func select_inventory_item(stack: ItemStack) -> void:
	grid_inventory_ui._select(stack)
