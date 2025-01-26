@tool
extends Control
class_name GridDropZoneUI

signal dragable_dropped(dragable, position)


func activate() -> void:
	mouse_filter = Control.MOUSE_FILTER_PASS


func deactivate() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE


func is_active() -> bool:
	return (mouse_filter != Control.MOUSE_FILTER_IGNORE)


func _can_drop_data(_at_position: Vector2, data) -> bool:
	if data == null:
		return false
	return data is GridDraggableElementUI


func _drop_data(at_position: Vector2, data) -> void:
	var local_offset := GridDraggableElementUI.get_grab_offset_local_to(self)
	dragable_dropped.emit(data, at_position - local_offset)
	GridDraggableElementUI.dragable_dropped.emit(data, self, at_position - local_offset)
