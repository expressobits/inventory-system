@tool
extends Control


func load_recipes(recipes : Array):
	for recipe in recipes:
		print(recipe.product.item.name)
		for ingredient in recipe.ingredients:
			print("-",ingredient.item.name)
