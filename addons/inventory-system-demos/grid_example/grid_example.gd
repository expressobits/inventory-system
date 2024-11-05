extends Node

const info_offset: Vector2 = Vector2(20, 0)

@onready var ctrl_inventory_left := %GridInventoryUILeft
@onready var ctrl_inventory_right := %GridInventoryUIRight
@onready var btn_sort_left: Button = %ButtonSortLeft
@onready var btn_sort_right: Button = %ButtonSortRight
@onready var btn_split_left: Button = %ButtonSplitLeft
@onready var btn_split_right: Button = %ButtonSplitRight
#@onready var ctrl_slot: CtrlItemSlot = $"%CtrlItemSlot"
#@onready var btn_unequip: Button = $"%BtnUnequip"
@onready var lbl_info: Label = %LblInfo
@onready var control: Control = $Control



func _ready() -> void:
	ctrl_inventory_left.item_mouse_entered.connect(_on_item_mouse_entered)
	ctrl_inventory_left.item_mouse_exited.connect(_on_item_mouse_exited)
	ctrl_inventory_right.item_mouse_entered.connect(_on_item_mouse_entered)
	ctrl_inventory_right.item_mouse_exited.connect(_on_item_mouse_exited)
	btn_sort_left.pressed.connect(_on_btn_sort.bind(ctrl_inventory_left))
	btn_sort_right.pressed.connect(_on_btn_sort.bind(ctrl_inventory_right))
	btn_split_left.pressed.connect(_on_btn_split.bind(ctrl_inventory_left))
	btn_split_right.pressed.connect(_on_btn_split.bind(ctrl_inventory_right))
	#btn_unequip.pressed.connect(_on_btn_unequip)
	
	await get_tree().create_timer(1).timeout
	
	ctrl_inventory_left.inventory.add("wood", 16)
	await get_tree().create_timer(1).timeout
	ctrl_inventory_left.inventory.add("stone_axe")
	await get_tree().create_timer(1).timeout
	ctrl_inventory_left.inventory.add("stone_axe")
	await get_tree().create_timer(1).timeout
	ctrl_inventory_left.inventory.add("campfire")
	await get_tree().create_timer(1).timeout
	ctrl_inventory_left.inventory.add("workbench")


func _on_item_mouse_entered(item: ItemStack) -> void:
	lbl_info.show()
	lbl_info.text = item.item_id


func _on_item_mouse_exited(_item: ItemStack) -> void:
	lbl_info.hide()


func _input(event: InputEvent) -> void:
	if !(event is InputEventMouseMotion):
		return

	lbl_info.set_global_position(control.get_global_mouse_position() + info_offset)


func _can_drop_data(_at_position: Vector2, _data: Variant) -> bool:
	return true


func _drop_data(_at_position: Vector2, data: Variant) -> void:
	ctrl_inventory_left.inventory.remove_item(data.item)
	ctrl_inventory_right.inventory.remove_item(data.item)
	# Replace the following line with custom logic for handling the item drop:
	data.item.queue_free()


func _on_btn_sort(ctrl_inventory) -> void:
	if !ctrl_inventory.inventory.sort():
		print("Warning: InventoryGrid.sort() returned false!")


func _on_btn_split(ctrl_inventory) -> void:
	var selected_items = ctrl_inventory.get_selected_inventory_items()
	if selected_items.is_empty():
		return

	for selected_item in selected_items:
		var stack_size : int = selected_item.amount
		if stack_size < 2:
			return
		
		var stack_index = ctrl_inventory.inventory.items.find(selected_item)
		
		# All this floor/float jazz just to do integer division without warnings
		var new_stack_size: int = floor(float(stack_size) / 2)
		ctrl_inventory.inventory.split(stack_index, new_stack_size)


#func _on_btn_unequip() -> void:
	#ctrl_slot.item_slot.clear()
