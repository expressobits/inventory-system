#include "grid_inventory.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

bool GridInventory::_bounds_broken() const {
	for (size_t i = 0; i < get_stacks().size(); i++) {
		Ref<ItemStack> stack = get_stacks()[i];
		if (!rect_free(get_stack_rect(stack), stack))
			return true;
	}
	return false;
}

void GridInventory::_refresh_quad_tree() {
	Ref<QuadTree> new_quad_tree = memnew(QuadTree());
	new_quad_tree->init(size);
	set_quad_tree(new_quad_tree);
	for (size_t i = 0; i < get_stacks().size(); i++) {
		Ref<ItemStack> stack = get_stacks()[i];
		quad_tree->add(get_stack_rect(stack), stack);
	}
}

void GridInventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_size", "size"), &GridInventory::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &GridInventory::get_size);
	ClassDB::bind_method(D_METHOD("set_grid_constraints", "grid_constraints"), &GridInventory::set_grid_constraints);
	ClassDB::bind_method(D_METHOD("get_grid_constraints"), &GridInventory::get_grid_constraints);
	ClassDB::bind_method(D_METHOD("set_quad_tree", "quad_tree"), &GridInventory::set_quad_tree);
	ClassDB::bind_method(D_METHOD("get_quad_tree"), &GridInventory::get_quad_tree);
	ClassDB::bind_method(D_METHOD("set_stack_positions", "stack_positions"), &GridInventory::set_stack_positions);
	ClassDB::bind_method(D_METHOD("get_stack_positions"), &GridInventory::get_stack_positions);
	ClassDB::bind_method(D_METHOD("set_stack_rotations", "stack_rotations"), &GridInventory::set_stack_rotations);
	ClassDB::bind_method(D_METHOD("get_stack_rotations"), &GridInventory::get_stack_rotations);

	ClassDB::bind_method(D_METHOD("get_stack_position", "stack"), &GridInventory::get_stack_position);
	ClassDB::bind_method(D_METHOD("set_stack_position", "stack", "new_position"), &GridInventory::set_stack_position);
	ClassDB::bind_method(D_METHOD("get_stack_size", "stack"), &GridInventory::get_stack_size);
	ClassDB::bind_method(D_METHOD("get_stack_rect", "stack"), &GridInventory::get_stack_rect);
	ClassDB::bind_method(D_METHOD("is_stack_rotated", "stack"), &GridInventory::is_stack_rotated);
	ClassDB::bind_method(D_METHOD("rotate", "stack"), &GridInventory::rotate);
	ClassDB::bind_method(D_METHOD("can_rotate_item", "stack"), &GridInventory::can_rotate_item);

	ClassDB::bind_method(D_METHOD("get_stack_at", "position"), &GridInventory::get_stack_at);
	ClassDB::bind_method(D_METHOD("get_stack_index_at", "position"), &GridInventory::get_stack_index_at);
	ClassDB::bind_method(D_METHOD("add_at_position", "position", "item_id", "amount", "properties", "is_rotated"), &GridInventory::add_at_position, DEFVAL(1), DEFVAL(Dictionary()), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_stacks_under", "rect"), &GridInventory::get_stacks_under);
	// ClassDB::bind_method(D_METHOD("move_stack_to", "stack", "position"), &GridInventory::move_stack_to);
	ClassDB::bind_method(D_METHOD("transfer_to", "from_position", "destination", "destination_position", "amount", "is_rotated"), &GridInventory::transfer_to, DEFVAL(1), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("swap_stacks", "position", "other_inventory", "other_position"), &GridInventory::swap_stacks);
	ClassDB::bind_method(D_METHOD("rect_free", "rect", "exception"), &GridInventory::rect_free, DEFVAL(nullptr));
	// ClassDB::bind_method(D_METHOD("find_free_place", "stack_size", "exception"), &GridInventory::find_free_place, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("sort"), &GridInventory::sort);

	ADD_SIGNAL(MethodInfo("size_changed"));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "grid_constraints", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "GridInventoryConstraint")), "set_grid_constraints", "get_grid_constraints");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "stack_positions"), "set_stack_positions", "get_stack_positions");
	// ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "quad_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_quad_tree", "get_quad_tree");
}

GridInventory::GridInventory() {
}

GridInventory::~GridInventory() {
}

void GridInventory::set_size(const Vector2i &new_size) {
	if (new_size.x <= 0)
		return;
	if (new_size.y <= 0)
		return;
	Vector2i old_size = size;
	size = new_size;
	if (!Engine::get_singleton()->is_editor_hint()) {
		if (_bounds_broken())
			size = old_size;
	}
	if (size != old_size) {
		_refresh_quad_tree();
		emit_signal("size_changed");
	}
}

Vector2i GridInventory::get_size() const {
	return size;
}

void GridInventory::set_grid_constraints(const TypedArray<GridInventoryConstraint> &new_grid_constraints) {
	grid_constraints = new_grid_constraints;
}

TypedArray<GridInventoryConstraint> GridInventory::get_grid_constraints() const {
	return grid_constraints;
}

void GridInventory::set_quad_tree(const Ref<QuadTree> &new_quad_tree) {
	quad_tree = new_quad_tree;
}

Ref<QuadTree> GridInventory::get_quad_tree() const {
	return quad_tree;
}

void GridInventory::set_stack_positions(const TypedArray<Vector2i> &new_stack_positions) {
	stack_positions = new_stack_positions;
}

TypedArray<Vector2i> GridInventory::get_stack_positions() const {
	return stack_positions;
}

void GridInventory::set_stack_rotations(const TypedArray<bool> &new_stack_rotations) {
	stack_rotations = new_stack_rotations;
}

TypedArray<bool> GridInventory::get_stack_rotations() const {
	return stack_rotations;
}

Vector2i GridInventory::get_stack_position(const Ref<ItemStack> &stack) const {
	ERR_FAIL_NULL_V_MSG(stack, Vector2i(0, 0), "stack' is null.");

	int stack_index = stacks.find(stack);
	if (stack_index == -1)
		return Vector2i(0, 0);

	ERR_FAIL_COND_V_MSG(stack_index >= stack_positions.size(), Vector2i(0, 0), "stack_index' is out of bounds for stack_positions.");
	return stack_positions[stack_index];
}

bool GridInventory::set_stack_position(const Ref<ItemStack> &stack, const Vector2i new_position) {
	Rect2i new_rect = Rect2i(new_position, get_stack_size(stack));
	if (has_stack(stack) && !rect_free(new_rect, stack))
		return false;

	int stack_index = stacks.find(stack);
	if (stack_index == -1)
		return false;
	stack_positions[stack_index] = new_position;
	return true;
}

bool GridInventory::can_rotate_item(const Ref<ItemStack> &stack) const {
	Rect2i rotated_rect = get_stack_rect(stack);
	int temp = rotated_rect.size.x;
	rotated_rect.size.x = rotated_rect.size.y;
	rotated_rect.size.y = temp;
	return rect_free(rotated_rect, stack);
}

void GridInventory::rotate(const Ref<ItemStack> &stack) {
	Vector2i size = get_stack_size(stack);
	if (size.x == size.y)
		return;
	Vector2i item_pos = get_stack_position(stack);
	bool is_rotated = is_stack_rotated(stack);
	transfer_to(item_pos, this, item_pos, stack->get_amount(), !is_rotated);
}

bool GridInventory::is_stack_rotated(const Ref<ItemStack> &stack) const {
	ERR_FAIL_NULL_V_MSG(stack, false, "stack' is null.");

	int stack_index = stacks.find(stack);
	if (stack_index == -1)
		return false;

	ERR_FAIL_COND_V_MSG(stack_index >= stack_rotations.size(), false, "stack_index' is out of bounds for stack_rotations.");
	return stack_rotations[stack_index];
}

Vector2i GridInventory::get_stack_size(const Ref<ItemStack> &stack) const {
	bool is_rotated = is_stack_rotated(stack);
	Ref<ItemDefinition> definition = get_database()->get_item(stack->get_item_id());
	if (definition == nullptr)
		return Vector2i();
	Vector2i size = definition->get_size();
	if (is_rotated) {
		size = Vector2i(size.y, size.x);
	}
	return size;
}

Rect2i GridInventory::get_stack_rect(const Ref<ItemStack> &stack) const {
	Vector2i item_pos = get_stack_position(stack);
	Vector2i item_size = get_stack_size(stack);
	return Rect2i(item_pos, item_size);
}

Ref<ItemStack> GridInventory::get_stack_at(const Vector2i position) const {
	Ref<QuadTree::QuadRect> first = quad_tree->get_first(position);
	if (first == nullptr)
		return nullptr;
	return first->get_metadata();
}

int GridInventory::get_stack_index_at(const Vector2i position) const {
	Ref<ItemStack> stack = get_stack_at(position);
	if (stack == nullptr)
		return -1;
	int stack_index = stacks.find(stack);
	return stack_index;
}

TypedArray<ItemStack> GridInventory::get_stacks_under(const Rect2i rect) const {
	TypedArray<ItemStack> result = TypedArray<ItemStack>();
	for (size_t i = 0; i < stacks.size(); i++) {
		Ref<ItemStack> stack = stacks[i];
		Rect2i stack_rect = get_stack_rect(stack);
		if (stack_rect.intersects(rect))
			result.append(stack);
	}
	return result;
}

int GridInventory::add_at_position(const Vector2i position, const String item_id, const int amount, const Dictionary &properties, const bool is_rotated) {
	int stack_index = get_stack_index_at(position);
	if (stack_index == -1) {
		Ref<ItemDefinition> definition = get_database()->get_item(item_id);
		Vector2i size;
		if (is_rotated) {
			size = definition->get_rotated_size();
		} else {
			size = definition->get_size();
		}
		Rect2i rect = Rect2i(position, size);
		if (rect_free(rect) && _can_add_on_position(position, item_id, amount, properties, is_rotated)) {
			int no_added = add_on_new_stack(item_id, amount, properties);
			if (no_added == amount)
				return amount;
			Ref<ItemStack> stack = stacks[stacks.size() - 1];
			stack_rotations[stacks.size() - 1] = is_rotated;
			bool move_success = move_stack_to(stack, position);
			if (!move_success)
				UtilityFunctions::printerr("Can't move the item to the given place!");
			return no_added;
		}
	} else {
		Ref<ItemStack> stack = stacks[stack_index];
		if (is_rotated != is_stack_rotated(stack))
			return amount;
		int no_added = add_at_index(stack_index, item_id, amount, properties);
		return no_added;
	}
	return amount;
}

bool GridInventory::move_stack_to(const Ref<ItemStack> stack, const Vector2i position) {
	Vector2i stack_size = get_stack_size(stack);
	Rect2i rect = Rect2i(position, stack_size);
	if (rect_free(rect, stack)) {
		_move_stack_to_unsafe(stack, position);
		_flag_contents_changed = true;
		return true;
	}
	return false;
}

int GridInventory::transfer_to(const Vector2i from_position, GridInventory *destination, const Vector2i destination_position, const int &amount, const bool is_rotated) {
	ERR_FAIL_COND_V_MSG(from_position.x < 0 || from_position.x >= size.x, amount, "from_position.x' is out of size grid bounds.");
	ERR_FAIL_COND_V_MSG(from_position.y < 0 || from_position.y >= size.y, amount, "from_position.x' is out of size grid bounds.");
	ERR_FAIL_NULL_V_MSG(destination, amount, "Destination inventory is null on transfer.");
	ERR_FAIL_NULL_V_MSG(get_database(), amount, "InventoryDatabase is null.");
	ERR_FAIL_NULL_V_MSG(destination->get_database(), amount, "InventoryDatabase is null.");
	ERR_FAIL_COND_V_MSG(get_database() != destination->get_database(), amount, "Operation between inventories that do not have the same database is invalid.");
	ERR_FAIL_COND_V_MSG(amount < 0, amount, "The 'amount' is negative.");

	Ref<ItemStack> stack = get_stack_at(from_position);
	if (stack == nullptr)
		return amount;

	bool is_rotated_on_origin_position = is_stack_rotated(stack);

	if (this == destination && from_position == destination_position && is_rotated_on_origin_position == is_rotated)
		return amount;

	int amount_of_stack = stack->get_amount();
	int stack_index = stacks.find(stack);
	ERR_FAIL_COND_V_MSG(stack_index < 0 || stack_index >= stacks.size(), amount, "The 'stack index' is out of bounds.");

	String item_id = stack->get_item_id();
	Dictionary properties = stack->get_properties();
	int amount_to_interact = amount;
	if (amount_to_interact == 0)
		return amount;

	// TODO implement left on destination for swap amount only
	// Ref<ItemDefinition> destination_definition = get_database()->get_item(destination_stack->get_item_id());
	// ERR_FAIL_NULL_V_MSG(destination_definition, amount, "Destination item_definition is null on transfer.");
	// int amount_to_left = destination_definition->get_max_stack() - destination_stack->get_amount();
	// if (amount_to_left > -1) {
	// 	amount_to_interact = MIN(amount_to_interact, amount_to_left);
	// }

	int amount_not_transferred = 0;

	if (amount == amount_of_stack && swap_stacks(from_position, destination, destination_position)) {
		amount_not_transferred = 0;
	} else {
		int amount_not_removed = remove_at(stack_index, item_id, amount_to_interact);
		int amount_to_transfer = amount_to_interact - amount_not_removed;
		if (amount_to_transfer == 0)
			return amount;

		int amount_not_transferred = destination->add_at_position(destination_position, item_id, amount_to_transfer, properties, is_rotated);

		if (amount_not_transferred > 0)
			amount_not_transferred = add_at_position(from_position, item_id, amount_not_transferred, properties, is_rotated_on_origin_position);
	}

	if (amount_not_transferred != amount) {
		_flag_contents_changed = true;
		if (this != destination) {
			destination->_flag_contents_changed = true;
		}
	}

	return amount_not_transferred;
}

bool GridInventory::swap_stacks(const Vector2i position, GridInventory *other_inventory, const Vector2i other_position) {
	Ref<ItemStack> stack = get_stack_at(position);
	if (stack == nullptr)
		return false;
	Ref<ItemStack> other_stack = other_inventory->get_stack_at(other_position);
	if (other_stack == nullptr)
		return false;
	if (stack == other_stack)
		return false;
	Vector2i real_other_position = other_inventory->get_stack_position(other_stack);
	if (!_size_check(stack, other_stack))
		return false;
	int stack_index = stacks.find(stack);
	if (stack_index == -1)
		return false;

	String stack_item_id = stack->get_item_id();
	String other_stack_item_id = other_stack->get_item_id();

	if (stack_item_id == other_stack_item_id)
		return false;
	int stack_amount = stack->get_amount();
	int other_stack_amount = other_stack->get_amount();
	Dictionary stack_properties = stack->get_properties();
	Dictionary other_stack_properties = other_stack->get_properties();
	bool stack_rotation = false;
	bool other_stack_rotation = false;

	int other_stack_index = other_inventory->stacks.find(other_stack);
	if (other_stack_index == -1)
		return false;

	if (!_can_swap_to_inventory(this, other_stack_item_id, other_stack_amount, other_stack_properties))
		return false;

	if (!_can_swap_to_inventory(other_inventory, stack_item_id, stack_amount, stack_properties))
		return false;

	if (!_can_add_on_position(position, other_stack_item_id, other_stack_amount, other_stack_properties, other_stack_rotation))
		return false;

	if (!other_inventory->_can_add_on_position(real_other_position, stack_item_id, stack_amount, stack_properties, stack_rotation))
		return false;

	remove_at(stack_index, stack_item_id, stack_amount);
	other_inventory->remove_at(other_stack_index, other_stack_item_id, other_stack_amount);

	add_at_position(position, other_stack_item_id, other_stack_amount, other_stack_properties);
	other_inventory->add_at_position(real_other_position, stack_item_id, stack_amount, stack_properties);

	return true;
}

bool GridInventory::rect_free(const Rect2i &rect, const Ref<ItemStack> &exception) const {
	if (rect.position.x < 0 || rect.position.y < 0 || rect.size.x < 1 || rect.size.y < 1)
		return false;
	if (rect.position.x + rect.size.x > size.x)
		return false;
	if (rect.position.y + rect.size.y > size.y)
		return false;

	ERR_FAIL_NULL_V_MSG(quad_tree, 0, "'quad_tree' is null.");
	return quad_tree->get_first(rect, exception) == nullptr;
}

Vector2i GridInventory::find_free_place(const Vector2i item_size, const String item_id, const int amount, const Dictionary properties, const bool is_rotated, const Ref<ItemStack> &exception) const {
	Vector2i result = Vector2i(-1, -1);
	Vector2i final_size = item_size;
	if (is_rotated) {
		final_size = Vector2i(final_size.y, final_size.x);
	}
	for (size_t y = 0; y < (size.y - (final_size.y - 1)); y++) {
		for (size_t x = 0; x < (size.x - (final_size.x - 1)); x++) {
			Rect2i rect = Rect2i(Vector2i(x, y), final_size);
			if (rect_free(rect, exception) && _can_add_on_position(Vector2i(x, y), item_id, amount, properties, is_rotated)) {
				return Vector2i(x, y);
			}
		}
	}
	return result;
}

bool GridInventory::sort() {
	// TypedArray<ItemStack> stack_array;
	// for (size_t i = 0; i < stacks.size(); i++) {
	// 	Ref<ItemStack> stack = stacks[i];
	// 	stack_array.append(stack);
	// 	stack_array.sort_custom(callable_mp(this, &GridInventory::_compare_stacks));
	// }

	// for (size_t i = 0; i < stack_array.size(); i++) {
	// 	Ref<ItemStack> stack = stack_array[i];
	// 	_move_stack_to_unsafe(stack, -get_stack_size(stack));
	// }

	// for (size_t i = 0; i < stack_array.size(); i++) {
	// 	Ref<ItemStack> stack = stack_array[i];
	// 	Vector2i free_place = find_free_place(get_stack_size(stack), stack->get_item_id(), stack->get_amount(), stack->get_properties());
	// 	if (free_place == Vector2i(-1, -1))
	// 		return false;
	// 	move_stack_to(stack, free_place);
	// }
	return true;
}

Dictionary GridInventory::serialize() const {
	Dictionary data = Inventory::serialize();
	data["stack_positions"] = stack_positions.duplicate();
	data["stack_rotations"] = stack_rotations.duplicate();
	return data;
}

void GridInventory::deserialize(const Dictionary data) {
	stack_positions = data["stack_positions"];
	stack_rotations = data["stack_rotations"];
	Inventory::deserialize(data);
	for (size_t i = 0; i < stack_positions.size(); i++) {
		Ref<ItemStack> stack = stacks[i];
		quad_tree->add(get_stack_rect(stack), stack);
	}
}

bool GridInventory::can_add_new_stack(const String &item_id, const int &amount, const Dictionary &properties) const {
	return (has_space_for(item_id, amount, properties, false) || has_space_for(item_id, amount, properties, true)) && Inventory::can_add_new_stack(item_id, amount, properties);
}

bool GridInventory::has_space_for(const String &item_id, const int amount, const Dictionary &properties, const bool is_rotated) const {
	Ref<ItemDefinition> definition = get_database()->get_item(item_id);
	ERR_FAIL_NULL_V_MSG(definition, false, "'definition' is null.");

	// if (Inventory::can_stack_with_actual_slots(item_id, amount, properties))
	// 	return true;

	Vector2i item_size = definition->get_size();
	Vector2i result = find_free_place(item_size, item_id, amount, properties, is_rotated);
	return result != Vector2i(-1, -1);
}

void GridInventory::on_insert_stack(const int stack_index) {
	Ref<ItemStack> stack = stacks[stack_index];
	if (stack == nullptr)
		return;
	ERR_FAIL_NULL_MSG(quad_tree, "'quad_tree' is null.");
	ERR_FAIL_NULL_MSG(get_database(), "'database' is null.");
	Ref<ItemDefinition> definition = get_database()->get_item(stack->get_item_id());
	ERR_FAIL_NULL_MSG(definition, "'definition' is null.");
	bool is_rotated = false;
	Vector2i position;
	position = find_free_place(definition->get_size(), stack->get_item_id(), stack->get_amount(), stack->get_properties(), is_rotated);
	if (position == Vector2i(-1, -1)) {
		is_rotated = true;
		position = find_free_place(definition->get_size(), stack->get_item_id(), stack->get_amount(), stack->get_properties(), true);
	}
	stack_positions.insert(stack_index, position);
	stack_rotations.insert(stack_index, is_rotated);
	Vector2i size;
	if (is_rotated) {
		size = definition->get_rotated_size();
	} else {
		size = definition->get_size();
	}
	quad_tree->add(Rect2i(position, size), stack);
}

void GridInventory::on_removed_stack(const Ref<ItemStack> stack, const int stack_index) {
	stack_positions.remove_at(stack_index);
	stack_rotations.remove_at(stack_index);
	if (stack == nullptr)
		return;
	quad_tree->remove(stack);
}

bool GridInventory::_size_check(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2) {
	return get_stack_size(stack1) == get_stack_size(stack2);
}

bool GridInventory::_is_sorted() {
	for (size_t x = 0; x < stacks.size(); x++) {
		Ref<ItemStack> stack1 = stacks[x];
		for (size_t y = 0; y < stacks.size(); y++) {
			Ref<ItemStack> stack2 = stacks[y];
			if (stack1 == stack2)
				continue;
			Rect2i rect1 = get_stack_rect(stack1);
			Rect2i rect2 = get_stack_rect(stack2);
			if (rect1.intersects(rect2))
				return false;
		}
	}
	return true;
}

void GridInventory::_move_stack_to_unsafe(const Ref<ItemStack> &stack, const Vector2i &position) {
	int stack_index = stacks.find(stack);
	if (stack_index == -1)
		return;
	stack_positions[stack_index] = position;
	quad_tree->remove(stack);
	quad_tree->add(get_stack_rect(stack), stack);
}

bool GridInventory::_compare_stacks(const Ref<ItemStack> &stack1, const Ref<ItemStack> &stack2) const {
	Rect2i rect1 = Rect2i(get_stack_position(stack1), get_stack_size(stack1));
	Rect2i rect2 = Rect2i(get_stack_position(stack2), get_stack_size(stack2));
	return rect1.get_area() > rect2.get_area();
}

void GridInventory::_sort_if_needed() {
	if (!_is_sorted() || _bounds_broken())
		sort();
}

bool GridInventory::_can_add_on_position(const Vector2i position, const String item_id, const int amount, const Dictionary properties, const bool is_rotated) const {
	for (size_t i = 0; i < grid_constraints.size(); i++) {
		Ref<GridInventoryConstraint> grid_constraint = grid_constraints[i];
		if (grid_constraint != nullptr && !grid_constraint->can_add_on_position(this, position, item_id, amount, properties, is_rotated)) {
			return false;
		}
	}
	return true;
}
