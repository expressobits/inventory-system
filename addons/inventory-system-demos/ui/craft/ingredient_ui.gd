class_name IngredientUI
extends Control

## Interface represents Ingredient in [RecipeUI].

@onready var icon : TextureRect = get_node("Icon")
@onready var amount : Label = get_node("Amount")
var database : InventoryDatabase

## Setup ingredient
func setup(_database : InventoryDatabase, item_stack : ItemStack):
	self.database = _database
	var definition = database.get_item(item_stack.item_id)
	self.icon.texture = definition.icon
	self.amount.text = "X "+str(item_stack.amount)
