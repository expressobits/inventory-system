@tool
extends Panel
class_name GridSlotUI

enum StylePriority {HIGH = 0, MEDIUM = 1, LOW = 2}

@export var regular_style: StyleBox
@export var hover_style: StyleBox
var _styles: Array[StyleBox] = [null, null, null]

@onready var category_icon: TextureRect = %CategoryIcon
var grid_position: Vector2i
var inventory: GridInventory


var item_selected: bool = false:
	set(value):
		pass
		#print("item_selected", value)


var item_dragged: bool = false:
	set(value):
		pass
		#print("item_grabbed",value)
		
		
func setup(grid_position: Vector2i, inventory: GridInventory):
	self.grid_position = grid_position
	self.inventory = inventory


func _ready() -> void:
	_set_panel_style(regular_style)
	mouse_entered.connect(func():
		_set_panel_style(hover_style)
	)
	mouse_exited.connect(func():
		_set_panel_style(regular_style)
	)
	if inventory == null:
		return
	for i in inventory.grid_constraints:
		if i is CategoryGridInventoryConstraint:
			var category: String = i.get_category(grid_position)
			if category != "" and inventory.database != null:
				var cat: ItemCategory = inventory.database.get_category_from_id(category)
				if cat != null:
					category_icon.visible = true
					category_icon.texture = cat.icon
					self_modulate = cat.color
					return
	self_modulate = Color.WHITE
	category_icon.visible = false


func set_style(style: StyleBox, priority: int = StylePriority.LOW) -> void:
	if priority > 2 || priority < 0:
		return
	if _styles[priority] == style:
		return

	#_styles[priority] = style
#
	#for i in range(0, 3):
		#if _styles[i] != null or i == 2:
			#_set_panel_style(_styles[i])
			#return


func _set_panel_style(style: StyleBox) -> void:
	remove_theme_stylebox_override("panel")
	if style != null:
		add_theme_stylebox_override("panel", style)
