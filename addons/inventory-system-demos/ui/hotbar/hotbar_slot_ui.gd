extends Panel

@onready var icon: TextureRect = $Icon
@onready var slot_index_label: Label = $SlotIndexLabel
@onready var selection_panel: Panel = $SelectionPanel


func set_selection(is_selected: bool):
	selection_panel.visible = is_selected


func update_info_with_stack(database: InventoryDatabase, stack: ItemStack, slot_index: int):
	slot_index_label.text = str(slot_index + 1)
	if stack == null:
		icon.visible = false
		return
		
	var def = database.get_item(stack.item_id)
	if def == null:
		icon.visible = false
		return
		
	icon.visible = true
	icon.texture = def.icon
