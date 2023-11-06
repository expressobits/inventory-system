extends Node

var inventory_handler : InventoryHandler
var hotbar : Hotbar
var crafter : Crafter
var interactor : InventoryInteractor

@export var is_console_mode := false


signal inventory_handler_changed
signal crafter_changed
signal hotbar_changed
signal interactor_changed


func setup_inventory_handler(inventory_handler : InventoryHandler):
	self.inventory_handler = inventory_handler
	inventory_handler_changed.emit()


func setup_hotbar(hotbar : Hotbar):
	self.hotbar = hotbar
	hotbar_changed.emit()


func setup_crafter(crafter : Crafter):
	self.crafter = crafter
	crafter_changed.emit()


func setup_interactor(interactor : InventoryInteractor):
	self.interactor = interactor
	interactor_changed.emit()
