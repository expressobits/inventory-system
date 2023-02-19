extends Player
class_name MPPlayer


func _enter_tree():
	set_multiplayer_authority(str(name).to_int())


func _ready():
	if is_multiplayer_authority():
		$Camera3D.make_current()
	else:
		$Camera3D.clear_current()
		
	if is_multiplayer_authority():
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
