extends Control
class_name RecipeUI

## Represents a [Recipe] in the interface. It is instantiated by [CraftStationUI]

# Scene to instantiate the [IngredientUI]
@export var ingredient_scene : PackedScene

@onready var icon : TextureRect = get_node("Control/ItemIcon")
@onready var item_name : Label = get_node("ItemName")
@onready var time_to_craft : Label = get_node("TimeToCraft")
@onready var craft_button : Button = get_node("CraftButton")
@onready var ingredients_container : HBoxContainer = get_node("IngredientsItemsList")
@onready var required_items_list = %RequiredItemsList


var _recipe_index : int
var _craft_station : CraftStation
var _ingredients : Array[IngredientUI]

func _ready():
	craft_button.button_down.connect(_on_craft_button_button_down.bind())


## Configures the recipe with index values from [Recipe] from [InventoryDatabase].
## Called by [CraftStationUI].
func set_recipe(craft_station : CraftStation, recipe : Recipe, recipe_index : int):
	self._craft_station = craft_station
	self._recipe_index = recipe_index
	icon.texture = recipe.products[0].item.definition.icon
	item_name.text = recipe.products[0].item.definition.name
	time_to_craft.text = str(recipe.time_to_craft) + " Seconds"
	_clear_ingredients()
	for ingredient in recipe.ingredients:
		var ingredient_obj : IngredientUI = ingredient_scene.instantiate()
		ingredients_container.add_child(ingredient_obj)
		ingredient_obj.setup(ingredient)
		_ingredients.append(ingredient_obj)
	for ingredient in recipe.required_items:
		var ingredient_obj : IngredientUI = ingredient_scene.instantiate()
		ingredients_container.add_child(ingredient_obj)
		ingredient_obj.setup(ingredient)
		_ingredients.append(ingredient_obj)
	_check_if_has_ingredients()
	for input_inventory in craft_station.input_inventories:
		input_inventory.updated_slot.connect(_on_updated_slot.bind())


func _clear_ingredients():
	for ingredient_ui in _ingredients:
		ingredient_ui.queue_free()
		_ingredients.clear()		


func _on_craft_button_button_down():
	_craft_station.craft(_recipe_index)


func _on_updated_slot(slot_index : int):
	_check_if_has_ingredients()


func _on_removed_item(item : InventoryItem, amount : int):
	_check_if_has_ingredients()


func _check_if_has_ingredients():
	var recipe = _craft_station.database.recipes[_recipe_index]
	craft_button.disabled = not _craft_station.can_craft(recipe)
