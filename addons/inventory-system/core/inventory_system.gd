extends Node

var inventory_handler : InventoryHandler
var crafter : Crafter

@export var is_console_mode := false


signal inventory_handler_changed
signal crafter_changed


func setup_inventory_handler(inventory_handler : InventoryHandler):
	self.inventory_handler = inventory_handler
	inventory_handler_changed.emit()


func setup_crafter(crafter : Crafter):
	self.crafter = crafter
	crafter_changed.emit()
