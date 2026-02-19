@tool
class_name IngredientItemInRecipeItem
extends HBoxContainer

var texture_rect : TextureRect
var label : Label

var database : InventoryDatabase
var item_stack : ItemStack

func _ready():
	custom_minimum_size = Vector2(48, 0)
	add_theme_constant_override("separation", 0)
	
	texture_rect = TextureRect.new()
	texture_rect.custom_minimum_size = Vector2(24, 24)
	texture_rect.layout_mode = 2
	texture_rect.size_flags_vertical = SIZE_SHRINK_CENTER
	texture_rect.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	add_child(texture_rect)
	
	label = Label.new()
	label.layout_mode = 2
	label.size_flags_horizontal = 3
	label.size_flags_vertical = 1
	add_child(label)
	
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
