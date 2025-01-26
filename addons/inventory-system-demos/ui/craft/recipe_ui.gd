class_name RecipeUI
extends Control

## Represents a [Recipe] in the interface. It is instantiated by [CraftStationUI]

# Scene to instantiate the [IngredientUI]
@export var ingredient_scene : PackedScene

@onready var icon : TextureRect = get_node("Control/ItemIcon")
@onready var item_name : Label = get_node("ItemName")
@onready var time_to_craft : Label = get_node("TimeToCraft")
@onready var craft_button : Button = get_node("CraftButton")
@onready var ingredients_container : HBoxContainer = get_node("IngredientsItemsList")
@onready var required_items_list = %RequiredItemsList


var recipe_index : int
var craft_station : CraftStation
var _ingredients : Array[IngredientUI]

## Configures the recipe with index values from [Recipe] from [InventoryDatabase].
## Called by [CraftStationUI].
func set_recipe(_craft_station : CraftStation, _recipe : Recipe, _recipe_index : int):
	self.craft_station = _craft_station
	self.recipe_index = _recipe_index
	var product_item : ItemDefinition = craft_station.database.get_item(_recipe.products[0].item_id)
	icon.texture = product_item.icon
	item_name.text = product_item.name
	time_to_craft.text = str(_recipe.time_to_craft) + " Seconds"
	_clear_ingredients()
	for ingredient in _recipe.ingredients:
		var ingredient_obj : IngredientUI = ingredient_scene.instantiate()
		ingredients_container.add_child(ingredient_obj)
		ingredient_obj.setup(craft_station.database, ingredient)
		_ingredients.append(ingredient_obj)
	for ingredient in _recipe.required_items:
		var ingredient_obj : IngredientUI = ingredient_scene.instantiate()
		ingredients_container.add_child(ingredient_obj)
		ingredient_obj.setup(craft_station.database, ingredient)
		_ingredients.append(ingredient_obj)
	check_if_has_ingredients()


func _clear_ingredients():
	for ingredient_ui in _ingredients:
		ingredient_ui.queue_free()
		_ingredients.clear()		


func check_if_has_ingredients():
	var recipe = craft_station.database.recipes[recipe_index]
	craft_button.disabled = not craft_station.can_craft(recipe)
