@tool
extends Panel
class_name GridDraggableElementUI

# Somewhat hacky way to do static signals:
# https://stackoverflow.com/questions/77026156/how-to-write-a-static-event-emitter-in-gdscript/77026952#77026952

static var dragable_grabbed: Signal = (func():
	if (GridDraggableElementUI as Object).has_user_signal("dragable_grabbed"):
		return (GridDraggableElementUI as Object).dragable_grabbed
	(GridDraggableElementUI as Object).add_user_signal("dragable_grabbed")
	return Signal(GridDraggableElementUI, "dragable_grabbed")
).call()

static var dragable_dropped: Signal = (func():
	if (GridDraggableElementUI as Object).has_user_signal("dragable_dropped"):
		return (GridDraggableElementUI as Object).dragable_dropped
	(GridDraggableElementUI as Object).add_user_signal("dragable_dropped")
	return Signal(GridDraggableElementUI, "dragable_dropped")
).call()

signal grabbed(position)
#signal dropped(zone, position)

static var _grabbed_dragable: GridDraggableElementUI = null
static var _grab_offset: Vector2

var _enabled: bool = true


static func get_grabbed_dragable() -> GridDraggableElementUI:
	if !is_instance_valid(_grabbed_dragable):
		return null
	return _grabbed_dragable


static func get_grab_offset() -> Vector2:
	return _grab_offset


static func get_grab_offset_local_to(control: Control) -> Vector2:
	return GridDraggableElementUI.get_grab_offset() / control.get_global_transform().get_scale()


func _get_drag_data(at_position: Vector2):
	if !_enabled:
		return null

	_grabbed_dragable = self
	_grab_offset = at_position * get_global_transform().get_scale()
	dragable_grabbed.emit(_grabbed_dragable, _grab_offset)
	grabbed.emit(_grab_offset)

	var preview = Control.new()
	var sub_preview = create_preview()
	sub_preview.position = -get_grab_offset()
	preview.add_child(sub_preview)
	set_drag_preview(preview)
	return self


func _notification(what: int) -> void:
	if what == NOTIFICATION_DRAG_END:
		_grabbed_dragable = null


func create_preview() -> Control:
	return null


func activate() -> void:
	_enabled = true


func deactivate() -> void:
	_enabled = false


func is_active() -> bool:
	return _enabled


func is_dragged() -> bool:
	return _grabbed_dragable == self
