@tool
extends HBoxContainer

@onready var texture_rect = $TextureRect
@onready var label = $Label

var database : InventoryDatabase
var item_stack : ItemStack

func _ready():
	if item_stack == null:
		return
	var definition = database.get_item(item_stack.item_id)
	if definition != null and definition.icon != null:
		texture_rect.texture = definition.icon
	else:
		texture_rect.texture = null 
	label.text = "X "+str(item_stack.amount)


func setup(database : InventoryDatabase, item_stack : ItemStack):
	self.database = database
	self.item_stack = item_stack
