extends GridDraggableElementUI
class_name GridItemStackUI

signal activated
signal clicked
signal middle_clicked
signal context_activated

@export var stack_style : StyleBox

var inventory : GridInventory

func _init(inventory : GridInventory):
	self.inventory = inventory

var stack: ItemStack:
	set(new_stack):
		if stack == new_stack:
			return

		_disconnect_item_signals()
		_connect_item_signals(new_stack)

		stack = new_stack
		if stack and inventory != null:
			var definition: ItemDefinition = inventory.database.get_item(stack.item_id)
			texture = definition.icon
			activate()
		else:
			texture = null
			deactivate()
		_update_stack_size()

var texture: Texture2D:
	set(new_texture):
		if new_texture == texture:
			return
		texture = new_texture
		_update_texture()


var stretch_mode: TextureRect.StretchMode = TextureRect.StretchMode.STRETCH_SCALE:
	set(new_stretch_mode):
		if stretch_mode == new_stretch_mode:
			return
		stretch_mode = new_stretch_mode
		if is_instance_valid(_texture_rect):
			_texture_rect.stretch_mode = stretch_mode

var _texture_bg: Panel
var _texture_rect: TextureRect
var _stack_size_label: Label
static var _stored_preview_size: Vector2
static var _stored_preview_offset: Vector2


func _connect_item_signals(new_item: ItemStack) -> void:
	if new_item == null:
		return
	
	if !new_item.updated.is_connected(_refresh):
		new_item.updated.connect(_refresh)


func _disconnect_item_signals() -> void:
	if !is_instance_valid(stack):
		return
	
	if stack.updated.is_connected(_refresh):
		stack.updated.disconnect(_refresh)


func _ready() -> void:
	_texture_bg = Panel.new()
	_texture_bg.mouse_filter = Control.MOUSE_FILTER_IGNORE
	_texture_bg.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	_texture_bg.size = size
	remove_theme_stylebox_override("panel")
	if stack_style != null:
		_texture_bg.add_theme_stylebox_override("panel", stack_style)
	
	_texture_rect = TextureRect.new()
	_texture_rect.mouse_filter = Control.MOUSE_FILTER_IGNORE
	_texture_rect.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	_texture_rect.stretch_mode = stretch_mode
	
	_stack_size_label = Label.new()
	_stack_size_label.mouse_filter = Control.MOUSE_FILTER_IGNORE
	_stack_size_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_RIGHT
	_stack_size_label.vertical_alignment = VERTICAL_ALIGNMENT_BOTTOM
	
	add_child(_texture_bg)
	add_child(_texture_rect)
	add_child(_stack_size_label)
	

	resized.connect(func():
		_texture_bg.size = size
		_texture_rect.size = size
		_stack_size_label.size = size
	)
	grabbed.connect(func(_offset):
		if _texture_rect:
			_texture_rect.hide()
		if _stack_size_label:
			_stack_size_label.hide()
	)

	if stack == null:
		deactivate()

	_refresh()


func _notification(what) -> void:
	if what == NOTIFICATION_DRAG_END:
		if _texture_rect:
			_texture_rect.show()
		if _stack_size_label:
			_stack_size_label.show()


func _update_texture() -> void:
	if !is_instance_valid(_texture_rect):
		return
	_texture_rect.texture = texture
	#TODO rotation
	if is_instance_valid(stack):
		_texture_rect.size = Vector2(size.x, size.y)
		if inventory.is_stack_rotation_positive(stack):
			_texture_rect.position = Vector2(_texture_rect.size.y, 0)
			_texture_rect.rotation = 0
		else:
			_texture_rect.position = Vector2(0, 0)
			_texture_rect.rotation = 0
	else:
		_texture_rect.size = size
		_texture_rect.position = Vector2.ZERO
		_texture_rect.rotation = 0


func _update_stack_size() -> void:
	if !is_instance_valid(_stack_size_label):
		return
	if !is_instance_valid(stack):
		_stack_size_label.text = ""
		return
	var stack_size: int = stack.amount
	if stack_size <= 1:
		_stack_size_label.text = ""
	else:
		_stack_size_label.text = "%d" % stack_size
	_stack_size_label.size = size


func _refresh() -> void:
	_update_texture()
	_update_stack_size()


func create_preview() -> Control:
	var preview = GridItemStackUI.new(inventory)
	preview.stack = stack
	preview.texture = texture
	preview.size = size
	preview.stretch_mode = stretch_mode
	return preview


func _gui_input(event: InputEvent) -> void:
	if !(event is InputEventMouseButton):
		return

	var mb_event: InputEventMouseButton = event
	if !mb_event.pressed:
		return
	if mb_event.button_index == MOUSE_BUTTON_LEFT:
		if mb_event.double_click:
			activated.emit()
		else:
			clicked.emit()
	if mb_event.button_index == MOUSE_BUTTON_MIDDLE:
		middle_clicked.emit()
	elif mb_event.button_index == MOUSE_BUTTON_MASK_RIGHT:
		context_activated.emit()
