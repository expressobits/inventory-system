class_name RecipeUI
extends Control

## Represents a [Recipe] in the interface. It is instantiated by [CraftStationUI]

signal on_craft(station : CraftStation, recipe_index : int)

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
func set_recipe(craft_station : CraftStation, recipe : Recipe, recipe_index : int):
	self.craft_station = craft_station
	self.recipe_index = recipe_index
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
	for i in craft_station.input_inventories.size():
		craft_station.get_input_inventory(i).updated_slot.connect(_on_updated_slot.bind())


func _clear_ingredients():
	for ingredient_ui in _ingredients:
		ingredient_ui.queue_free()
		_ingredients.clear()		


func _on_updated_slot(slot_index : int):
	_check_if_has_ingredients()


func _on_removed_item(item : ItemDefinition, amount : int):
	_check_if_has_ingredients()


func _check_if_has_ingredients():
	var recipe = craft_station.database.recipes[recipe_index]
	craft_button.disabled = not craft_station.can_craft(recipe)
