extends Node
class_name SyncInventoryHandler

# TODO Create this sync in character system inventory
# @export var inventory_handler : InventoryHandler

# func _ready() -> void:
# 	inventory_handler.updated_transaction_slot.connect(on_updated_transaction_slot)

	
# func on_updated_transaction_slot():
# 	if not multiplayer.is_server():
# 		return
# 	var item_id : int
# 	if not inventory_handler.transaction_slot.has_valid():
# 		item_id = ItemDefinition.NONE
# 	else:
# 		item_id = inventory_handler.transaction_slot.item.definition.id
# 	updated_transaction_slot_rpc.rpc(item_id, inventory_handler.transaction_slot.amount)

# @rpc
# func updated_transaction_slot_rpc(item_id : int, amount : int):
# 	var definition = inventory_handler.database.get_item(item_id)
# 	var item = Item.new()
# 	item.definition = definition
# 	inventory_handler.change_transaction_slot(item, amount);
