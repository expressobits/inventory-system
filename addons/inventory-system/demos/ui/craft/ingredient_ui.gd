extends Control
class_name IngredientUI

@onready var icon : TextureRect = get_node("Icon")
@onready var amount : Label = get_node("Amount")


func setup(slot : Slot):
	self.icon.texture = slot.item.icon
	self.amount.text = str(slot.amount)
