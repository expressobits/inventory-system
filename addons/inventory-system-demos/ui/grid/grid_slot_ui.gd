@tool
extends Panel
class_name GridSlotUI

enum StylePriority {HIGH = 0, MEDIUM = 1, LOW = 2}

@export var regular_style: StyleBox
@export var hover_style: StyleBox
var _styles: Array[StyleBox] = [null, null, null]

var item_selected: bool = false:
	set(value):
		pass
		#print("item_selected", value)


var item_dragged: bool = false:
	set(value):
		pass
		#print("item_grabbed",value)


func _ready() -> void:
	_set_panel_style(regular_style)
	mouse_entered.connect(func():
		_set_panel_style(hover_style)
	)
	mouse_exited.connect(func():
		_set_panel_style(regular_style)
	)


func set_style(style: StyleBox, priority: int = StylePriority.LOW) -> void:
	if priority > 2 || priority < 0:
		return
	if _styles[priority] == style:
		return

	#_styles[priority] = style
#
	#for i in range(0, 3):
		#if _styles[i] != null or i == 2:
			#_set_panel_style(_styles[i])
			#return


func _set_panel_style(style: StyleBox) -> void:
	remove_theme_stylebox_override("panel")
	if style != null:
		add_theme_stylebox_override("panel", style)
