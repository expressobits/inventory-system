class_name InteractorUI
extends Control

@export var interact_messages : Array[ActionMessageUI]
@onready var interact_position : Control = $InteractPosition
var default_interact_message_position : Vector2
@export var interactor : InventoryInteractor
@export var input_icons : InputIcons

var interact_object
var interact_hand_object

func setup(interactor : InventoryInteractor):
	if self.interactor != null:
		interactor.preview_interacted.disconnect(_on_preview_interacted.bind())
	self.interactor = interactor
	interactor.preview_interacted.connect(_on_preview_interacted.bind())


func _ready():
	for i in interact_messages:
		i.input_icons = input_icons
	default_interact_message_position = interact_position.position


func _on_preview_interacted(actions : Array[InteractAction], position_on_screen : Vector2):
	preview_interact(actions, position_on_screen)


func preview_interact(actions : Array[InteractAction], position_on_screen : Vector2):
	for i in interact_messages:
		i.visible = true
	if position_on_screen == Vector2.ZERO:
		interact_position.position = default_interact_message_position
	else:
		interact_position.position = position_on_screen
#	for action in actions:
#		message += "["+get_input_string(action.input)+"] " + action.description + "\n"
	for i in interact_messages.size():
		var interact_message = interact_messages[i]
		if i < actions.size():
			interact_message.show_message(actions[i].input, actions[i].description)
		else:
			interact_message.hide_message()
		
