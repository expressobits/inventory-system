class_name ActionMessageUI
extends HBoxContainer

@export var input_helper : InputHelper
@export var input_icons : InputIcons
@export var texture_rect : TextureRect
@export var interact_message : Label
@export var action_input : String:
	set(value):
		action_input = value
		setup_icon(value)
@export var description : String:
	set(value):
		description = value
		setup_description(value)
		


func setup_icon(input_text : String):
	var action_0_events : Array[InputEvent] = InputMap.action_get_events(input_text)
	if action_0_events.is_empty():
		return
	var action_0_event_0 : InputEvent = action_0_events[0]
	var texture = input_helper.get_event_as_icon(action_0_event_0, input_icons)
	texture_rect.visible = true
	texture_rect.texture = texture


func setup_description(new_description : String):
	interact_message.text = new_description


func hide_message():
	visible = false


func show_message(new_action_input : String, new_description : String):
	self.action_input = new_action_input
	self.description = new_description
	visible = true
