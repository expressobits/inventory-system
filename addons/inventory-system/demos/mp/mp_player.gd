extends Player
class_name MPPlayer


func _enter_tree():
	set_multiplayer_authority(str(name).to_int())
	$InventoryHandler.set_multiplayer_authority(1)
	$InventoryHandler/Inventory.set_multiplayer_authority(1)
	$Crafter.set_multiplayer_authority(1)
	$Crafter/CraftStation.set_multiplayer_authority(1)


func _ready():
	if is_multiplayer_authority():
		$Camera3D.make_current()
	else:
		$Camera3D.clear_current()
	$Camera3D/HandItem.visible = is_multiplayer_authority()
	$Camera3D/ThirdPersonHandItem.visible = not is_multiplayer_authority()
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
