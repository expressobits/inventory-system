@tool
extends HBoxContainer
class_name SlotSelector

signal slot_changed(slot : Slot)

@onready var product_id_spin_box = $ProductIDSpinBox
@onready var option_button = $OptionButton
@onready var product_amount_spin_box = $ProductAmountSpinBox


var slot : Slot
var database : InventoryDatabase
var ids_list : Array[int]

# Called when the node enters the scene tree for the first time.
func _ready():		
	product_id_spin_box.value_changed.connect(_on_product_id_spin_box_value_changed.bind())
	option_button.item_selected.connect(_on_option_button_item_selected.bind())
	product_amount_spin_box.value_changed.connect(_on_product_amount_spin_box_value_changed.bind())


func setup(slot : Slot, database : InventoryDatabase):
	self.slot = slot
	self.database = database
	product_id_spin_box.value = slot.id
	product_amount_spin_box.value = slot.amount
	ids_list.clear()
	option_button.clear()

	for i in database.items.size():
		var item_database = database.items[i]
		option_button.add_icon_item(item_database.item.icon ,item_database.item.name)
		ids_list.append(item_database.id)
		if item_database.id == slot.id:
			option_button.select(i)


func _on_product_id_spin_box_value_changed(value):
	slot.id = int(value)
	var index = ids_list.find(slot.id )
	if index != -1:
		if option_button.selected != index:
			option_button.select(index)
	emit_signal("slot_changed", slot)


func _on_product_amount_spin_box_value_changed(value):
	slot.amount = int(value)
	emit_signal("slot_changed", slot)


func _on_option_button_item_selected(index):
	var id : int = ids_list[index]
	if product_id_spin_box.value != id:
		product_id_spin_box.value = id
