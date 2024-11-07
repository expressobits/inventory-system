extends Panel
class_name GridPriorityPanel

enum StylePriority {HIGH = 0, MEDIUM = 1, LOW = 2}

var regular_style: StyleBox
var hover_style: StyleBox
var _styles: Array[StyleBox] = [null, null, null]

func _init(regular_style_: StyleBox, hover_style_: StyleBox) -> void:
	regular_style = regular_style_
	hover_style = hover_style_
	
func _ready() -> void:
	set_style(regular_style)
	mouse_entered.connect(func():
		set_style(hover_style)
	)
	mouse_exited.connect(func():
		set_style(regular_style)
	)


func set_style(style: StyleBox, priority: int = StylePriority.LOW) -> void:
	if priority > 2 || priority < 0:
		return
	if _styles[priority] == style:
		return

	_styles[priority] = style

	for i in range(0, 3):
		if _styles[i] != null:
			_set_panel_style(_styles[i])
			return


func _set_panel_style(style: StyleBox) -> void:
	remove_theme_stylebox_override("panel")
	if style != null:
		add_theme_stylebox_override("panel", style)
