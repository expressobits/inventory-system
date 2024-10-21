#include "grid_inventory.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

bool GridInventory::_bounds_broken() const {
	for (size_t i = 0; i < get_items().size(); i++) {
		Ref<ItemStack> stack = get_items()[i];
		if (!rect_free(get_stack_rect(stack), stack))
			return true;
	}
	return false;
}

void GridInventory::_refresh_quad_tree() {
	_quad_tree = memnew(Ref<QuadTree>(size));
	for (size_t i = 0; i < get_items().size(); i++) {
		Ref<ItemStack> stack = get_items()[i];
		_quad_tree->add(get_stack_rect(stack), stack);
	}
}

void GridInventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_size", "size"), &GridInventory::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &GridInventory::get_size);

	ADD_SIGNAL(MethodInfo("size_changed"));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
}

GridInventory::GridInventory() {
}

GridInventory::~GridInventory() {
	// inventory->connect("item_added", callable_mp(this, &CraftStation::_on_input_inventory_item_added));
	// inventory->connect("item_removed", callable_mp(this, &CraftStation::_on_input_inventory_item_removed));
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

Vector2i GridInventory::get_item_position(const Ref<ItemStack> &stack) const {
	int stack_index = items.find(stack);
	if (stack_index == -1)
		return Vector2i();
	return stack_positions[stack_index];
}

bool GridInventory::set_item_position(const Ref<ItemStack> &stack, const Vector2i new_position) {
	Rect2i new_rect = Rect2i(new_position, get_stack_size(stack));
	if (has_stack(stack) && !rect_free(new_rect, stack))
		return false;

	int stack_index = items.find(stack);
	if (stack_index == -1)
		return false;
	stack_positions[stack_index] = new_position;
	return true;
}

Vector2i GridInventory::get_stack_size(const Ref<ItemStack> &stack) const {
	// TODO implement rotation
	Ref<ItemDefinition> definition = get_database()->get_item(stack->get_item_id());
	if (definition == nullptr)
		return Vector2i();
	return definition->get_size();
}

Rect2i GridInventory::get_stack_rect(const Ref<ItemStack> &stack) const {
	Vector2i item_pos = get_item_position(stack);
	Vector2i item_size = get_stack_size(stack);
	return Rect2i(item_pos, item_size);
}

bool GridInventory::set_stack_rect(const Ref<ItemStack> &stack, const Rect2i &new_rect) {
	if (!rect_free(new_rect, stack))
		return false;
	if (!set_item_position(stack, new_rect.position))
		return false;
	// if !set_item_size(item, new_rect.size):
	//     return false;
	return true;
}

bool GridInventory::set_stack_rotation(const Ref<ItemStack> &stack) {
	// if is_item_rotated(item) == rotated:
	//     return false
	// if !can_rotate_item(item):
	//     return false

	// if rotated:
	//     item.set_property(KEY_ROTATED, true)
	// else:
	//     item.clear_property(KEY_ROTATED)
	//
	// return true
	return false;
}

bool GridInventory::is_stack_rotated(const Ref<ItemStack> &stack) const {
	// return item.get_property(KEY_ROTATED, false)
	return false;
}

bool GridInventory::is_stack_rotation_positive(const Ref<ItemStack> &stack) const {
	// return item.get_property(KEY_POSITIVE_ROTATION, false)
	return false;
}

void GridInventory::set_item_rotation_direction(const Ref<ItemStack> &stack, const bool positive) {
	// if positive:
	//     item.set_property(KEY_POSITIVE_ROTATION, true)
	// else:
	//     item.clear_property(KEY_POSITIVE_ROTATION)
}

bool GridInventory::rotate_stack(const Ref<ItemStack> &stack) {
	// return set_item_rotation(item, !is_item_rotated(item))
	return false;
}

bool GridInventory::is_rotate_item(const Ref<ItemStack> &stack) const {
	// return item.get_property(KEY_ROTATED, false)
	return false;
}

bool GridInventory::can_rotate_item(const Ref<ItemStack> &stack) const {
	// var rotated_rect := get_item_rect(item)
	// var temp := rotated_rect.size.x
	// rotated_rect.size.x = rotated_rect.size.y
	// rotated_rect.size.y = temp
	// return rect_free(rotated_rect, item)
	return false;
}

Ref<ItemStack> GridInventory::get_stack_at(const Vector2i position) const {
	Ref<QuadTree::QuadRect> first = _quad_tree->get_first(position);
	if (first == nullptr)
		return nullptr;
	return first->metadata;
}

TypedArray<ItemStack> GridInventory::get_stacks_under(const Rect2i rect) const {
	TypedArray<ItemStack> result = TypedArray<ItemStack>();
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemStack> stack = items[i];
		Ref<Rect2i> stack_rect = get_stack_rect(stack);
		if (stack_rect->intersects(rect))
			result.append(stack);
	}
	return result;
}

int GridInventory::add_at(const Vector2i position, const String item_id, const int amount, const Dictionary &properties) {
	// _insert_new_stack(item_id, amount, properties);

	// TODO fix link here
	Ref<ItemStack> stack = items[0];
	Vector2i item_size = get_stack_size(stack);
	Rect2i rect = Rect2i(position, item_size);
	if (rect_free(rect)) {
		if (!add(item_id, amount, properties))
			return false;
		bool move_success = move_stack_to(stack, position);
		UtilityFunctions::printerr(move_success, "Can't move the item to the given place!");
		return true;
	}
	// return false;
	return 0;
}

bool GridInventory::move_stack_to(const Ref<ItemStack> stack, const Vector2i position) {
	Vector2i stack_size = get_stack_size(stack);
	Rect2i rect = Rect2i(position, stack_size);
	if (rect_free(rect, stack)) {
		_move_item_to_unsafe(stack, position);
		emit_signal("contents_changed");
		return true;
	}
	return false;
}

bool GridInventory::move_item_to_free_spot(const Ref<ItemStack> stack) {
	if (rect_free(get_stack_rect(stack), stack))
		return true;

	Vector2i free_place = find_free_place(stack, stack);
	if (free_place == Vector2i(-1, -1))
		return false;

	_move_item_to_unsafe(stack, free_place);
	return true;
}

bool GridInventory::transfer_to(const Vector2i from_position, const GridInventory *destination, const Vector2i destination_position) {
	// ERR_FAIL_NULL_V_MSG(destination, false, "Destination inventory is null!");
	// Ref<ItemStack> from_stack = get_stack_at(from_position);
	// Vector2i item_size = get_stack_size(from_stack);
	// Rect2i rect = Rect2i(destination_position, item_size);
	// if (destination->rect_free(rect) && destination->add_at(from_stack, destination_position))
	// 	return true;

	// if (_merge_to(stack, destination, destination_position))
	// 	return true;

	// Ref<ItemStack> item_dst = destination->get_item_at(destination_position);
	// if (item_dst != nullptr && !StacksConstraint.items_mergable(item_dst, item))
	// 	return InventoryItem.swap(item, item_dst) return false;
}

bool GridInventory::rect_free(const Rect2i &rect, const Ref<ItemStack> &exception) const {
	if (rect.position.x < 0 || rect.position.y < 0 || rect.size.x < 1 || rect.size.y < 1)
		return false;
	if (rect.position.x + rect.size.x > size.x)
		return false;
	if (rect.position.y + rect.size.y > size.y)
		return false;

	return _quad_tree->get_first(rect, exception) == nullptr;
}

Vector2i GridInventory::find_free_place(const Ref<ItemStack> &stack, const Ref<ItemStack> &exception) const {
	Vector2i result = Vector2i(-1, -1);
	Vector2i item_size = get_stack_size(stack);
	for (size_t x = 0; x < (size.x - (item_size.x - 1)); x++) {
		for (size_t y = 0; y < (size.y - (item_size.y - 1)); y++) {
			Rect2i rect = Rect2i(Vector2i(x, y), item_size);
			if (rect_free(rect, exception))
				result = Vector2i(x, y);
			return result;
		}
	}
	return result;
}

bool GridInventory::sort() {
	TypedArray<ItemStack> stack_array;
	for (size_t i = 0; i < items.size(); i++) {
		Ref<ItemStack> stack = items[i];
		stack_array.append(stack);
		stack_array.sort_custom(callable_mp(this, &GridInventory::_compare_stacks));
	}

	for (size_t i = 0; i < stack_array.size(); i++) {
		Ref<ItemStack> stack = stack_array[i];
		_move_item_to_unsafe(stack, -get_stack_size(stack));
	}

	for (size_t i = 0; i < stack_array.size(); i++) {
		Ref<ItemStack> stack = stack_array[i];
		Vector2i free_place = find_free_place(stack);
		if (free_place == Vector2i(-1, -1))
			return false;
		move_stack_to(stack, free_place);
	}
	return true;
}

void GridInventory::on_insert_stack(const int stack_index) {
	Ref<ItemStack> stack = items[stack_index];
	if (stack == nullptr)
		return;
	_quad_tree->add(get_stack_rect(stack), stack);
}

void GridInventory::on_removed_stack(const Ref<ItemStack> stack, const int stack_index) {
	if (stack == nullptr)
		return;
	_quad_tree->remove(stack);
}

void GridInventory::_on_item_grid_info_changed(const Ref<ItemStack> stack) {
	if (stack == nullptr)
		return;
	_quad_tree->remove(stack);
	_quad_tree->add(get_stack_rect(stack), stack);
}

bool GridInventory::_on_pre_item_swap(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2) {
	if (!_size_check(stack1, stack2))
		return false;

	if (has_stack(stack1))
		_swap_position = get_item_position(stack1);
	else if (has_stack(stack2))
		_swap_position = get_item_position(stack2);
	return true;
}

bool GridInventory::_size_check(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2) {
	return get_stack_size(stack1) == get_stack_size(stack2);
}

void GridInventory::_on_post_item_swap(const Ref<ItemStack> stack1, const Ref<ItemStack> stack2) {
	bool has1 = has_stack(stack1);
	bool has2 = has_stack(stack2);
	if (has1 && has2) {
		Vector2i temp_pos = get_item_position(stack1);
		_move_item_to_unsafe(stack1, get_item_position(stack2));
		_move_item_to_unsafe(stack2, temp_pos);
	} else if (has1)
		move_stack_to(stack1, _swap_position);
	else if (has2)
		move_stack_to(stack2, _swap_position);
}

bool GridInventory::_is_sorted() {
	for (size_t x = 0; x < items.size(); x++) {
		Ref<ItemStack> stack1 = items[x];
		for (size_t y = 0; y < items.size(); y++) {
			Ref<ItemStack> stack2 = items[y];
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

void GridInventory::_move_item_to_unsafe(const Ref<ItemStack> &stack, const Vector2i &position) {
	int stack_index = items.find(stack);
	if (stack_index == -1)
		return;
	stack_positions[stack_index] = position;
}

bool GridInventory::_compare_stacks(const Ref<ItemStack> &stack1, const Ref<ItemStack> &stack2) const {
	Rect2i rect1 = Rect2i(get_item_position(stack1), get_stack_size(stack1));
	Rect2i rect2 = Rect2i(get_item_position(stack2), get_stack_size(stack2));
	return rect1.get_area() > rect2.get_area();
}

void GridInventory::_sort_if_needed() {
	if (!_is_sorted() || _bounds_broken())
		sort();
}

void QuadTree::QuadRect::_init(const Rect2i &rect, const Variant &metadata) {
	this->rect = rect;
	this->metadata = metadata;
}

String QuadTree::QuadRect::to_string() const {
	return "[R: " + UtilityFunctions::str(rect) + ", M: " + UtilityFunctions::str(metadata) + "]";
}

TypedArray<Rect2i> QuadTree::QuadNode::_get_quadrant_rects(const Rect2i &rect) {
	TypedArray<Rect2i> result = TypedArray<Rect2i>();
	int64_t q0w = UtilityFunctions::roundi(float(rect.size.x) / 2.0);
	int64_t q0h = UtilityFunctions::roundi(float(rect.size.y) / 2.0);
	Rect2i q0 = Rect2i(rect.position, Vector2i(q0w, q0h));
	Rect2i q3 = Rect2i(rect.position + q0.size, rect.size - q0.size);
	Rect2i q1 = Rect2i(Vector2i(q3.position.x, q0.position.y), Vector2i(q3.size.x, q0.size.y));
	Rect2i q2 = Rect2i(Vector2i(q0.position.x, q3.position.y), Vector2i(q0.size.x, q3.size.y));
	result.append(q0);
	result.append(q1);
	result.append(q2);
	result.append(q3);
	return result;
}

QuadTree::QuadNode::QuadNode() {
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
}

QuadTree::QuadNode::~QuadNode() {
}

void QuadTree::QuadNode::_init(const Rect2i &rect) {
	this->rect = rect;
}

String QuadTree::QuadNode::to_string() const {
	return "[R: " + UtilityFunctions::str(rect) + "]";
}

bool QuadTree::QuadNode::is_empty() const {
	return quadrant_count == 0 && quad_rects.is_empty();
}

Ref<QuadTree::QuadRect> QuadTree::QuadNode::get_first_under_rect(const Rect2i &test_rect, const Variant &exception_metadata) const {
	for (size_t i = 0; i < quad_rects.size(); i++) {
		Ref<QuadRect> quad_rect = quad_rects[i];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->metadata == exception_metadata)
			continue;
		if (quad_rect->rect.intersects(test_rect))
			return quad_rect;
	}
	for (size_t quadrant_index = 0; quadrant_index < quadrants.size(); quadrant_index++) {
		Ref<QuadNode> quadrant = quadrants[quadrant_index];
		if (quadrant == nullptr)
			continue;
		if (!quadrant->rect.intersects(test_rect))
			continue;
		Ref<QuadRect> first = quadrant->get_first_under_rect(test_rect, exception_metadata);
		if (first != nullptr)
			return first;
	}
	return nullptr;
}

Ref<QuadTree::QuadRect> QuadTree::QuadNode::get_first_containing_point(const Vector2i &point, const Variant &exception_metadata) const {
	for (size_t quad_rect_index = 0; quad_rect_index < quad_rects.size(); quad_rect_index++) {
		Ref<QuadRect> quad_rect = quad_rects[quad_rect_index];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->metadata == exception_metadata)
			continue;
		if (quad_rect->rect.has_point(point))
			return quad_rect;
	}

	for (size_t quadrant_index = 0; quadrant_index < quadrants.size(); quadrant_index++) {
		Ref<QuadNode> quadrant = quadrants[quadrant_index];
		if (quadrant == nullptr)
			continue;
		if (!quadrant->rect.has_point(point))
			continue;
		Ref<QuadRect> first = quadrant->get_first_containing_point(point, exception_metadata);
		if (first != nullptr)
			return first;
	}

	return nullptr;
}

TypedArray<QuadTree::QuadRect> QuadTree::QuadNode::get_all_under_rect(const Rect2i &test_rect, const Variant &exception_metadata) const {
	TypedArray<QuadRect> result = TypedArray<QuadRect>();
	for (size_t quad_rect_index = 0; quad_rect_index < quad_rects.size(); quad_rect_index++) {
		Ref<QuadRect> quad_rect = quad_rects[quad_rect_index];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->metadata == exception_metadata)
			continue;
		if (quad_rect->rect.intersects(test_rect))
			result.append(result);
	}
	for (size_t quadrant_index = 0; quadrant_index < quadrants.size(); quadrant_index++) {
		Ref<QuadNode> quadrant = quadrants[quadrant_index];
		if (quadrant == nullptr)
			continue;
		if (!quadrant->rect.intersects(test_rect))
			continue;
		result.append_array(quadrant->get_all_under_rect(test_rect, exception_metadata));
	}
	return result;
}

TypedArray<QuadTree::QuadRect> QuadTree::QuadNode::get_all_containing_point(const Vector2i &point, const Variant &exception_metadata) const {
	TypedArray<QuadRect> result = TypedArray<QuadRect>();
	for (size_t quad_rect_index = 0; quad_rect_index < quad_rects.size(); quad_rect_index++) {
		Ref<QuadRect> quad_rect = quad_rects[quad_rect_index];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->metadata == exception_metadata)
			continue;
		if (quad_rect->rect.has_point(point))
			result.append(result);
	}
	for (size_t quadrant_index = 0; quadrant_index < quadrants.size(); quadrant_index++) {
		Ref<QuadNode> quadrant = quadrants[quadrant_index];
		if (quadrant == nullptr)
			continue;
		if (!quadrant->rect.has_point(point))
			continue;
		result.append_array(quadrant->get_all_containing_point(point, exception_metadata));
	}
	return result;
}

void QuadTree::QuadNode::add(const Ref<QuadTree::QuadRect> &quad_rect) {
	if (!_can_subdivide(rect.size)) {
		quad_rects.append(quad_rect);
		return;
	}
	if (is_empty()) {
		quad_rects.append(quad_rect);
		return;
	}

	TypedArray<Rect2i> quadrant_rects = _get_quadrant_rects(rect);
	for (size_t i = 0; i < quadrant_rects.size(); i++) {
		Rect2i quadrant_rect = quadrant_rects[i];
		if (!quadrant_rect.intersects(quad_rect->rect))
			continue;
		Ref<QuadNode> quadrant = quadrants[i];
		if (quadrant == nullptr) {
			quadrants[i] = memnew(QuadNode());
			quadrant_count += 1;
			while (!quad_rects.is_empty()) {
				Ref<QuadTree::QuadRect> new_quad_rect = quad_rects.pop_back();
				add(new_quad_rect);
			}
		}
		quadrant->add(quad_rect);
	}
}

bool QuadTree::QuadNode::remove(const Variant &metadata) {
	bool result = false;
	for (size_t i = quad_rects.size() - 1; i >= 0; i--) {
		Ref<QuadRect> quad_rect = quad_rects[i];
		if (quad_rect->metadata == metadata) {
			quad_rects.remove_at(i);
			result = true;
		}
	}

	for (size_t i = 0; i < quadrants.size(); i++) {
		Ref<QuadNode> quad_node = quadrants[i];
		if (quad_node == nullptr)
			continue;
		if (quad_node->remove(metadata))
			result = true;
		if (quad_node->is_empty()) {
			quadrants[i] = nullptr;
			quadrant_count -= 1;
		}
	}

	_collapse();

	return result;
}

void QuadTree::QuadNode::_collapse() {
	if (quadrant_count == 0)
		return;
	Ref<QuadRect> collapsing_into = nullptr;
	for (size_t node_index = 0; node_index < quadrants.size(); node_index++) {
		Ref<QuadNode> quad_node = quadrants[node_index];
		if (quad_node == nullptr)
			continue;
		if (quad_node->quadrant_count != 0)
			return;
		for (size_t quad_rect_index = 0; quad_rect_index < quad_node->quad_rects.size(); quad_rect_index++) {
			Ref<QuadRect> qtr = quad_node->quad_rects[quad_rect_index];
			if (collapsing_into != nullptr && collapsing_into != qtr)
				return;
			collapsing_into = qtr;
		}
	}
	for (size_t node_index = 0; node_index < quadrants.size(); node_index++) {
		quadrants[node_index] = nullptr;
	}
	quadrant_count = 0;
	quad_rects.append(collapsing_into);
}

void QuadTree::_init(const Vector2i &size) {
	this->size = size;
	Ref<QuadTree::QuadNode> new_node = memnew(Ref<QuadTree::QuadNode>());
	new_node->_init(Rect2i(Vector2i(0, 0), this->size));
	this->_root = new_node;
}

Ref<QuadTree::QuadRect> QuadTree::get_first(const Variant &at, const Variant &exception_metadata) const {
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_RECT2I)
		return this->_root->get_first_under_rect(at, exception_metadata);
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_VECTOR2I)
		return this->_root->get_first_containing_point(at, exception_metadata);
	return nullptr;
}

TypedArray<QuadTree::QuadRect> QuadTree::get_all(const Variant &at, const Variant &exception_metadata) const {
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_RECT2I)
		return this->_root->get_all_under_rect(at, exception_metadata);
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_VECTOR2I)
		return this->_root->get_all_containing_point(at, exception_metadata);
	return TypedArray<QuadTree::QuadRect>();
}

void QuadTree::add(const Rect2i &rect, const Variant &metadata) {
	Ref<QuadTree::QuadRect> new_quad_rect = memnew(Ref<QuadTree::QuadRect>());
	new_quad_rect->_init(rect, metadata);
	_root->add(new_quad_rect);
}

bool QuadTree::remove(const Variant &metadata) {
	return _root->remove(metadata);
}

bool QuadTree::is_empty() const {
	return _root->is_empty();
}
