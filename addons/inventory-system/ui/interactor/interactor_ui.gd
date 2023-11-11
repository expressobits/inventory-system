class_name InteractorUI
extends Control

@onready var interact_message : Label = $InteractPosition/InteractMessage
@onready var interact_position : Control = $InteractPosition
var default_interact_message_position : Vector2
@export var interactor : InventoryInteractor

var interact_object
var interact_hand_object

var labels: Dictionary = {
	"mouse": [
		"LMB", "RMB", "MMB", "MW Up", "MW Down", "MW Left", "MW Right", "MB1", "MB2"
	],
	
	"xbox": [
		"A", "B", "X", "Y", "Back", "Home", "Start", "L", "R", "LB", "RB",
		"DPad Up", "DPad Down", "DPad Left", "DPad Right", "Share"
	],
	
	"ps": [
		"Cross", "Circle", "Square", "Triangle", "Select", "PS", "Start",
		"L3", "R3", "L1", "R1", "DPad Up", "DPad Down", "DPad Left",
		"DPad Right", "Microphone"
	],
	
	"switch": [
		"B", "A", "Y", "X", "Minus", "", "Plus", "", "", "", "",
		"DPad Up", "DPad Down", "DPad Left", "DPad Right", "Capture"
	],
	
	"other": [
		"A", "B", "X", "Y", "Back", "Home", "Start", "L", "R", "LB", "RB",
		"DPad Up", "DPad Down", "DPad Left", "DPad Right", "Share",
		"Paddle 1", "Paddle 2", "Paddle 3", "Paddle 4", "Touch"
	],
	
	"motion": [
		{"-": "LStick Left", "+": "LStick Right"},
		{"-": "LStick Up", "+": "LStick Down"},
		{"-": "RStick Left", "+": "RStick Right"},
		{"-": "RStick Up", "+": "RStick Down"},
		{"+": "Left Trigger"},
		{"+": "Right Trigger"}
	],
}

func setup(interactor : InventoryInteractor):
	if self.interactor != null:
		interactor.preview_interacted.disconnect(_on_preview_interacted.bind())
		interactor.clear_preview.connect(_on_interactor_clear_preview.bind())
	self.interactor = interactor
	interactor.preview_interacted.connect(_on_preview_interacted.bind())
	interactor.clear_preview.connect(_on_interactor_clear_preview.bind())


func _ready():
	get_input_string("interact")
	default_interact_message_position = interact_position.position


func _on_preview_interacted(actions : Array[InteractAction], position_on_screen : Vector2):
	preview_interact(actions, position_on_screen)


func _on_interactor_clear_preview():
	interact_message.visible = false


func preview_interact(actions : Array[InteractAction], position_on_screen : Vector2):
	interact_message.visible = true
	var message = ""
	if position_on_screen == Vector2.ZERO:
		interact_position.position = default_interact_message_position
	else:
		interact_position.position = position_on_screen
	for action in actions:
		message += "["+get_input_string(action.input)+"] " + action.description + "\n"
	interact_message.text = message


func get_input_string(action_name : String) -> String:
	var action_0_events : Array[InputEvent] = InputMap.action_get_events(action_name)
	var action_0_event_0 : InputEvent = action_0_events[0]
	if action_0_event_0 is InputEventKey:
		var input_event_key : InputEventKey = action_0_event_0 as InputEventKey
		var button_name = OS.get_keycode_string(input_event_key.physical_keycode)
		return button_name
	if action_0_event_0 is InputEventMouseButton:
		var input_event_mouse : InputEventMouseButton = action_0_event_0 as InputEventMouseButton
		var button_name = _get_mouse_event_as_text(input_event_mouse)
		return button_name
	return action_name


func _get_mouse_event_as_text(event: InputEventMouseButton) -> String:
	var modifiers: String = _get_modifiers_as_string(event)
	var btn: String = labels["mouse"][event.button_index - 1]
	var result: String = "%s"%btn if modifiers.is_empty() else "%s+%s"%[modifiers, btn]
	return result


func _get_modifiers_as_string(event: InputEventWithModifiers) -> String:
	var modifiers: PackedStringArray
	if event.shift_pressed:
		modifiers.append("Shift")
	
	if event.ctrl_pressed:
		modifiers.append("Ctrl")
	
	if event.alt_pressed:
		modifiers.append("Alt")
	
	var modifiers_string: String = "+".join(modifiers) 
	return modifiers_string
