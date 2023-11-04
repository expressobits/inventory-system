extends CharacterBody3D
class_name Player

const SPEED = 5.0
const JUMP_VELOCITY = 4.5

# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

@export var mouse_sensitivity := 0.002
@export var vertical_angle_limit := 90.0
var rot := Vector3()
@onready var character_inventory_system = $CharacterInventorySystem
@onready var raycast : RayCast3D = $Camera3D/RayCast3D
@onready var camera_3d : Camera3D = $Camera3D
@onready var interact_message_position : Control = $"../UI/Labels/Control"
@onready var interact_message : Label = $"../UI/Labels/Control/InteractMessage"
var default_interact_message_position : Vector2

func _ready():
	default_interact_message_position = interact_message_position.position


func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta

	# Handle Jump.
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
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
	
	interact()	


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


func interact():
	if raycast.is_colliding():
		var object = raycast.get_collider()
		var node = object as Node
		var shelf := node as Shelf
		if shelf != null:
			var inv = shelf.get_inventory()
			if inv != null:
				interact_message.visible = !character_inventory_system.inventory_handler.is_open(inv)
				interact_message.text = shelf.get_interact_message()
				interact_message_position.position = camera_3d.unproject_position(shelf.get_interaction_position(raycast.get_collision_point()))
				if Input.is_action_just_pressed("interact"):
					character_inventory_system.open_inventory(inv)
					return
				if Input.is_action_just_pressed("item_pickup"):
					var item = shelf.get_actual_item()
					if item != null:
						inv.remove_at(shelf.slot_index, item)
					return
				return
		var box := node as BoxInventory
		if box != null:
			var inv = box.get_inventory()
			if inv != null:
				interact_message.visible = !character_inventory_system.inventory_handler.is_open(inv)
				interact_message.text = "E to Open Inventory"
				interact_message_position.position = camera_3d.unproject_position(box.position)
				if Input.is_action_just_pressed("interact"):
					character_inventory_system.open_inventory(inv)
				return
		var campfire := node as Campfire
		if campfire != null:
			var station = campfire.get_station()
			if station != null:
				interact_message.visible = !character_inventory_system.crafter.is_open(station)
				interact_message.text = "E to Open Station"
				interact_message_position.position = camera_3d.unproject_position(campfire.position)
				if Input.is_action_just_pressed("interact"):
					character_inventory_system.open_inventory(station.input_inventory)
				return
		var workbench := node as Workbench
		if workbench != null:
			var station = workbench.get_station()
			if station != null:
				interact_message.visible = !character_inventory_system.crafter.is_open(station)
				interact_message.text = "E to Open Station"
				interact_message_position.position = camera_3d.unproject_position(workbench.position)
				if Input.is_action_just_pressed("interact"):
					character_inventory_system.open_station(station)
				return
		var dropped_item := node as DroppedItem3D
		if dropped_item != null:
			if dropped_item.is_pickable:
				interact_message.visible = true
				interact_message.text = "E to Pickup"
				interact_message_position.position = camera_3d.unproject_position(dropped_item.position)

				if Input.is_action_just_pressed("interact"):
					character_inventory_system.inventory_handler.pick_to_inventory(dropped_item)
			return
		if node != null:
			if node.is_in_group("placeable"):
				if Input.is_action_just_pressed("interact_item"):
					var item = character_inventory_system.hotbar.get_selected_item()
					if item != null:
						place_item(item, raycast.get_collision_point())
	interact_message.visible = false
	interact_message_position.position = default_interact_message_position

func place_item(item : InventoryItem, position_to_place : Vector3):
	# TODO Add Preview
	if !item.properties.has("placeable"):
		return
	var path = item.properties["placeable"]
	var res = load(path)
	if res is PackedScene:
		var scene = res as PackedScene
		var obj = scene.instantiate()
		obj.position = position_to_place 
		get_node("..").add_child(obj)
		print("place item ",item.name)
		character_inventory_system.inventory_handler.inventory.remove(item)
