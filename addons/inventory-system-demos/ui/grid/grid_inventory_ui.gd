extends Control
class_name GridInventoryUI

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

enum SelectMode {SELECT_SINGLE = 0, SELECT_MULTI = 1}

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
		
		
## The default texture that will be used for items with no [code]image[/code]
## property.
@export var default_item_texture: Texture2D:
	set(new_default_item_texture):
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.default_item_texture = new_default_item_texture
		default_item_texture = new_default_item_texture
		
		
## The size of each inventory field in pixels.
@export var field_dimensions: Vector2 = Vector2(32, 32):
	set(new_field_dimensions):
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.field_dimensions = new_field_dimensions
		field_dimensions = new_field_dimensions


## The spacing between items in pixels.
@export var item_spacing: int = 0:
	set(new_item_spacing):
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.item_spacing = new_item_spacing
		item_spacing = new_item_spacing


## Single or multi select mode (hold CTRL to select multiple items).
@export_enum("Single", "Multi") var select_mode: int = GridInventoryContentUI.SelectMode.SELECT_SINGLE:
	set(new_select_mode):
		if select_mode == new_select_mode:
			return
		select_mode = new_select_mode
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.select_mode = select_mode


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
		
		_disconnect_inventory_signals()
		inventory = new_inventory
		_connect_inventory_signals()

		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.inventory = inventory
		_queue_refresh()

var _grid_inventory_content_ui: GridInventoryContentUI = null
var _field_background_grid: Control = null
var _field_backgrounds: Array = []
var _selection_panels: Control = null
var _refresh_queued: bool = false


func _connect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if !inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.connect(_queue_refresh)
	if !inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.connect(_on_inventory_resized)


func _disconnect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.disconnect(_queue_refresh)
	if inventory.size_changed.is_connected(_on_inventory_resized):
		inventory.size_changed.disconnect(_on_inventory_resized)


func _on_stack_added(stack_index : int):
	_queue_refresh()


func _process(_delta) -> void:
	if _refresh_queued:
		_refresh()
		_refresh_queued = false


func _refresh() -> void:
	_refresh_field_background_grid()
	_refresh_selection_panel()


func _queue_refresh() -> void:
	_refresh_queued = true


func _refresh_selection_panel() -> void:
	if !is_instance_valid(_selection_panels):
		return

	for child in _selection_panels.get_children():
		child.queue_free()

	var selected_items := _grid_inventory_content_ui.get_selected_inventory_items()
	_selection_panels.visible = (!selected_items.is_empty()) && (selection_style != null)
	if selected_items.is_empty():
		return

	for selected_item in selected_items:
		var selection_panel := GridSelectionPanel.new()
		var rect := _grid_inventory_content_ui.get_item_rect(selected_item)
		selection_panel.position = rect.position
		selection_panel.size = rect.size
		selection_panel.set_style(selection_style)
		selection_panel.mouse_filter = Control.MOUSE_FILTER_IGNORE
		_selection_panels.add_child(selection_panel)


func _refresh_field_background_grid() -> void:
	if is_instance_valid(_field_background_grid):
		while _field_background_grid.get_child_count() > 0:
			_field_background_grid.get_children()[0].queue_free()
			_field_background_grid.remove_child(_field_background_grid.get_children()[0])
	_field_backgrounds = []

	if !is_instance_valid(inventory):
		return
	
	for i in range(inventory.size.x):
		_field_backgrounds.append([])
		for j in range(inventory.size.y):
			var field_panel: GridSlotUI = grid_slot_ui_scene.instantiate()
			field_panel.size = field_dimensions
			field_panel.position = _grid_inventory_content_ui._get_field_position(Vector2i(i, j))
			field_panel.setup(Vector2i(i, j), inventory)
			_field_background_grid.add_child(field_panel)
			_field_backgrounds[i].append(field_panel)


func _ready() -> void:
	if Engine.is_editor_hint():
		# Clean up, in case it is duplicated in the editor
		if is_instance_valid(_grid_inventory_content_ui):
			_grid_inventory_content_ui.queue_free()
			_field_background_grid.queue_free()

	if has_node(inventory_path):
		inventory = get_node_or_null(inventory_path)
		
	_field_background_grid = Control.new()
	_field_background_grid.name = "FieldBackgrounds"
	add_child(_field_background_grid)

	_grid_inventory_content_ui = GridInventoryContentUI.new()
	_grid_inventory_content_ui.grid_item_stack_ui_scene = grid_item_stack_ui_scene
	_grid_inventory_content_ui.inventory = inventory
	_grid_inventory_content_ui.field_dimensions = field_dimensions
	_grid_inventory_content_ui.item_spacing = item_spacing
	_grid_inventory_content_ui.default_item_texture = default_item_texture
	_grid_inventory_content_ui.name = "GridInventoryContentUI"
	_grid_inventory_content_ui.resized.connect(_update_size)
	_grid_inventory_content_ui.item_dropped.connect(func(item: ItemStack, drop_position: Vector2):
		item_dropped.emit(item, drop_position)
	)
	_grid_inventory_content_ui.inventory_item_activated.connect(func(item: ItemStack):
		inventory_item_activated.emit(item)
	)
	_grid_inventory_content_ui.inventory_stack_context_activated.connect(func(event: InputEvent, item: ItemStack):
		inventory_stack_context_activated.emit(event, inventory, item)
	)
	_grid_inventory_content_ui.request_split.connect(func(inventory: GridInventory, stack_index: int, amount : int):
		request_split.emit(inventory, stack_index, amount)
	)
	_grid_inventory_content_ui.request_transfer_to.connect(func(origin_inventory: GridInventory, origin_position: Vector2i, destination_inventory : GridInventory, destination_position: Vector2i, amount: int, is_rotated: bool):
		request_transfer_to.emit(origin_inventory, origin_position, destination_inventory, destination_position, amount, is_rotated)
	)
	_grid_inventory_content_ui.item_mouse_entered.connect(_on_item_mouse_entered)
	_grid_inventory_content_ui.item_mouse_exited.connect(_on_item_mouse_exited)
	_grid_inventory_content_ui.selection_changed.connect(_on_selection_changed)
	_grid_inventory_content_ui.select_mode = select_mode
	add_child(_grid_inventory_content_ui)

	_selection_panels = Control.new()
	_selection_panels.mouse_filter = Control.MOUSE_FILTER_IGNORE
	_selection_panels.name = "SelectionPanels"
	add_child(_selection_panels)

	#GridDraggableElementUI.dragable_dropped.connect(func(_grabbed_dragable, _zone, _local_drop_position):
		#_fill_background(field_style, GridSlotUI.StylePriority.LOW)
	#)

	_update_size()
	_queue_refresh()


func _notification(what: int) -> void:
	pass
	#if what == NOTIFICATION_DRAG_END:
		#_fill_background(field_style, GridSlotUI.StylePriority.LOW)


func _update_size() -> void:
	custom_minimum_size = _grid_inventory_content_ui.size
	size = _grid_inventory_content_ui.size


func _on_item_mouse_entered(item: ItemStack) -> void:
	item_mouse_entered.emit(item)


func _on_item_mouse_exited(item: ItemStack) -> void:
	item_mouse_exited.emit(item)


func _on_selection_changed() -> void:
	_handle_selection_change()
	selection_changed.emit()


func _handle_selection_change() -> void:
	if !is_instance_valid(inventory):
		return
	_refresh_selection_panel()

	for item in inventory.stacks:
		if item:
			_set_background_on_item_selected(inventory.get_stack_rect(item), item in _grid_inventory_content_ui.get_selected_inventory_items())


func _on_inventory_resized() -> void:
	_refresh_field_background_grid()


func _input(event) -> void:
	if !(event is InputEventMouseMotion):
		return
	if !is_instance_valid(inventory):
		return

		return
	_highlight_grabbed_item()


func _highlight_grabbed_item():
	var grabbed_item: ItemStack = _get_global_grabbed_item()
	if !grabbed_item:
		return

	var global_grabbed_item_pos: Vector2 = _get_global_grabbed_item_local_pos()
	if !_is_hovering(global_grabbed_item_pos):
		#_set_background_on_item_dragged(rect, false)
		return

	#_set_background_on_item_dragged(rect, false)

	var grabbed_item_coords := _grid_inventory_content_ui.get_field_coords(global_grabbed_item_pos + (field_dimensions / 2))
	var definition : ItemDefinition = inventory.database.get_item(grabbed_item.item_id)
	var item_size := definition.size
	var rect := Rect2i(grabbed_item_coords, item_size)
	if !Rect2i(Vector2i.ZERO, inventory.size).encloses(rect):
		return
	_set_background_on_item_dragged(rect, true)


func _is_hovering(local_pos: Vector2) -> bool:
	return Rect2(Vector2.ZERO, size).has_point(local_pos)


func _set_background_on_item_selected(rect: Rect2i, selected: bool) -> void:
	var h_range = min(rect.size.x + rect.position.x, inventory.size.x)
	for i in range(rect.position.x, h_range):
		var v_range = min(rect.size.y + rect.position.y, inventory.size.y)
		for j in range(rect.position.y, v_range):
			_field_backgrounds[i][j].item_selected = selected


func _set_background_on_item_dragged(rect: Rect2i, dragged: bool) -> void:
	var h_range = min(rect.size.x + rect.position.x, inventory.size.x)
	for i in range(rect.position.x, h_range):
		var v_range = min(rect.size.y + rect.position.y, inventory.size.y)
		for j in range(rect.position.y, v_range):
			_field_backgrounds[i][j].item_dragged = dragged


func _fill_background(style: StyleBox, priority: int) -> void:
	for panel in _field_background_grid.get_children():
		panel.set_style(style, priority)


func _get_global_grabbed_item() -> ItemStack:
	if GridDraggableElementUI.get_grabbed_dragable() == null:
		return null
	return (GridDraggableElementUI.get_grabbed_dragable() as GridItemStackUI).stack


func _get_global_grabbed_item_local_pos() -> Vector2:
	if GridDraggableElementUI.get_grabbed_dragable():
		return get_local_mouse_position() - GridDraggableElementUI.get_grab_offset_local_to(self)
	return Vector2(-1, -1)


## Deselects the selected item.
func deselect_inventory_item() -> void:
	if !is_instance_valid(_grid_inventory_content_ui):
		return
	_grid_inventory_content_ui.deselect_inventory_item()


## Selects the given item.
func select_inventory_item(item: ItemStack) -> void:
	if !is_instance_valid(_grid_inventory_content_ui):
		return
	_grid_inventory_content_ui.select_inventory_item(item)


## Returns the currently selected item. In case multiple items are selected,
## the first one is returned.
func get_selected_inventory_item() -> ItemStack:
	if !is_instance_valid(_grid_inventory_content_ui):
		return null
	return _grid_inventory_content_ui.get_selected_inventory_item()


## Returns all the currently selected items.
func get_selected_inventory_items() -> Array[ItemStack]:
	if !is_instance_valid(_grid_inventory_content_ui):
		return []
	return _grid_inventory_content_ui.get_selected_inventory_items()
