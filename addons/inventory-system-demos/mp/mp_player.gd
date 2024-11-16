extends Player
class_name MPPlayer

func _enter_tree():
	set_multiplayer_authority(str(name).to_int())
	$CharacterInventorySystem/Inventory/SyncInventory.set_multiplayer_authority(1)
	$CharacterInventorySystem/EquipmentInventory/SyncInventory.set_multiplayer_authority(1)
	%SyncHotbar.set_multiplayer_authority(1)
	$CharacterInventorySystem/CraftStation/SyncCraftStation.set_multiplayer_authority(1)
	$Dropper.set_multiplayer_authority(1)

func _ready():
	super._ready()
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


func _input(event: InputEvent) -> void:
	if is_multiplayer_authority():
		super._input(event)
