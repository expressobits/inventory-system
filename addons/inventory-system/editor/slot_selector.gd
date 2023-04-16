@tool
extends HBoxContainer
class_name SlotSelector

signal slot_changed(slot : Slot)

@onready var product_id_spin_box = $ProductIDSpinBox
@onready var option_button = $OptionButton
@onready var product_amount_spin_box = $ProductAmountSpinBox


var slot : Slot:
	set(new_slot):
		slot = new_slot
	get:
		return slot

var database : InventoryDatabase
var ids_list : Array[InventoryItem]

# Called when the node enters the scene tree for the first time.
func _ready():		
	product_id_spin_box.value_changed.connect(_on_product_id_spin_box_value_changed.bind())
	option_button.item_selected.connect(_on_option_button_item_selected.bind())
	product_amount_spin_box.value_changed.connect(_on_product_amount_spin_box_value_changed.bind())


func setup(slot : Slot, database : InventoryDatabase):
	self.slot = slot
	self.database = database
	var id = slot.item.id
	product_id_spin_box.value = id
	product_amount_spin_box.value = slot.amount
	ids_list.clear()
	option_button.clear()

	for i in database.items.size():
		var item = database.items[i]
		option_button.add_icon_item(item.icon ,item.name)
		ids_list.append(item)
		if item == slot.item:
			option_button.select(i)


func _on_product_id_spin_box_value_changed(value):
	var item = database.get_item(int(value))
	if item == null:
		item = ids_list[option_button.selected]
		product_amount_spin_box.value = item.id
	slot.item = item
	var index = ids_list.find(slot.item)
	if index != -1:
		if option_button.selected != index:
			_on_option_button_item_selected(index)
			option_button.selected = index


func _on_product_amount_spin_box_value_changed(value):
	slot.amount = int(value)
	emit_signal("slot_changed", slot)


func _on_option_button_item_selected(index):
	var item : InventoryItem = ids_list[index]
	if product_id_spin_box.value != item.id:
		product_id_spin_box.value = item.id
	emit_signal("slot_changed", slot)	
