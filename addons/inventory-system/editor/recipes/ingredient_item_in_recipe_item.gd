@tool
extends HBoxContainer

@onready var texture_rect = $TextureRect
@onready var label = $Label

var slot : Slot
var database : InventoryDatabase


func _ready():
	var item = database.get_item(slot.id)
	if item != null:
		texture_rect.texture = item.icon
	else:
		texture_rect.texture = null 
	label.text = "X "+str(slot.amount)


func setup(slot : Slot, database : InventoryDatabase):
	self.slot = slot
	self.database = database
