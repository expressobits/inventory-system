class_name InteractorUI
extends Control

@onready var interact_message : Label = $InteractPosition/InteractMessage
@onready var interact_position : Control = $InteractPosition
var default_interact_message_position : Vector2
@export var interactor : InventoryInteractor

var interact_object
var interact_hand_object

func setup(interactor : InventoryInteractor):
	if self.interactor != null:
		interactor.preview_interacted.disconnect(_on_preview_interacted.bind())
	self.interactor = interactor
	interactor.preview_interacted.connect(_on_preview_interacted.bind())


func _ready():
	default_interact_message_position = interact_position.position


func _on_preview_interacted(object : Node, hand_object : Node, position_on_screen : Vector2):
	preview_interact(object, hand_object, position_on_screen)


func _process(delta):
	var message = ""
	if interact_object != null and interact_object.has_method("get_interact_preview_message"):
		message += interact_object.get_interact_preview_message(interactor)
	if interact_hand_object != null and interact_hand_object.has_method("get_interact_preview_message"):
		message += interact_hand_object.get_interact_preview_message(interactor)
	interact_message.text = message


func preview_interact(object : Node, hand_object : Node, position_on_screen : Vector2):
	interact_object = object
	interact_hand_object = hand_object
	if position_on_screen == Vector2.ZERO:
		interact_position.position = default_interact_message_position
	else:
		interact_position.position = position_on_screen
