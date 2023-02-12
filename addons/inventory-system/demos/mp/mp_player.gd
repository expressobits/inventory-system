extends Player
class_name MPPlayer

func _ready():
	if str(multiplayer.get_unique_id()) == name:
		$Camera3D.make_current()
		print("local")
	else:
		$Camera3D.clear_current()
		print("remote")
		
	if str(multiplayer.get_unique_id()) == name:
		$Mesh.visible = false


func _physics_process(delta):
	if is_multiplayer_authority():
		super._physics_process(delta)


func _process(delta):
	if is_multiplayer_authority():
		super._process(delta)


func _input(event: InputEvent) -> void:
	if is_multiplayer_authority():
		super._input(event)

