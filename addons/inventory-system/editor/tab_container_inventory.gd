@tool
extends TabContainer

@export var items_icon : Texture2D
@export var recipes_icon : Texture2D
@export var craft_station_types_icon : Texture2D
@export var categories_icon : Texture2D

func _ready():
#	set_tab_button_icon(0, items_icon)
#	set_tab_button_icon(1, recipes_icon)
#	set_tab_button_icon(2, craft_station_types_icon)
#	set_tab_button_icon(3, categories_icon)
	get_tab_control(0).mouse_filter = Control.MOUSE_FILTER_PASS
	get_tab_control(1).mouse_filter = Control.MOUSE_FILTER_PASS
	get_tab_control(2).mouse_filter = Control.MOUSE_FILTER_PASS
	get_tab_control(3).mouse_filter = Control.MOUSE_FILTER_PASS
