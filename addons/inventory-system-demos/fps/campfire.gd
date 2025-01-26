class_name Campfire
extends Workbench

signal changed_burning_state(is_burning : bool)

@export var burnable_category : String = "burnables"
@onready var gpu_particles_3d = $Node/GPUParticles3D
@onready var audio_stream_player_3d = $Node/AudioStreamPlayer3D
@onready var craft_audio = $Node/CraftAudio
@export var toggle_fire_action : InteractAction

@export var decrease_fuel_multiplier = 1

var active_fire : bool = true:
	set(value):
		active_fire = value
		if value:
			is_burning = fuel > 0.0
		else:
			is_burning = false
		

var fuel := 0.0:
	set(new_value):
		fuel = new_value
		is_burning = fuel > 0.0
		
var max_seconds := 120.0

var is_burning := false:
	set(new_value):
		if is_burning != new_value:
			is_burning = new_value
			check()
			changed_burning_state.emit(is_burning)
			_update_is_burning()
			

func _ready():
	_update_is_burning()
	craft_station.on_crafted.connect(_on_crafted)
	input_inventory.contents_changed.connect(_on_input_inventory_contents_changed)
	

func _update_is_burning():
	$Node.visible = is_burning
	craft_station.auto_craft = is_burning
	craft_station.can_processing_craftings = is_burning
	audio_stream_player_3d.playing = is_burning


func _on_input_inventory_contents_changed():
	check()


func _on_crafted(_recipe_index: int):
	craft_audio.play(1.0)


func check() -> bool:
	if not active_fire:
		return false
	if is_burning:
		return false
	var category = input_inventory.database.get_category_from_id(burnable_category)
	if not input_inventory.contains_category(category):
		return false
	var index = input_inventory.get_stack_index_with_an_item_of_category(category)
	if index == -1:
		return false
	var item_id = input_inventory.stacks[index].item_id
	var definition = input_inventory.database.get_item(item_id)
	if not definition.properties.has("fuel"):
		return false
	fuel += definition.properties["fuel"]
	input_inventory.remove(item_id)
	return true


func _process(delta):
	if not active_fire:
		return
	if fuel > 0.0:
		fuel -= delta * decrease_fuel_multiplier


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	if openable.is_open:
		return []
	var current_actions : Array = self.actions.duplicate()
	if fuel > 0.0:
		if active_fire:
			toggle_fire_action.description = "Disable Fire"
		else:
			toggle_fire_action.description = "Enable Fire"
		current_actions.append(toggle_fire_action)
	return current_actions


func interact(_character : Node, action_index : int = 0):
	if action_index == 0 and !openable.is_open:
		_character.character_inventory_system.open_inventory(craft_station.get_input_inventory())
		#craft_station.load_valid_recipes()
		openable.open(_character)
	else:
		active_fire = !active_fire
