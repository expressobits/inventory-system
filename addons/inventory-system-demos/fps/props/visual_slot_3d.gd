class_name VisualSlot3D
extends Node3D

@export_node_path("Node3D") var default_hand_item_object_path = NodePath("DefaultItem3D")
@onready var default_hand_item_object := get_node(default_hand_item_object_path) 
@export var distance_for_stack : float = 0.05

var property_name_of_visual : String
var last_item_id : String = ""
var inventory : Inventory
#var objects_per_id : Dictionary
var slot_index : int
var objects : Array


func setup(inv : Inventory, new_slot_index : int, property := "visual_3d"):
	self.property_name_of_visual = property
	self.inventory = inv
	self.inventory.updated_stack.connect(_on_updated_slot)
	self.slot_index = new_slot_index
	_on_updated_slot(slot_index)
#	objects_per_id = {}


func _on_updated_slot(changed_slot_index : int):
	if self.slot_index != changed_slot_index:
		return
	if changed_slot_index >= inventory.stacks.size():
		return
	_clear_last_visual()
	var item_id = inventory.items[slot_index].item_id
	var amount = inventory.items[slot_index].amount
	var definition = inventory.database.get_item(item_id)
	if item_id == "" or definition == null:
		return
	var item_scene = null
	if definition.properties.has(property_name_of_visual):
		var path = definition.properties[property_name_of_visual]
		item_scene = load(path)
	last_item_id = item_id
	
	if item_scene == null:
#		default_hand_item_object.visible = true
		return
#	if objects_per_id.has(item):
#		objects_per_id[item].visible = true
#	else:
	for i in amount:
		var item_obj = item_scene.instantiate()
		add_child(item_obj)
		item_obj.position.y = i * distance_for_stack
		objects.append(item_obj)
#		objects_per_id[item] = item_obj


func _clear_last_visual():
#	default_hand_item_object.visible = false
#	if last_item == null:
#		return
	for i in objects:
		i.queue_free()
	objects.clear()
#	if objects_per_id.has(last_item):
#		objects_per_id[last_item].visible = false	
