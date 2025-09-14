extends Panel
class_name HotbarSlotUI

@onready var icon: TextureRect = $Icon
@onready var slot_index_label: Label = $SlotIndexLabel
@onready var selection_panel: Panel = $SelectionPanel
var stack: ItemStack
var database: InventoryDatabase
var inventory: Inventory


@export var texture_rect: Array[TextureRect]


func set_selection(is_selected: bool):
	selection_panel.visible = is_selected


func _process(_delta: float) -> void:
	if database == null or stack == null:
		return
	var def = database.get_item(stack.item_id)
	if def == null:
		return

func update_info_with_stack(_database: InventoryDatabase, _stack: ItemStack, _inventory: Inventory, slot_index: int):
	self.database = _database
	self.stack = _stack
	self.inventory = _inventory
	for i in texture_rect.size():
		texture_rect[i].visible = i == slot_index
	
	if stack == null:
		icon.visible = false
		return
		
	var def = database.get_item(stack.item_id)
	if def == null:
		icon.visible = false
		size = Vector2(32, 32)
		custom_minimum_size = Vector2(32, 32)
		return
		
	size = Vector2(def.size.x * 32, def.size.y * 32)
	custom_minimum_size = Vector2(def.size.x * 32, def.size.y * 32)
		
	icon.visible = true
	icon.texture = def.icon
