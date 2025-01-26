class_name Player
extends CharacterBody3D

const SPEED = 5.0
const JUMP_VELOCITY = 4.5

# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

@export var mouse_sensitivity := 0.002
@export var vertical_angle_limit := 90.0
var rot := Vector3()
@onready var character_inventory_system : CharacterInventorySystem = $CharacterInventorySystem
@onready var raycast : RayCast3D = $Camera3D/RayCast3D
@onready var camera_3d : Camera3D = $Camera3D

@export_group("🔊 Audios")
@export_node_path("AudioStreamPlayer3D") var picked_audio_path := NodePath("PickupAudio")
@onready var picked_audio : AudioStreamPlayer3D = get_node(picked_audio_path)
@export_node_path("AudioStreamPlayer3D") var drop_audio_path := NodePath("DropAudio")
@onready var drop_audio : AudioStreamPlayer3D = get_node(drop_audio_path)
@export_node_path("AudioStreamPlayer3D") var hotbar_change_audio_path := NodePath("HotbarChangeAudio")
@onready var hotbar_change_audio : AudioStreamPlayer3D = get_node(hotbar_change_audio_path)
@export_node_path("AudioStreamPlayer3D") var player_inventory_open_audio_path := NodePath("PlayerInventoryOpenAudio")
@onready var player_inventory_open_audio : AudioStreamPlayer3D = get_node(player_inventory_open_audio_path)
@export_node_path("AudioStreamPlayer3D") var player_inventory_close_audio_path := NodePath("PlayerInventoryCloseAudio")
@onready var player_inventory_close_audio : AudioStreamPlayer3D = get_node(player_inventory_close_audio_path)


func _ready():
	# Setup for audios 🔊
	character_inventory_system.picked.connect(_on_inventory_handler_picked)
	character_inventory_system.opened_inventory.connect(_on_player_inventory_opened)
	character_inventory_system.closed_inventory.connect(_on_player_inventory_closed)
	#character_inventory_system.hotbar.on_change_selection.connect(_on_hotbar_changed)
	
	
func _on_inventory_handler_picked(_dropped_item):
	picked_audio.play()
	

func _on_player_inventory_opened(_inventory : Inventory):
	player_inventory_open_audio.play()


func _on_player_inventory_closed(_inventory : Inventory):
	player_inventory_close_audio.play()


func _on_hotbar_changed(_selection : int):
	hotbar_change_audio.play()


func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta

	# Handle Jump.
	if Input.is_action_just_pressed("jump_move") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var input_dir = Input.get_vector("left_move", "right_move", "forward_move", "back_move")
	var direction = (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction and Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		velocity.x = direction.x * SPEED
		velocity.z = direction.z * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)
		velocity.z = move_toward(velocity.z, 0, SPEED)

	move_and_slide()


func _input(event: InputEvent) -> void:
	# Mouse look (only if the mouse is captured).
	if event is InputEventMouseMotion and Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		rotate_camera(event.relative)


func rotate_camera(mouse_axis : Vector2) -> void:
	# Horizontal mouse look.
	rot.y -= mouse_axis.x * mouse_sensitivity
	# Vertical mouse look.
	rot.x = clamp(rot.x - mouse_axis.y * mouse_sensitivity, -vertical_angle_limit, vertical_angle_limit)
	
	rotation.y = rot.y
	$Camera3D.rotation.x = rot.x
