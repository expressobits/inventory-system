extends InventoryHandler
class_name NetworkedHandler


func open(inventory : Inventory) -> bool:
	if not multiplayer.is_server():
		open_rpc.rpc_id(1, inventory.get_path())
	else:
		open_rpc(inventory.get_path())
	return true

@rpc
func open_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.open(inventory)


func close(inventory : Inventory) -> bool:
	if not multiplayer.is_server():
		close_rpc.rpc_id(1, inventory.get_path())
	else:
		close_rpc(inventory.get_path())
	return true


## Close all open [Inventory]s.
func close_all_inventories():
	if not multiplayer.is_server():
		close_all_inventories_rpc.rpc_id(1)
	else:
		close_all_inventories_rpc()
	return true

@rpc
func close_all_inventories_rpc():
	super.close_all_inventories()

@rpc
func close_rpc(object_path : NodePath):
	if not multiplayer.is_server():
		return
	var object = get_node(object_path)
	if object == null:
		return
	var inventory = object as Inventory
	if inventory == null:
		return
	super.close(inventory)


## Returns [code]true[/code] if main [Inventory] is open.
func is_open_main_inventory() -> bool:
	return super.is_open(inventory)


## Open main [Inventory]. Return [code]true[/code] if opened successfully.
func open_main_inventory() -> bool:
	return super.open(inventory)


## Open main [Inventory]. Return [code]true[/code] if opened successfully.
func close_main_inventory() -> bool:
	return super.close(inventory)
