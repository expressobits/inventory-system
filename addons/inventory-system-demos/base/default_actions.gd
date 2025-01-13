const CHARACTER_CONTROLLER_FORWARD := {
	"name": "forward_move",
	"events": [
		{
			"physical_keycode": KEY_W,
		}
	]
}

const CHARACTER_CONTROLLER_BACKWARD := {
	"name": "back_move",
	"events": [
		{
			"physical_keycode": KEY_S,
		}
	]
}
const CHARACTER_CONTROLLER_RIGHT := {
	"name": "right_move",
	"events": [
		{
			"physical_keycode": KEY_D,
		}
	]
}

const CHARACTER_CONTROLLER_LEFT := {
	"name": "left_move",
	"events": [
		{
			"physical_keycode": KEY_A,
		}
	]
}

const CHARACTER_CONTROLLER_JUMP := {
	"name": "jump_move",
	"events": [
		{
			"physical_keycode": KEY_SPACE,
		}
	]
}

const CHARACTER_CONTROLLER_ESCAPE := {
	"name": "escape",
	"events": [
		{
			"physical_keycode": KEY_ESCAPE,
		}
	]
}

const CHARACTER_CONTROLLER_INTERACT := {
	"name": "interact",
	"events": [
		{
			"physical_keycode": KEY_E,
		}
	]
}

const CHARACTER_CONTROLLER_HAND_INTERACT := {
	"name": "hand_interact",
	"event_type": "mouse_button",
	"events": [
		{
			"button_index": 1,
		}
	]
}

const CHARACTER_CONTROLLER_TOGGLE_CRAFT_PANEL := {
	"name": "toggle_craft_panel",
	"events": [
		{
			"physical_keycode": KEY_C,
		}
	]
}

const CHARACTER_CONTROLLER_TOGGLE_INVENTORY := {
	"name": "toggle_inventory",
	"events": [
		{
			"physical_keycode": KEY_I,
		}
	]
}

const CHARACTER_CONTROLLER_PICK_ITEM := {
	"name": "item_pickup",
	"events": [
		{
			"physical_keycode": KEY_E,
		}
	]
}

const CHARACTER_CONTROLLER_PLACE_ITEM := {
	"name": "item_place",
	"event_type": "mouse_button",
	"events": [
		{
			"button_index": 2,
		}
	]
}

const CHARACTER_CONTROLLER_ROTATE := {
	"name": "rotate",
	"events": [
		{
			"physical_keycode": KEY_R,
		}
	]
}

const CHARACTER_CONTROLLER_ADD_ITEM_A := {
	"name": "add_item_a",
	"events": [
		{
			"physical_keycode": KEY_F1,
		}
	]
}

const CHARACTER_CONTROLLER_REMOVE_ITEM_A := {
	"name": "remove_item_a",
	"events": [
		{
			"physical_keycode": KEY_F2,
		}
	]
}

const CHARACTER_CONTROLLER_ADD_ITEM_B := {
	"name": "add_item_b",
	"events": [
		{
			"physical_keycode": KEY_F3,
		}
	]
}

const CHARACTER_CONTROLLER_REMOVE_ITEM_B := {
	"name": "remove_item_b",
	"events": [
		{
			"physical_keycode": KEY_F4,
		}
	]
}

const CHARACTER_CONTROLLER_ADD_ITEM_C := {
	"name": "add_item_c",
	"events": [
		{
			"physical_keycode": KEY_F5,
		}
	]
}

const CHARACTER_CONTROLLER_REMOVE_ITEM_C := {
	"name": "remove_item_c",
	"events": [
		{
			"physical_keycode": KEY_F6,
		}
	]
}

const UI_INVENTORY_TRANSFER := {
	"name": "ui_inventory_transfer",
	"events": [
		{
			"physical_keycode": KEY_SHIFT,
		}
	]
}
