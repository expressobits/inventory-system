extends Node
class_name LagTest

@export var actived := false

func _process(_delta: float) -> void:
	if not actived:
		return
	if multiplayer.is_server():
		for i in 50000000:
			var _a := 2 + 2
