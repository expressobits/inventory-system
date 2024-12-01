extends GridDraggableElementUI
class_name GridItemStackUI

signal activated
signal clicked
signal middle_clicked
signal context_activated(event: InputEvent)

@export var stack_style: StyleBox
@export var hover_stack_style: StyleBox

@onready var texture_bg: Panel = $TextureBG
@onready var item_icon: TextureRect = %ItemIcon
@onready var stack_size_label: Label = $StackSizeLabel

var inventory : GridInventory
var stack: ItemStack

func setup(inventory : Inventory, stack : ItemStack):
	self.inventory = inventory
	self.stack = stack
	if stack and inventory != null:
		var definition: ItemDefinition = inventory.database.get_item(stack.item_id)
		tooltip_text = definition.name
		var is_rotated = inventory.is_stack_rotated(stack)
		var texture = definition.icon
		if is_rotated:
			var image = texture.get_image()
			image.rotate_90(CLOCKWISE)
			texture = ImageTexture.create_from_image(image)
		%ItemIcon.texture = texture
		
		activate()
		_disconnect_item_signals()
		_connect_item_signals(stack)
	else:
		%ItemIcon.texture = null
		deactivate()
	_update_stack_size()


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
	mouse_entered.connect(func():
		_set_panel_style(hover_stack_style)
	)
	mouse_exited.connect(func():
		_set_panel_style(stack_style)
	)
	grabbed.connect(func(_offset):
		visible = false
	)
	if stack == null:
		deactivate()
	
	_refresh()


func _notification(what) -> void:
	if what == NOTIFICATION_DRAG_END:
		visible = true


func _update_stack_size() -> void:
	if !is_instance_valid(stack_size_label):
		return
	if !is_instance_valid(stack):
		stack_size_label.text = ""
		return
	var stack_size: int = stack.amount
	if stack_size <= 1:
		stack_size_label.text = ""
	else:
		stack_size_label.text = "%d" % stack_size


func _refresh() -> void:
	_update_stack_size()


func create_preview() -> Control:
	var preview = self.duplicate()
	preview.setup(inventory, stack)
	preview.visible = true
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
		context_activated.emit(mb_event)


func _set_panel_style(style: StyleBox) -> void:
	remove_theme_stylebox_override("panel")
	if style != null:
		add_theme_stylebox_override("panel", style)
