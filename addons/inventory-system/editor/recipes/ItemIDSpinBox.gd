@tool
extends SpinBox


# Called when the node enters the scene tree for the first time.
func _ready():
	value_changed.connect(_on_value_changed.bind())
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func _on_value_changed(value):
	pass # Replace with function body.
