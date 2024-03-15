################################################################################
# MIT License
# Copyright (c) 2021 PunchablePlushie
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
@tool
class_name InputHelper
extends Resource

enum InputType {INVALID, KEYBOARD, MOUSE, GP_BTN, GP_MOTION}

@export var joy_name_abbr: Dictionary = {
	"XInput Gamepad": "xbox",
	"Xbox Series Controller": "xbox",
	"Sony DualSense": "ps",
	"PS5 Controller": "ps",
	"PS4 Controller": "ps",
	"Switch": "switch",
} 

@export var labels: Dictionary = {
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


func get_event_id(event: InputEvent) -> int:
	if event is InputEventKey:
		if event.physical_keycode == 0:
			return -1
		
		return event.physical_keycode | event.get_modifiers_mask()
	
	if event is InputEventMouseButton:
		return event.button_index | event.get_modifiers_mask()
	
	if event is InputEventJoypadButton:
		return event.button_index
	
	if event is InputEventJoypadMotion:
		return event.axis
	
	return -1


func set_event_id(event: InputEvent, id: int) -> void:
	if event is InputEventKey:
		event.physical_keycode = id & ~(KEY_MASK_SHIFT | KEY_MASK_CTRL | KEY_MASK_ALT)
		_set_event_modifiers(event, id)
	
	if event is InputEventMouseButton:
		event.button_index = id & ~(KEY_MASK_SHIFT | KEY_MASK_CTRL | KEY_MASK_ALT)
		_set_event_modifiers(event, id)
	
	if event is InputEventJoypadButton:
		event.button_index = id
	
	if event is InputEventJoypadMotion:
		event.axis = id


func get_event_type(event: InputEvent) -> InputType:
	if event is InputEventKey:
		return InputType.KEYBOARD
	
	if event is InputEventMouseButton:
		return InputType.MOUSE
	
	if event is InputEventJoypadButton:
		return InputType.GP_BTN
	
	if event is InputEventJoypadMotion:
		return InputType.GP_MOTION
	
	return InputType.INVALID


func create_event_from_type(type: InputType) -> InputEvent:
	match type:
		InputType.KEYBOARD:
			return InputEventKey.new()
		InputType.MOUSE:
			return InputEventMouseButton.new()
		InputType.GP_BTN:
			return InputEventJoypadButton.new()
		InputType.GP_MOTION:
			return InputEventJoypadMotion.new()
		_:
			return null


func input_already_exists(event: InputEvent, self_action: String) -> Array:
	for action in InputMap.get_actions():
		if action.begins_with("ui_"):
			continue
		
		if action == self_action:
			continue
		
		if InputMap.action_has_event(action, event):
			return [true, action]
	
	return [false, ""]

 
func _set_event_modifiers(event: InputEventWithModifiers, modifier_mask: int) -> void:
	event.shift_pressed = bool(modifier_mask & KEY_MASK_SHIFT)
	event.ctrl_pressed = bool(modifier_mask & KEY_MASK_CTRL)
	event.alt_pressed = bool(modifier_mask & KEY_MASK_ALT)


### Events as Text

func get_event_as_text(event: InputEvent) -> String:
	if get_event_id(event) == -1:
		return "INVALID"
	
	if event is InputEventKey:
		return OS.get_keycode_string(event.get_physical_keycode_with_modifiers())
	
	if event is InputEventMouseButton:
		return _get_mouse_event_as_text(event)
	
	if event is InputEventJoypadButton:
		return _get_gp_btn_event_as_text(event)
	
	if event is InputEventJoypadMotion:
		return _get_gp_motion_event_as_text(event)
	
	return ""


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


func _get_joy_name_abbr(name: String) -> String:
	if joy_name_abbr.has(name):
		return joy_name_abbr[name]
	else:
		return "other"


func _get_mouse_event_as_text(event: InputEventMouseButton) -> String:
	var modifiers: String = _get_modifiers_as_string(event)
	var btn: String = labels["mouse"][event.button_index - 1]
	var result: String = "%s"%btn if modifiers.is_empty() else "%s+%s"%[modifiers, btn]
	return result


func _get_gp_btn_event_as_text(event: InputEventJoypadButton) -> String:
	var device_name: String = Input.get_joy_name(event.device)
	device_name = _get_joy_name_abbr(device_name)
	return labels[device_name][event.button_index]


func _get_gp_motion_event_as_text(event: InputEventJoypadMotion) -> String:
	var axis_value: String = "-" if event.axis_value < 0 else "+"
	return labels["motion"][event.axis][axis_value]


### Events as Icons

func get_event_as_icon(event: InputEvent, icon_db: InputIcons) -> Texture2D:
	if event is InputEventKey:
		return _get_keyboard_event_as_icon(event, icon_db)
		
	if event is InputEventMouseButton:
		return _get_mouse_event_as_icon(event, icon_db)
	
	if event is InputEventJoypadButton:
		return _get_gp_btn_event_as_icon(event, icon_db)
	
	if event is InputEventJoypadMotion:
		return _get_gp_motion_event_as_icon(event, icon_db)
	
	return null


func _get_keyboard_event_as_icon(event: InputEventKey, icon_db: InputIcons) -> Texture2D:
	var keycode: int = event.physical_keycode
	var icon: Texture2D = icon_db.get_keyboard_button_texture(keycode)
	
	return icon


func _get_mouse_event_as_icon(event: InputEventMouse, icon_db: InputIcons) -> Texture2D:
	var button_index: int = event.button_index
	var icon: Texture2D = icon_db.get_mouse_button_texture(button_index)
	
	return icon


func _get_gp_btn_event_as_icon(event: InputEventJoypadButton, icon_db: InputIcons) -> Texture2D:
	var device_name: String = Input.get_joy_name(event.device)
	device_name = _get_joy_name_abbr(device_name)
	
	var button_index: int = event.button_index
	var icon: Texture2D = icon_db.get_gp_button_texture(device_name, button_index)
	
	return icon


func _get_gp_motion_event_as_icon(event: InputEventJoypadMotion, icon_db: InputIcons) -> Texture2D:
	var device_name: String = Input.get_joy_name(event.device)
	device_name = _get_joy_name_abbr(device_name)
	
	var axis: int = event.axis
	var axis_dir: String = "-" if event.axis_value < 1 else "+"
	var icon: Texture2D = icon_db.get_gp_motion_texture(device_name, axis, axis_dir)
	
	return icon

