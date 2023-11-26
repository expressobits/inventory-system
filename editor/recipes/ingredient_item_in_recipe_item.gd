@tool
extends HBoxContainer

@onready var texture_rect = $TextureRect
@onready var label = $Label

var slot : Slot

func _ready():
	if slot.item != null:
		texture_rect.texture = slot.item.definition.icon
	else:
		texture_rect.texture = null 
	label.text = "X "+str(slot.amount)


func setup(slot : Slot):
	self.slot = slot
