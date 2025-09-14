extends Panel
class_name HotbarSlotUI

@onready var icon: TextureRect = %Icon
@onready var slot_index_label: Label = $SlotIndexLabel
@onready var selection_panel: Panel = $SelectionPanel
var stack: ItemStack
var database: InventoryDatabase
var inventory: Inventory
var default_size: Vector2


@export var texture_rect: Array[TextureRect]

func _ready() -> void:
	default_size = size


func set_selection(is_selected: bool):
	selection_panel.visible = is_selected


func _process(_delta: float) -> void:
	if database == null or stack == null:
		return
	var def = database.get_item(stack.item_id)
	if def == null:
		return

func update_info_with_stack(_database: InventoryDatabase, _stack: ItemStack, _inventory: Inventory, slot_indexes: PackedInt32Array):
	self.database = _database
	self.stack = _stack
	self.inventory = _inventory
	for i in texture_rect.size():
		texture_rect[i].visible = i == slot_indexes[0]
	var slot_text = ""
	for i in slot_indexes:
		slot_text += str(i + 1)
		if slot_indexes.size() != i + 1:
			slot_text += " "
	slot_index_label.text = slot_text
	
	if stack == null:
		icon.visible = false
		return
		
	var def = database.get_item(stack.item_id)
	if def == null:
		icon.visible = false
		size = default_size
		custom_minimum_size = default_size
		return
		
	size = Vector2(def.size.x * default_size.x, def.size.y * default_size.y)
	custom_minimum_size = Vector2(def.size.x * default_size.x, def.size.y * default_size.y)
		
	icon.visible = true
	icon.texture = def.icon
	icon.size = Vector2(def.size.x * default_size.x, def.size.y * default_size.y)
