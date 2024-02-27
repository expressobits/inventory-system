@tool
extends Node

const DefaultActions = preload("res://addons/inventory-system-demos/base/default_actions.gd")

const ACTIONS = [
	DefaultActions.CHARACTER_CONTROLLER_FORWARD,
	DefaultActions.CHARACTER_CONTROLLER_BACKWARD,
	DefaultActions.CHARACTER_CONTROLLER_RIGHT,
	DefaultActions.CHARACTER_CONTROLLER_LEFT,
	DefaultActions.CHARACTER_CONTROLLER_ESCAPE,
	DefaultActions.CHARACTER_CONTROLLER_INTERACT,
	DefaultActions.CHARACTER_CONTROLLER_TOGGLE_CRAFT_PANEL,
	DefaultActions.CHARACTER_CONTROLLER_TOGGLE_INVENTORY,
	DefaultActions.CHARACTER_CONTROLLER_ADD_ITEM_A,
	DefaultActions.CHARACTER_CONTROLLER_REMOVE_ITEM_A,
	DefaultActions.CHARACTER_CONTROLLER_ADD_ITEM_B,
	DefaultActions.CHARACTER_CONTROLLER_REMOVE_ITEM_B,
	DefaultActions.CHARACTER_CONTROLLER_ADD_ITEM_C,
	DefaultActions.CHARACTER_CONTROLLER_REMOVE_ITEM_C,
]

func _enter_tree():
	# Register input events
	var save := false
	for action_props in ACTIONS:
		var setting_name = "input/" + action_props["name"]

		if not ProjectSettings.has_setting(setting_name):
			print("Set config:"+setting_name)
			save = true
			var events = []

			var action_props_events = action_props["events"]

			for event_data in action_props_events:
				var event = InputEventKey.new()
				for prop_name in event_data:
					event.set(prop_name, event_data[prop_name])

				events.append(event)

			ProjectSettings.set_setting(setting_name, {
				"deadzone": float(action_props["deadzone"] if "deadzone" in action_props else 0.5),
				"events": events
			})
	if save:
		var result = ProjectSettings.save()
		assert(result == OK, "Failed to save project settings")
