@tool
extends Panel
class_name GridSlotUI

enum StylePriority {HIGH = 0, MEDIUM = 1, LOW = 2}

@export var regular_style: StyleBox
@export var hover_style: StyleBox
@export var selected_style: StyleBox
var _styles: Array[StyleBox] = [null, null, null]
@onready var audio_stream_player: AudioStreamPlayer = $AudioStreamPlayer

@onready var category_icon: TextureRect = %CategoryIcon
var _grid_position: Vector2i
var inventory: GridInventory


var item_dragged: bool = false:
	set(value):
		pass
		#print("item_grabbed",value)
		
		
func setup(grid_position: Vector2i, new_inventory: GridInventory):
	self._grid_position = grid_position
	self.inventory = new_inventory


func select():
	if is_instance_valid(selected_style):
		_set_panel_style(selected_style)


func unselect():
	_set_panel_style(regular_style)


func _ready() -> void:
	_set_panel_style(regular_style)
	mouse_entered.connect(func():
		_set_panel_style(hover_style)
		audio_stream_player.play()
	)
	mouse_exited.connect(func():
		_set_panel_style(regular_style)
	)
	if inventory == null:
		return
	for i in inventory.grid_constraints:
		if i is CategoryGridInventoryConstraint:
			var category: String = i.get_category(_grid_position)
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
