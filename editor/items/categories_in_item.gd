@tool
class_name CategoriesInItem
extends HBoxContainer

signal changed

var item : ItemDefinition
var database : InventoryDatabase
var objs : Array[CheckBox]


@onready var h_flow_container = $ScrollContainer/HFlowContainer


func load_item(database : InventoryDatabase, item : ItemDefinition):
	self.database = database
	self.item = item
	loading_categories()


func loading_categories():
	for p in objs:
		p.queue_free()
	objs.clear()
	for category in database.item_categories:
		var option_obj = CheckBox.new()
		option_obj.text = category.name
		option_obj.button_pressed = item.categories.find(category) != -1
		option_obj.icon = category.icon
		option_obj.expand_icon = true
		option_obj.toggled.connect(_on_toggled_category_option.bind(category))
		objs.append(option_obj)
		h_flow_container.add_child(option_obj)


func make_array_unique(input_array) -> Array[ItemCategory]:
	var unique_array : Array[ItemCategory] = []
	for value in input_array:
		unique_array.append(value)
	return unique_array


func _on_toggled_category_option(toggled : bool, category : ItemCategory):
	var new_categories : Array[ItemCategory] = make_array_unique(item.categories)
	if toggled:
		new_categories.append(category)
	else:
		var index = new_categories.find(category)
		if index > -1:
			new_categories.remove_at(index)
	item.categories = new_categories
	changed.emit()
