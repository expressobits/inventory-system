extends Control
class_name IngredientUI

## Interface represents Ingredient in [RecipeUI].

@onready var icon : TextureRect = get_node("Icon")
@onready var amount : Label = get_node("Amount")

## Setup ingredient
func setup(slot : Slot):
	self.icon.texture = slot.item.definition.icon
	self.amount.text = "X "+str(slot.amount)
