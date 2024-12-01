@tool
extends Control
class_name GridInventoryContentUI

signal item_dropped(item, offset)
signal selection_changed
signal inventory_item_activated(item)
signal inventory_stack_context_activated(event: InputEvent, item: ItemStack)
signal item_mouse_entered(item)
signal item_mouse_exited(item)

signal request_split(inventory: GridInventory, stack_index : int, amount : int)
signal request_transfer_to(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, destination_position: Vector2i, amount: int, is_rotated: bool)

enum SelectMode {SELECT_SINGLE = 0, SELECT_MULTI = 1}

@export var grid_item_stack_ui_scene: PackedScene

@export var field_dimensions: Vector2 = Vector2(32, 32):
	set(new_field_dimensions):
		if new_field_dimensions == field_dimensions:
			return
		field_dimensions = new_field_dimensions
		_queue_refresh()

@export var item_spacing: int = 0:
	set(new_item_spacing):
		if new_item_spacing == item_spacing:
			return
		item_spacing = new_item_spacing
		_queue_refresh()

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

@export var default_item_texture: Texture2D:
	set(new_default_item_texture):
		if new_default_item_texture == default_item_texture:
			return
		default_item_texture = new_default_item_texture
		_queue_refresh()

@export_enum("Single", "Multi") var select_mode: int = SelectMode.SELECT_SINGLE:
	set(new_select_mode):
		if select_mode == new_select_mode:
			return
		select_mode = new_select_mode
		_clear_selection()


var inventory: GridInventory = null:
	set(new_inventory):
		if inventory == new_inventory:
			return

		_clear_selection()

		_disconnect_inventory_signals()
		inventory = new_inventory
		_connect_inventory_signals()

		_queue_refresh()

var _ctrl_item_container: Control = null
var _grid_drop_zone_ui: GridDropZoneUI = null
var _selected_items: Array[ItemStack] = []
var _refresh_queued: bool = false


func _get_configuration_warnings() -> PackedStringArray:
	if inventory_path.is_empty():
		return PackedStringArray([
				"This node is not linked to an inventory and it can't display any content.\n" + \
				"Set the inventory_path property to point to an InventoryGrid node."])
	return PackedStringArray()


func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE

	_ctrl_item_container = Control.new()
	_ctrl_item_container.size = size
	_ctrl_item_container.mouse_filter = Control.MOUSE_FILTER_IGNORE
	resized.connect(func(): _ctrl_item_container.size = size)
	add_child(_ctrl_item_container)

	_grid_drop_zone_ui = GridDropZoneUI.new()
	_grid_drop_zone_ui.dragable_dropped.connect(_on_dragable_dropped)
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

	_queue_refresh()


func _notification(what: int) -> void:
	if what == NOTIFICATION_DRAG_END:
		_grid_drop_zone_ui.deactivate()


func _connect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return

	if !inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.connect(_queue_refresh)
	#if !inventory.stack_added.is_connected(_on_stack_added):
		#inventory.stack_added.connect(_on_stack_added)
	if !inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.connect(_on_inventory_resized)


func _disconnect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return

	if inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.disconnect(_queue_refresh)
	#if inventory.stack_added.is_connected(_on_stack_added):
		#inventory.stack_added.disconnect(_on_stack_added)
	if inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.disconnect(_on_inventory_resized)


func _on_inventory_resized() -> void:
	_queue_refresh()


func _on_stack_added(stack_index: int) -> void:
	_queue_refresh()


func _process(_delta) -> void:
	if _refresh_queued:
		_refresh()
		_refresh_queued = false


func _queue_refresh() -> void:
	_refresh_queued = true


func _refresh() -> void:
	_grid_drop_zone_ui.deactivate()
	custom_minimum_size = _get_inventory_size_pixels()
	size = custom_minimum_size

	_clear_list()
	_populate_list()


func _get_inventory_size_pixels() -> Vector2:
	if !is_instance_valid(inventory):
		return Vector2.ZERO

	var result := Vector2(inventory.size.x * field_dimensions.x, \
		inventory.size.y * field_dimensions.y)

	# Also take item spacing into consideration
	result += Vector2(inventory.size - Vector2i.ONE) * item_spacing

	return result


func _clear_list() -> void:
	if !is_instance_valid(_ctrl_item_container):
		return

	for ctrl_inventory_item in _ctrl_item_container.get_children():
		_ctrl_item_container.remove_child(ctrl_inventory_item)
		ctrl_inventory_item.queue_free()


func _populate_list() -> void:
	if !is_instance_valid(inventory) || !is_instance_valid(_ctrl_item_container):
		return
	
	for stack in inventory.stacks:
		var grid_item_stack_ui : GridItemStackUI = grid_item_stack_ui_scene.instantiate()
		grid_item_stack_ui.size = _get_item_sprite_size(stack)
		grid_item_stack_ui.setup(inventory, stack)
		grid_item_stack_ui.grabbed.connect(_on_item_grab.bind(grid_item_stack_ui))
		grid_item_stack_ui.dropped.connect(_on_item_drop.bind(grid_item_stack_ui))
		grid_item_stack_ui.activated.connect(_on_item_activated.bind(grid_item_stack_ui))
		grid_item_stack_ui.context_activated.connect(_on_item_context_activated.bind(grid_item_stack_ui))
		grid_item_stack_ui.mouse_entered.connect(_on_item_mouse_entered.bind(grid_item_stack_ui))
		grid_item_stack_ui.mouse_exited.connect(_on_item_mouse_exited.bind(grid_item_stack_ui))
		grid_item_stack_ui.clicked.connect(_on_item_clicked.bind(grid_item_stack_ui))
		grid_item_stack_ui.middle_clicked.connect(_on_item_middle_clicked.bind(grid_item_stack_ui))
		grid_item_stack_ui.position = _get_field_position(inventory.get_stack_position(stack))

		_ctrl_item_container.add_child(grid_item_stack_ui)


func _on_item_grab(offset: Vector2, grid_item_stack_ui: GridItemStackUI) -> void:
	_clear_selection()


func _on_item_drop(zone: GridDropZoneUI, drop_position: Vector2, grid_item_stack_ui: GridItemStackUI) -> void:
	var stack: ItemStack = grid_item_stack_ui.item
	# The item might have been freed in case the item stack has been moved and merged with another
	# stack.
	if is_instance_valid(stack) and inventory.has_stack(stack):
		if zone == null:
			item_dropped.emit(stack, drop_position + grid_item_stack_ui.position)


func _get_item_sprite_size(stack: ItemStack) -> Vector2:
	var item_size: Vector2i = inventory.get_stack_size(stack)
	var sprite_size := Vector2(item_size) * field_dimensions
	# Also take item spacing into consideration
	sprite_size += (Vector2(item_size) - Vector2.ONE) * item_spacing
	return sprite_size


func _on_item_activated(grid_item_stack_ui: GridItemStackUI) -> void:
	var stack = grid_item_stack_ui.stack
	if !stack:
		return

	inventory_item_activated.emit(stack)


func _on_item_context_activated(event: InputEvent, grid_item_stack_ui: GridItemStackUI) -> void:
	var stack = grid_item_stack_ui.stack
	if !stack:
		return

	inventory_stack_context_activated.emit(event, stack)


func _on_item_mouse_entered(grid_item_stack_ui) -> void:
	item_mouse_entered.emit(grid_item_stack_ui.stack)


func _on_item_mouse_exited(grid_item_stack_ui) -> void:
	item_mouse_exited.emit(grid_item_stack_ui.stack)


func _on_item_clicked(grid_item_stack_ui) -> void:
	var stack = grid_item_stack_ui.stack
	if !is_instance_valid(stack):
		return

	if select_mode == SelectMode.SELECT_MULTI && Input.is_key_pressed(KEY_CTRL):
		if !_is_item_selected(stack):
			_select(stack)
		else:
			_deselect(stack)
	else:
		_clear_selection()
		_select(stack)


func _on_item_middle_clicked(grid_item_stack_ui) -> void:
	var stack = grid_item_stack_ui.stack
	if !is_instance_valid(stack):
		return
	
	var stack_size : int = stack.amount
	var stack_index = inventory.stacks.find(stack)

	# All this floor/float jazz just to do integer division without warnings
	var new_stack_size: int = floor(float(stack_size) / 2)
	request_split.emit(inventory, stack_index, new_stack_size)


func _select(stack: ItemStack) -> void:
	if stack in _selected_items:
		return

	if (stack != null) && !inventory.has_stack(stack):
		return

	_selected_items.append(stack)
	selection_changed.emit()


func _is_item_selected(stack: ItemStack) -> bool:
	return stack in _selected_items


func _deselect(stack: ItemStack) -> void:
	if !(stack in _selected_items):
		return
	var idx := _selected_items.find(stack)
	if idx < 0:
		return
	_selected_items.remove_at(idx)
	selection_changed.emit()


func _clear_selection() -> void:
	if _selected_items.is_empty():
		return
	_selected_items.clear()
	selection_changed.emit()


func _on_dragable_dropped(dragable: GridDraggableElementUI, drop_position: Vector2) -> void:
	var stack: ItemStack = dragable.stack
	if stack == null:
		return

	if !is_instance_valid(inventory):
		return

	_handle_stack_transfer(stack, drop_position, dragable.inventory)


func _handle_stack_transfer(stack: ItemStack, drop_position: Vector2, source_inventory : Inventory) -> void:
	var field_coords = get_field_coords(drop_position + (field_dimensions / 2))
	
	if source_inventory == null:
		printerr("source_inventory is null?")
		#inventory.add_at_position(stack, field_coords)
		return
	
	if source_inventory.database != inventory.database:
		return
		
	var stack_position : Vector2i = source_inventory.get_stack_position(stack)
	#TODO make rotation with R key or mouse wheel
	var is_rotated: bool = source_inventory.is_stack_rotated(stack)
	request_transfer_to.emit(source_inventory, stack_position, inventory, field_coords, stack.amount, is_rotated)


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
	if _selected_items.is_empty():
		return null
	return _selected_items[0]


func get_selected_inventory_items() -> Array[ItemStack]:
	return _selected_items.duplicate()


func _get_field_position(field_coords: Vector2i) -> Vector2:
	var field_position = Vector2(field_coords.x * field_dimensions.x, \
		field_coords.y * field_dimensions.y)
	field_position += Vector2(item_spacing * field_coords)
	return field_position


func deselect_inventory_item() -> void:
	_clear_selection()


func select_inventory_item(stack: ItemStack) -> void:
	_select(stack)


func get_item_rect(stack: ItemStack) -> Rect2:
	if !is_instance_valid(stack):
		return Rect2()
	return Rect2(
		_get_field_position(inventory.get_stack_position(stack)),
		_get_item_sprite_size(stack)
	)
