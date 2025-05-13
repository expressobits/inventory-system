@tool
extends ItemList
class_name SimpleInventoryUI
## Displays a list of items in the inventory.

## Emitted when an [InventoryItem] is activated (i.e. double clicked).
signal inventory_item_activated(stack)
## Emitted when the context menu of an [InventoryItem] is activated (i.e. right clicked).
signal inventory_item_context_activated(stack)

@export var inventory: Inventory:
	set(new_inventory):
		if new_inventory == inventory:
			return
		_disconnect_inventory_signals()
		inventory = new_inventory
		_connect_inventory_signals()
		_queue_refresh()
		update_configuration_warnings()

var _refresh_queued: bool = false


func _get_configuration_warnings() -> PackedStringArray:
	if inventory == null:
		return PackedStringArray([
				"This node is not linked to an inventory, so it can't display any content.\n" + \
				"Set the inventory_path property to point to an Inventory node."])
	return PackedStringArray()
	
	
func _ready():
	item_activated.connect(_on_list_item_activated)
	item_clicked.connect(_on_list_item_clicked)
	_queue_refresh()


func _connect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if !inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.connect(_queue_refresh)
	if !inventory.updated_stack.is_connected(_on_stack_updated):
		inventory.updated_stack.connect(_on_stack_updated)


func _disconnect_inventory_signals() -> void:
	if !is_instance_valid(inventory):
		return
	if inventory.contents_changed.is_connected(_queue_refresh):
		inventory.contents_changed.disconnect(_queue_refresh)
	if inventory.updated_stack.is_connected(_on_stack_updated):
		inventory.updated_stack.disconnect(_on_stack_updated)


func _on_list_item_activated(index: int) -> void:
	inventory_item_activated.emit(_get_inventory_item(index))


func _on_list_item_clicked(index: int, at_position: Vector2, mouse_button_index: int) -> void:
	if mouse_button_index == MOUSE_BUTTON_RIGHT:
		inventory_item_context_activated.emit(_get_inventory_item(index))


func _on_stack_updated(stack_index : int) -> void:
	_queue_refresh()


func _process(_delta) -> void:
	if _refresh_queued:
		_refresh()
		_refresh_queued = false


func _queue_refresh() -> void:
	_refresh_queued = true


func _refresh() -> void:
	if is_inside_tree():
		clear()
		_populate_list()


func _populate_list() -> void:
	if !is_instance_valid(inventory):
		return

	for i in inventory.stacks.size():
		var item = inventory.stacks[i]
		var definition : ItemDefinition = inventory.database.get_item(item.item_id)
		var texture := definition.icon
		add_item(_get_item_title(item, definition), texture)
		set_item_metadata(get_item_count() - 1, i)


func _get_item_title(item : ItemStack, definition : ItemDefinition) -> String:
	if item == null:
		return ""

	var title = definition.name
	var stack_size : int = item.amount
	if stack_size > 1:
		title = "%s (x%d)" % [title, stack_size]

	return title
	

## Returns the currently selected item. In case multiple items are selected,
## the first one is returned.
func get_selected_inventory_item() -> int:
	if get_selected_items().is_empty():
		return -1

	return _get_inventory_item(get_selected_items()[0])


## Returns all the currently selected items.
func get_selected_inventory_items() -> Array[int]:
	var result: Array[int]
	var indexes = get_selected_items()
	for i in indexes:
		result.append(_get_inventory_item(i))
	return result


func _get_inventory_item(index: int) -> int:
	assert(index >= 0)
	assert(index < get_item_count())

	return get_item_metadata(index)


## Deselects the selected item.
func deselect_inventory_item() -> void:
	pass
	deselect_all()


## Selects the given item.
func select_inventory_item(item_index : int) -> void:
	deselect_all()
	for index in item_count:
		if get_item_metadata(index) != item_index:
			continue
		select(index)
		return
