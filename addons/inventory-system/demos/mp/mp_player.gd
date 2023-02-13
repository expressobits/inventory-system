extends Player
class_name MPPlayer


func _enter_tree():
	set_multiplayer_authority(str(name).to_int())


func _ready():
	if is_multiplayer_authority():
		$Camera3D.make_current()
		print("local")
	else:
		$Camera3D.clear_current()
		print("remote")
		
	if is_multiplayer_authority():
		$Mesh.visible = false


func open_inventory(inventory : Inventory):
	if not inventory_handler.is_open(inventory):
		inventory_handler.open(inventory)
	if not inventory_handler.is_open_main_inventory():
		open_main_inventory()
		
@rpc("call_local")
func open_inventory_rpc(inventory : Inventory):
	if not inventory_handler.is_open(inventory):
		inventory_handler.open(inventory)
	if not inventory_handler.is_open_main_inventory():
		open_main_inventory()


func open_main_inventory():
	if is_multiplayer_authority():
		open_main_inventory_rpc()
	else:
		open_main_inventory_rpc.rpc_id(1)


#func open_other_inventory(inventory : Inventory):
#	if multiplayer.is_server():
#		open_other_inventory_rpc(inventory)
#	else:
#		open_other_inventory_rpc.rpc_id(1, inventory)

#@rpc
#func open_other_inventory_rpc(inventory : Inventory):
#	open_main_inventory_rpc.rpc_id(1)	

@rpc
func open_main_inventory_rpc():
	if not multiplayer.is_server():
		return
	if inventory_handler.is_open_main_inventory():
		inventory_handler.close_all_inventories()
	else:
		inventory_handler.open_main_inventory()


func _physics_process(delta):
	if is_multiplayer_authority():
		super._physics_process(delta)


func _process(delta):
	if is_multiplayer_authority():
		super._process(delta)


func _input(event: InputEvent) -> void:
	if is_multiplayer_authority():
		super._input(event)
