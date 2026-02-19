@tool
class_name CustomPropertiesItemEditor
extends VBoxContainer

var item : ItemDefinition
var database : InventoryDatabase
var properties_obj : Array

var v_box_container: VBoxContainer
var add_button
var line_edit
var new_type_option_button


func _ready():
	size_flags_horizontal = SIZE_EXPAND_FILL
	size_flags_vertical = SIZE_EXPAND_FILL
	
	var label: Label = Label.new()
	label.layout_mode = 2
	label.text = "Custom Properties"
	label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	add_child(label)
	
	var panel: Control = Control.new()
	panel.clip_contents = true
	panel.custom_minimum_size = Vector2(0, 40)
	panel.layout_mode = 2
	add_child(panel)
	
	var hbox_container: HBoxContainer = HBoxContainer.new()
	hbox_container.layout_mode = 1
	hbox_container.anchors_preset = 14
	hbox_container.anchor_top = 0.5
	hbox_container.anchor_right = 1.0
	hbox_container.anchor_bottom = 0.5
	hbox_container.offset_top = -15.5
	hbox_container.offset_bottom = 15.5
	hbox_container.grow_horizontal = GROW_DIRECTION_BOTH
	hbox_container.grow_vertical = GROW_DIRECTION_BOTH
	panel.add_child(hbox_container)
	
	var label2: Label = Label.new()
	label2.layout_mode = 2
	label2.text = "New Property:"
	label2.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	hbox_container.add_child(label2)
	
	line_edit = LineEdit.new()
	line_edit.layout_mode = 2
	line_edit.size_flags_horizontal = 3
	line_edit.placeholder_text = "Place New Property Key Name Here..."
	line_edit.text_changed.connect(_on_new_key_line_edit_text_changed)
	hbox_container.add_child(line_edit)
	
	new_type_option_button = OptionButton.new()
	new_type_option_button.layout_mode = 2
	new_type_option_button.selected = 0
	hbox_container.add_child(new_type_option_button)
	
	add_button = Button.new()
	add_button.layout_mode = 2
	add_button.text = "Add New Property"
	add_button.pressed.connect(_on_add_button_pressed)
	hbox_container.add_child(add_button)
	
	var scroll_container = ScrollContainer.new()
	scroll_container.custom_minimum_size = Vector2(0, 128)
	scroll_container.layout_mode = 2
	scroll_container.size_flags_vertical = SIZE_EXPAND_FILL
	add_child(scroll_container)
	
	v_box_container = VBoxContainer.new()
	v_box_container.layout_mode = 2
	v_box_container.size_flags_horizontal = SIZE_EXPAND_FILL
	v_box_container.size_flags_vertical = SIZE_EXPAND_FILL
	scroll_container.add_child(v_box_container)
	
	add_button.disabled = line_edit.text.is_empty()
	build_type_options()


func load_item(database : InventoryDatabase, item : ItemDefinition):
	self.database = database
	self.item = item
	loading_properties()


func make_dictionary_unique(input_dictionary) -> Dictionary:
	var unique_dictionary := {}
	for key in input_dictionary.keys():
		unique_dictionary[key] = input_dictionary[key]
	return unique_dictionary


func loading_properties():
	for p in properties_obj:
		p.queue_free()
	properties_obj.clear()
	if item == null:
		return
	var new_item_properties : Dictionary = make_dictionary_unique(item.properties)
	for key in new_item_properties.keys():
		if key is String:
			var property : ItemDefinitionPropertyEditor = ItemDefinitionPropertyEditor.new()
			property.setup(key, item.dynamic_properties.find(key) != -1, item.properties[key])
			property.removed.connect(_property_removed.bind(item))
			property.changed.connect(_property_changed.bind(item))
			properties_obj.append(property)
			v_box_container.add_child(property)
	item.properties = new_item_properties


func build_type_options():
	new_type_option_button.clear()
	new_type_option_button.add_icon_item(get_theme_icon("bool", "EditorIcons"),"Boolean", TYPE_BOOL)
	new_type_option_button.add_icon_item(get_theme_icon("int", "EditorIcons"),"Integer", TYPE_INT)
	new_type_option_button.add_icon_item(get_theme_icon("float", "EditorIcons"),"Float", TYPE_FLOAT)
	new_type_option_button.add_icon_item(get_theme_icon("String", "EditorIcons"),"String", TYPE_STRING)
	new_type_option_button.add_icon_item(get_theme_icon("Color", "EditorIcons"),"Color", TYPE_COLOR)


func _property_removed(key, item : ItemDefinition):
	item.properties.erase(key)
	loading_properties()


func _property_changed(key : String, is_dynamic : bool, value, item : ItemDefinition):
	item.properties[key] = value
	var index = item.dynamic_properties.find(key)
	var contains = index != -1
	if contains != is_dynamic:
		if is_dynamic:
			item.dynamic_properties.append(key)
		else:
			item.dynamic_properties.remove_at(index)


func _on_add_button_pressed():
	var new_item_properties : Dictionary = make_dictionary_unique(item.properties)
	match new_type_option_button.get_selected_id():
		TYPE_BOOL:
			new_item_properties[line_edit.text] = false
		TYPE_INT:
			new_item_properties[line_edit.text] = 0
		TYPE_FLOAT:
			new_item_properties[line_edit.text] = 0.0
		TYPE_STRING:
			new_item_properties[line_edit.text] = ""
		TYPE_COLOR:
			new_item_properties[line_edit.text] = Color()
	item.properties = new_item_properties
	loading_properties()


func _on_new_key_line_edit_text_changed(new_text):
	add_button.disabled = line_edit.text.is_empty()
	
