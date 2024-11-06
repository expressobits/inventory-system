extends Node

const info_offset: Vector2 = Vector2(20, 0)

@onready var ctrl_inventory_left := %GridInventoryUILeft
@onready var ctrl_inventory_right := %GridInventoryUIRight
@onready var btn_sort_left: Button = %ButtonSortLeft
@onready var btn_sort_right: Button = %ButtonSortRight
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
	#btn_unequip.pressed.connect(_on_btn_unequip)
	
	await get_tree().create_timer(0.2).timeout
	
	ctrl_inventory_left.inventory.add("wood", 16)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("stone_axe")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("stone_pickaxe")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_left.inventory.add("campfire", 2)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("workbench")
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("stone", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("grass", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("raw_meat", 8)
	await get_tree().create_timer(0.2).timeout
	ctrl_inventory_right.inventory.add("cooked_meat", 8)

#func _process(delta):
	#print(ctrl_inventory_left.inventory.items.size())
	#print(ctrl_inventory_left.inventory.stack_positions)

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
