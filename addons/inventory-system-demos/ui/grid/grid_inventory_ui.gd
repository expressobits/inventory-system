extends Control
class_name GridInventoryUI

#region Signals

## Emitted when a grabbed [ItemStack] is dropped.
signal item_dropped(item: ItemStack, offset)
## Emitted when the selection has changed. Use [method get_selected_inventory_item]
## to obtain the currently selected item.
signal selection_changed
## Emitted when an [InventoryItem] is activated (i.e. double clicked).
signal inventory_item_activated(item)
## Emitted when the context menu of an [ItemStack] is activated
## (i.e. right clicked).
signal inventory_stack_context_activated(event: InputEvent,  inventory: Inventory, item: ItemStack)
## Emitted when the mouse enters the [Rect2] area of the control representing
## the given [InventoryItem].
signal item_mouse_entered(item)
## Emitted when the mouse leaves the [Rect2] area of the control representing
## the given [InventoryItem].
signal item_mouse_exited(item)

signal request_split(inventory : Inventory, stack_index : int, amount : int)
signal request_transfer_to(origin_inventory: GridInventory, origin_position: Vector2i, inventory: GridInventory, destination_position: Vector2i, amount: int, is_rotated: bool)
signal request_fast_transfer(origin_inventory: GridInventory, origin_position: Vector2i, amount: int)

#endregion

enum SelectMode {SELECT_SINGLE = 0, SELECT_MULTI = 1}

#region Variables
@export var grid_slot_ui_scene: PackedScene
@export var grid_item_stack_ui_scene: PackedScene

## Path to an [Inventory] node.
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

## The size of each inventory gird slot in pixels.
@export var grid_slot_size: Vector2 = Vector2(32, 32)

## The spacing between items in pixels.
@export var item_spacing: int = 0

## Single or multi select mode (hold CTRL to select multiple items).
@export_enum("Single", "Multi") var select_mode: int = SelectMode.SELECT_SINGLE:
	set(new_select_mode):
		if select_mode == new_select_mode:
			return
		select_mode = new_select_mode
		_clear_selection()


@export_group("Custom Styles")
## Style of a rectangle that will be drawn on top of the selected item.
@export var selection_style: StyleBox:
	set(new_selection_style):
		selection_style = new_selection_style
		_queue_refresh()


## The [Inventory] node linked to this control.
var inventory: GridInventory = null:
	set(new_inventory):
		
		if inventory == new_inventory:
			return
		
		_clear_selection()
		_disconnect_inventory_signals()
		inventory = new_inventory
		_connect_inventory_signals()

		_queue_refresh()
		_update_stacks()

var _grid_inventory_content_ui: Control = null
var _grid_slots_container: Control = null
var _grid_slots: Array = []
var _refresh_queued: bool = false
var _stack_uis: Array = []
var _selected_stacks: Array[GridItemStackUI] = []
var _ctrl_item_container: Control = null
var _grid_drop_zone_ui: GridDropZoneUI = null
#endregion


#region Default Functions
func _ready() -> void:
	if Engine.is_editor_hint():
		# Clean up, in case it is duplicated in the editor
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.queue_free()
			_grid_slots_container.queue_free()

	if has_node(inventory_path):
		inventory = get_node_or_null(inventory_path)
		
	_grid_slots_container = Control.new()
	_grid_slots_container.name = "GridSlotsContainer"
	add_child(_grid_slots_container)

	_grid_inventory_content_ui = Control.new()
	_grid_inventory_content_ui.name = "GridInventoryContentUI"
	_grid_inventory_content_ui.resized.connect(_update_size)
	_grid_inventory_content_ui.mouse_filter = Control.MOUSE_FILTER_IGNORE
	add_child(_grid_inventory_content_ui)
	
	_ctrl_item_container = Control.new()
	_ctrl_item_container.size = size
	_ctrl_item_container.mouse_filter = Control.MOUSE_FILTER_IGNORE
	_grid_inventory_content_ui.resized.connect(func(): _ctrl_item_container.size = size)
	_grid_inventory_content_ui.add_child(_ctrl_item_container)

	_grid_drop_zone_ui = GridDropZoneUI.new()
	_grid_drop_zone_ui.dragable_dropped.connect(_on_dragable_dropped)
	_grid_drop_zone_ui.size = size
	resized.connect(func(): _grid_drop_zone_ui.size = size)
	GridDraggableElementUI.dragable_grabbed.connect(func(_dragable: GridDraggableElementUI, _grab_position: Vector2):
		_grid_drop_zone_ui.activate()
	)
	GridDraggableElementUI.dragable_dropped.connect(func(_dragable: GridDraggableElementUI, _zone: GridDropZoneUI, _drop_position: Vector2):
		_grid_drop_zone_ui.deactivate()
	)
	_grid_inventory_content_ui.add_child(_grid_drop_zone_ui)

	_update_size()
	_queue_refresh()
	
func _process(_delta) -> void:
	if _refresh_queued:
		_refresh()
		_refresh_queued = false


func _get_configuration_warnings() -> PackedStringArray:
	if inventory_path.is_empty():
		return PackedStringArray([
				"This node is not linked to an inventory and it can't display any content.\n" + \
				"Set the inventory_path property to point to an InventoryGrid node."])
	return PackedStringArray()
	

func _notification(what: int) -> void:
	if what == NOTIFICATION_DRAG_END:
		_grid_drop_zone_ui.deactivate()
		


func _input(event) -> void:
	if !(event is InputEventMouseMotion):
		return
	if !is_instance_valid(inventory):
		return
	_highlight_grabbed_item()
#endregion


func _connect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if !inventory.stack_added.is_connected(_on_inventory_stack_added):
		inventory.stack_added.connect(_on_inventory_stack_added)
	if !inventory.stack_removed.is_connected(_on_inventory_stack_removed):
		inventory.stack_removed.connect(_on_inventory_stack_removed)
	if !inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.connect(_on_inventory_resized)


func _disconnect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if inventory.stack_added.is_connected(_on_inventory_stack_added):
		inventory.stack_added.disconnect(_on_inventory_stack_added)
	if inventory.stack_removed.is_connected(_on_inventory_stack_removed):
		inventory.stack_removed.disconnect(_on_inventory_stack_removed)
	if inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.disconnect(_on_inventory_resized)


func _on_stack_added(_stack_index : int):
	_queue_refresh()


func _update_stacks():
	for stack_ui in _stack_uis:
		stack_ui.queue_free()
	_stack_uis.clear()
	for stack_index in inventory.stacks.size():
		_on_inventory_stack_added(stack_index)
	

func _on_inventory_stack_added(stack_index: int):
	var stack = inventory.stacks[stack_index]
	add_grid_item_stack_ui(stack)


func _on_inventory_stack_removed(stack_index: int):
	var _stack_ui = _stack_uis[stack_index]
	_stack_ui.queue_free()
	_stack_uis.remove_at(stack_index)


func _refresh() -> void:
	_refresh_grid_slots()
	size = custom_minimum_size
	
	if is_instance_valid(_grid_drop_zone_ui):
		_grid_drop_zone_ui.deactivate()
	if is_instance_valid(_grid_inventory_content_ui):
		_grid_inventory_content_ui.custom_minimum_size = _get_inventory_size_pixels()


func _queue_refresh() -> void:
	_refresh_queued = true
		

#region Grid Slots
func get_grid_slot_coords(local_pos: Vector2) -> Vector2i:
	# We have to consider the item spacing when calculating grid slot coordinates, thus we expand the
	# size of each field by Vector2(item_spacing, item_spacing).
	var grid_slot_dimensions_ex = grid_slot_size + Vector2(item_spacing, item_spacing)

	# We also don't want the item spacing to disturb snapping to the closest field, so we add half
	# the spacing to the local coordinates.
	var local_pos_ex = local_pos + (Vector2(item_spacing, item_spacing) / 2)

	var x: int = local_pos_ex.x / (grid_slot_dimensions_ex.x)
	var y: int = local_pos_ex.y / (grid_slot_dimensions_ex.y)
	return Vector2i(x, y)


func _refresh_grid_slots() -> void:
	if is_instance_valid(_grid_slots_container):
		while _grid_slots_container.get_child_count() > 0:
			_grid_slots_container.get_children()[0].queue_free()
			_grid_slots_container.remove_child(_grid_slots_container.get_children()[0])
	_grid_slots = []

	if !is_instance_valid(inventory):
		return
	
	for i in range(inventory.size.x):
		_grid_slots.append([])
		for j in range(inventory.size.y):
			var grid_slot_ui: GridSlotUI = grid_slot_ui_scene.instantiate()
			grid_slot_ui.size = grid_slot_size
			grid_slot_ui.position = _get_grid_slot_position(Vector2i(i, j))
			grid_slot_ui.setup(Vector2i(i, j), inventory)
			_grid_slots_container.add_child(grid_slot_ui)
			_grid_slots[i].append(grid_slot_ui)
			
			
func _set_selected_grid_slots(rect: Rect2i, selected: bool) -> void:
	var h_range = min(rect.size.x + rect.position.x, inventory.size.x)
	for i in range(rect.position.x, h_range):
		var v_range = min(rect.size.y + rect.position.y, inventory.size.y)
		for j in range(rect.position.y, v_range):
			if selected:
				_grid_slots[i][j].select()
			else:
				_grid_slots[i][j].unselect()


func _set_dragged_grid_slots(rect: Rect2i, dragged: bool) -> void:
	var h_range = min(rect.size.x + rect.position.x, inventory.size.x)
	for i in range(rect.position.x, h_range):
		var v_range = min(rect.size.y + rect.position.y, inventory.size.y)
		for j in range(rect.position.y, v_range):
			_grid_slots[i][j].item_dragged = dragged


func _fill_background(style: StyleBox, priority: int) -> void:
	for panel in _grid_slots_container.get_children():
		panel.set_style(style, priority)
#endregion


func _update_size() -> void:
	custom_minimum_size = _grid_inventory_content_ui.size
	size = _grid_inventory_content_ui.size


func _on_inventory_resized() -> void:
	_queue_refresh()
	_refresh_grid_slots()


func _highlight_grabbed_item():
	var grabbed_item: ItemStack = _get_global_grabbed_item()
	if !grabbed_item:
		return

	var global_grabbed_item_pos: Vector2 = _get_global_grabbed_item_local_pos()
	if !_is_hovering(global_grabbed_item_pos):
		#_set_dragged_grid_slots(rect, false)
		return

	#_set_dragged_grid_slots(rect, false)

	var grabbed_item_coords := get_grid_slot_coords(global_grabbed_item_pos + (grid_slot_size / 2))
	var definition : ItemDefinition = inventory.database.get_item(grabbed_item.item_id)
	var item_size := definition.size
	var rect := Rect2i(grabbed_item_coords, item_size)
	if !Rect2i(Vector2i.ZERO, inventory.size).encloses(rect):
		return
	#_set_dragged_grid_slots(rect, true)


func _is_hovering(local_pos: Vector2) -> bool:
	return Rect2(Vector2.ZERO, size).has_point(local_pos)


func _get_global_grabbed_item() -> ItemStack:
	if GridDraggableElementUI.get_grabbed_dragable() == null:
		return null
	return (GridDraggableElementUI.get_grabbed_dragable() as GridItemStackUI).stack


func _get_global_grabbed_item_local_pos() -> Vector2:
	if GridDraggableElementUI.get_grabbed_dragable():
		return get_local_mouse_position() - GridDraggableElementUI.get_grab_offset_local_to(self)
	return Vector2(-1, -1)


## Returns the currently selected item. In case multiple items are selected,
## the first one is returned.
func get_selected_inventory_item() -> GridItemStackUI:
	if !is_instance_valid(_grid_inventory_content_ui):
		return null
	return _selected_stacks[0]


## Returns all the currently selected items.
func get_selected_stacks() -> Array[GridItemStackUI]:
	return _selected_stacks.duplicate()

	
#region ContentUI
func _on_item_drop(zone: GridDropZoneUI, drop_position: Vector2, grid_item_stack_ui: GridItemStackUI) -> void:
	var stack: ItemStack = grid_item_stack_ui.item
	# The item might have been freed in case the item stack has been moved and merged with another
	# stack.
	if is_instance_valid(stack) and inventory.has_stack(stack):
		if zone == null:
			item_dropped.emit(stack, drop_position + grid_item_stack_ui.position)


func add_grid_item_stack_ui(stack: ItemStack):
	var grid_item_stack_ui : GridItemStackUI = grid_item_stack_ui_scene.instantiate()
	grid_item_stack_ui.size = _get_stack_sprite_size(stack)
	grid_item_stack_ui.setup(inventory, stack)
	grid_item_stack_ui.grabbed.connect(_on_item_grab.bind(grid_item_stack_ui))
	#grid_item_stack_ui.dropped.connect(_on_item_drop.bind(grid_item_stack_ui))
	grid_item_stack_ui.activated.connect(_on_item_activated.bind(grid_item_stack_ui))
	grid_item_stack_ui.context_activated.connect(_on_item_context_activated.bind(grid_item_stack_ui))
	grid_item_stack_ui.mouse_entered.connect(func():
		item_mouse_entered.emit(grid_item_stack_ui.stack)
	)
	grid_item_stack_ui.mouse_exited.connect(func():
		item_mouse_exited.emit(grid_item_stack_ui.stack)
	)
	grid_item_stack_ui.clicked.connect(_on_item_clicked.bind(grid_item_stack_ui))
	grid_item_stack_ui.middle_clicked.connect(_on_item_middle_clicked.bind(grid_item_stack_ui))
	grid_item_stack_ui.position = _get_grid_slot_position(inventory.get_stack_position(stack))

	_ctrl_item_container.add_child(grid_item_stack_ui)
	_stack_uis.append(grid_item_stack_ui)
	
		
func _on_item_middle_clicked(grid_item_stack_ui) -> void:
	var stack = grid_item_stack_ui.stack
	if !is_instance_valid(stack):
		return
	
	var stack_size : int = stack.amount
	var stack_index = inventory.stacks.find(stack)

	# All this floor/float jazz just to do integer division without warnings
	var new_stack_size: int = floor(float(stack_size) / 2)
	request_split.emit(inventory, stack_index, new_stack_size)


func _on_item_clicked(grid_item_stack_ui) -> void:
	var stack = grid_item_stack_ui.stack
	if !is_instance_valid(stack):
			return
	
	if Input.is_action_pressed("ui_inventory_transfer"):
		var stack_position : Vector2i = inventory.get_stack_position(stack)
		#TODO make rotation with R key or mouse wheel
		var _is_rotated: bool = inventory.is_stack_rotated(stack)
		
		request_fast_transfer.emit(inventory, stack_position, stack.amount)
	else:
		
		if select_mode == SelectMode.SELECT_MULTI && Input.is_key_pressed(KEY_CTRL):
			if !_is_item_selected(stack):
				_select(grid_item_stack_ui)
			else:
				_deselect(grid_item_stack_ui)
		else:
			_clear_selection()
			_select(grid_item_stack_ui)


func _on_item_grab(_offset: Vector2, _grid_item_stack_ui: GridItemStackUI) -> void:
	_clear_selection()


func _on_item_context_activated(event: InputEvent, grid_item_stack_ui: GridItemStackUI) -> void:
	var stack = grid_item_stack_ui.stack
	if !stack:
		return

	inventory_stack_context_activated.emit(event, inventory, stack)


func _on_dragable_dropped(dragable: GridDraggableElementUI, drop_position: Vector2) -> void:
	var stack: ItemStack = dragable.stack
	if stack == null:
		return

	if !is_instance_valid(inventory):
		return

	_handle_stack_transfer(stack, drop_position, dragable.inventory)


func _handle_stack_transfer(stack: ItemStack, drop_position: Vector2, source_inventory : Inventory) -> void:
	var grid_slot_coords = get_grid_slot_coords(drop_position + (grid_slot_size / 2))
	
	if source_inventory == null:
		printerr("source_inventory is null?")
		return
	
	if source_inventory.database != inventory.database:
		return
		
	var stack_position : Vector2i = source_inventory.get_stack_position(stack)
	#TODO make rotation with R key or mouse wheel
	var is_rotated: bool = source_inventory.is_stack_rotated(stack)
	request_transfer_to.emit(source_inventory, stack_position, inventory, grid_slot_coords, stack.amount, is_rotated)


#region Stack UI Utils
func get_stack_rect(stack: ItemStack) -> Rect2:
	if !is_instance_valid(stack):
		return Rect2()
	return Rect2(
		_get_grid_slot_position(inventory.get_stack_position(stack)),
		_get_stack_sprite_size(stack)
	)


func _get_stack_sprite_size(stack: ItemStack) -> Vector2:
	var item_size: Vector2i = inventory.get_stack_size(stack)
	var sprite_size := Vector2(item_size) * grid_slot_size
	# Also take item spacing into consideration
	sprite_size += (Vector2(item_size) - Vector2.ONE) * item_spacing
	return sprite_size


func _get_grid_slot_position(grid_slot_coords: Vector2i) -> Vector2:
	var grid_slot_position = Vector2(grid_slot_coords.x * grid_slot_size.x, \
		grid_slot_coords.y * grid_slot_size.y)
	grid_slot_position += Vector2(item_spacing * grid_slot_coords)
	return grid_slot_position


func _get_inventory_size_pixels() -> Vector2:
	if !is_instance_valid(inventory):
		return Vector2.ZERO

	var result := Vector2(inventory.size.x * grid_slot_size.x, \
		inventory.size.y * grid_slot_size.y)

	# Also take item spacing into consideration
	result += Vector2(inventory.size - Vector2i.ONE) * item_spacing

	return result
#endregion

#region Selected Stacks
func _on_selection_changed() -> void:
	_handle_selection_change()
	selection_changed.emit()


func _handle_selection_change() -> void:
	if !is_instance_valid(inventory):
		return

	for stack_ui in _stack_uis:
		if stack_ui:
			_set_selected_grid_slots(inventory.get_stack_rect(stack_ui.stack), stack_ui in get_selected_stacks())


func _is_item_selected(stack: ItemStack) -> bool:
	return stack in _selected_stacks


func _select(stack_ui: GridItemStackUI) -> void:
	var stack = stack_ui.stack
	if stack in _selected_stacks:
		return

	if stack != null && !inventory.has_stack(stack):
		return

	_selected_stacks.append(stack_ui)
	_on_selection_changed()
	stack_ui.select()
	
	
func _deselect(stack_ui: GridItemStackUI) -> void:
	var stack = stack_ui.stack
	if !(stack in _selected_stacks):
		return
	var idx := _selected_stacks.find(stack_ui)
	if idx < 0:
		return
	_selected_stacks.remove_at(idx)
	_on_selection_changed()
	stack_ui.unselect()


func _clear_selection() -> void:
	if _selected_stacks.is_empty():
		return
	for selected_stack in _selected_stacks:
		if is_instance_valid(selected_stack):
			selected_stack.unselect()
	_selected_stacks.clear()
	selection_changed.emit()
	
	
func _on_item_activated(grid_item_stack_ui: GridItemStackUI) -> void:
	var stack = grid_item_stack_ui.stack
	if !stack:
		return

	inventory_item_activated.emit(stack)
#endregion
#endregion
