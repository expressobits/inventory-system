#include "quad_tree.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void QuadTree::QuadRect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_rect", "rect"), &QuadTree::QuadRect::set_rect);
	ClassDB::bind_method(D_METHOD("get_rect"), &QuadTree::QuadRect::get_rect);
	ClassDB::bind_method(D_METHOD("set_metadata", "metadata"), &QuadTree::QuadRect::set_metadata);
	ClassDB::bind_method(D_METHOD("get_metadata"), &QuadTree::QuadRect::get_metadata);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rect"), "set_rect", "get_rect");
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "metadata", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_metadata", "get_metadata");
}

void QuadTree::QuadRect::set_rect(const Rect2i &new_rect) {
    rect = new_rect;
}

Rect2i QuadTree::QuadRect::get_rect() const {
	return rect;
}

void QuadTree::QuadRect::set_metadata(const Variant &new_metadata) {
    metadata = new_metadata;
}

Variant QuadTree::QuadRect::get_metadata() const {
	return metadata;
}

void QuadTree::QuadRect::_init(const Rect2i &rect, const Variant &metadata) {
	this->rect = rect;
	this->metadata = metadata;
}

String QuadTree::QuadRect::to_string() const {
	return "[R: " + UtilityFunctions::str(rect) + ", M: " + UtilityFunctions::str(metadata) + "]";
}

void QuadTree::QuadNode::_bind_methods() {
	ClassDB::bind_static_method("QuadNode", D_METHOD("can_subdivide", "size"), &QuadTree::QuadNode::can_subdivide);
	ClassDB::bind_static_method("QuadNode", D_METHOD("get_quadrant_rects", "rect"), &QuadTree::QuadNode::get_quadrant_rects);

	ClassDB::bind_method(D_METHOD("is_empty"), &QuadTree::QuadNode::is_empty);
	ClassDB::bind_method(D_METHOD("get_first_under_rect", "test_rect", "exception_metadata"), &QuadTree::QuadNode::get_first_under_rect, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_first_containing_point", "point", "exception_metadata"), &QuadTree::QuadNode::get_first_containing_point, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_all_under_rect", "test_rect", "exception_metadata"), &QuadTree::QuadNode::get_all_under_rect, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_all_containing_point", "point", "exception_metadata"), &QuadTree::QuadNode::get_all_containing_point, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("add", "quad_rect"), &QuadTree::QuadNode::add);
	ClassDB::bind_method(D_METHOD("remove", "metadata"), &QuadTree::QuadNode::remove);
	ClassDB::bind_method(D_METHOD("_collapse"), &QuadTree::QuadNode::_collapse);

    ClassDB::bind_method(D_METHOD("set_quadrants", "quadrants"), &QuadTree::QuadNode::set_quadrants);
	ClassDB::bind_method(D_METHOD("get_quadrants"), &QuadTree::QuadNode::get_quadrants);
	ClassDB::bind_method(D_METHOD("set_quadrant_count", "quadrant_count"), &QuadTree::QuadNode::set_quadrant_count);
	ClassDB::bind_method(D_METHOD("get_quadrant_count"), &QuadTree::QuadNode::get_quadrant_count);
	ClassDB::bind_method(D_METHOD("set_quad_rects", "quad_rects"), &QuadTree::QuadNode::set_quad_rects);
	ClassDB::bind_method(D_METHOD("get_quad_rects"), &QuadTree::QuadNode::get_quad_rects);
	ClassDB::bind_method(D_METHOD("set_rect", "rect"), &QuadTree::QuadNode::set_rect);
	ClassDB::bind_method(D_METHOD("get_rect"), &QuadTree::QuadNode::get_rect);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "quadrants", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_quadrants", "get_quadrants");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "quadrant_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_quadrant_count", "get_quadrant_count");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "quad_rects", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_quad_rects", "get_quad_rects");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rect", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_rect", "get_rect");
}

QuadTree::QuadNode::QuadNode() {
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
	quadrants.push_back(nullptr);
}

QuadTree::QuadNode::~QuadNode() {
}

bool QuadTree::QuadNode::can_subdivide(const Vector2i &size) {
	return size.x > 1 && size.y > 1;
}

TypedArray<Rect2i> QuadTree::QuadNode::get_quadrant_rects(const Rect2i &rect) {
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

void QuadTree::QuadNode::_init(const Rect2i &rect) {
	this->rect = rect;
}

void QuadTree::QuadNode::set_quadrants(const TypedArray<QuadNode> &new_quadrants) {
    quadrants = new_quadrants;
}

TypedArray<QuadTree::QuadNode> QuadTree::QuadNode::get_quadrants() const {
	return quadrants;
}

void QuadTree::QuadNode::set_quadrant_count(const int &new_quadrant_count) {
    quadrant_count = new_quadrant_count;
}

int QuadTree::QuadNode::get_quadrant_count() const {
	return quadrant_count;
}

void QuadTree::QuadNode::set_quad_rects(const TypedArray<QuadRect> &new_quad_rects) {
    quad_rects = new_quad_rects;
}

TypedArray<QuadTree::QuadRect> QuadTree::QuadNode::get_quad_rects() const {
	return quad_rects;
}

void QuadTree::QuadNode::set_rect(const Rect2i &new_rect) {
    rect = new_rect;
}

Rect2i QuadTree::QuadNode::get_rect() const {
	return rect;
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
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->get_metadata() == exception_metadata)
			continue;
		if (quad_rect->get_rect().intersects(test_rect))
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
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->get_metadata() == exception_metadata)
			continue;
		if (quad_rect->get_rect().has_point(point))
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

Array QuadTree::QuadNode::get_all_under_rect(const Rect2i &test_rect, const Variant &exception_metadata) const {
	Array result = Array();
	for (size_t quad_rect_index = 0; quad_rect_index < quad_rects.size(); quad_rect_index++) {
		Ref<QuadRect> quad_rect = quad_rects[quad_rect_index];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->get_metadata() == exception_metadata)
			continue;
		if (quad_rect->get_rect().intersects(test_rect))
			result.append(quad_rect);
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

Array QuadTree::QuadNode::get_all_containing_point(const Vector2i &point, const Variant &exception_metadata) const {
	Array result = Array();
	for (size_t quad_rect_index = 0; quad_rect_index < quad_rects.size(); quad_rect_index++) {
		Ref<QuadRect> quad_rect = quad_rects[quad_rect_index];
		if (UtilityFunctions::type_of(exception_metadata) != GDEXTENSION_VARIANT_TYPE_NIL && quad_rect->get_metadata() == exception_metadata)
			continue;
		if (quad_rect->get_rect().has_point(point))
			result.append(quad_rect);
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
	if (!can_subdivide(rect.size)) {
		quad_rects.append(quad_rect);
		return;
	}
	if (is_empty()) {
		quad_rects.append(quad_rect);
		return;
	}

	TypedArray<Rect2i> quadrant_rects = get_quadrant_rects(rect);
	for (size_t i = 0; i < quadrant_rects.size(); i++) {
		Rect2i quadrant_rect = quadrant_rects[i];
		if (!quadrant_rect.intersects(quad_rect->get_rect()))
			continue;
		Ref<QuadTree::QuadNode> quadrant = quadrants[i];
		if (quadrant == nullptr) {
			quadrants[i] = memnew(QuadNode());
            quadrant = quadrants[i];
            quadrant->set_rect(quadrant_rect);
			quadrant_count += 1;
			while (!quad_rects.is_empty()) {
				Ref<QuadTree::QuadRect> new_quad_rect = quad_rects.pop_back();
				add(new_quad_rect);
			}
		}
        ERR_FAIL_NULL_MSG(quadrant, "'quadrant' is null.");
		quadrant->add(quad_rect);
	}
}

bool QuadTree::QuadNode::remove(const Variant &metadata) {
	bool result = false;
	for (int i = (quad_rects.size() - 1); i >= 0; i--) {
        ERR_FAIL_COND_V_MSG(i < 0 || i >= quad_rects.size(), result, "'quad_rects_index' is out of bounds.");
		Ref<QuadRect> quad_rect = quad_rects[i];
        ERR_FAIL_NULL_V_MSG(quad_rect, result, "'quad_rect' is null.");
		if (quad_rect->get_metadata() == metadata) {
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

void QuadTree::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_first", "at", "exception_metadata"), &QuadTree::get_first, DEFVAL(nullptr));
    ClassDB::bind_method(D_METHOD("get_all", "at", "exception_metadata"), &QuadTree::get_all, DEFVAL(nullptr));
    ClassDB::bind_method(D_METHOD("add", "rect", "metadata"), &QuadTree::add);
    ClassDB::bind_method(D_METHOD("remove", "metadata"), &QuadTree::remove);
    ClassDB::bind_method(D_METHOD("is_empty"), &QuadTree::is_empty);

	ClassDB::bind_method(D_METHOD("set_root", "root"), &QuadTree::set_root);
	ClassDB::bind_method(D_METHOD("get_root"), &QuadTree::get_root);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &QuadTree::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &QuadTree::get_size);

    ClassDB::bind_method(D_METHOD("init", "size"), &QuadTree::init);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "root", PROPERTY_HINT_RESOURCE_TYPE, "QuadNode", PROPERTY_USAGE_NO_EDITOR), "set_root", "get_root");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
}

QuadTree::QuadTree() {
}

QuadTree::~QuadTree() {
}

void QuadTree::init(const Vector2i &size) {
	this->size = size;
	Ref<QuadTree::QuadNode> new_node = memnew(QuadTree::QuadNode());
    ERR_FAIL_NULL_MSG(new_node, "'new_node' is null.");
	new_node->_init(Rect2i(Vector2i(0, 0), this->size));
	set_root(new_node);
}

Ref<QuadTree::QuadRect> QuadTree::get_first(const Variant &at, const Variant &exception_metadata) const {
	ERR_FAIL_NULL_V_MSG(this->root, nullptr, "'root' is null.");
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_RECT2I)
		return this->root->get_first_under_rect(at, exception_metadata);
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_VECTOR2I)
		return this->root->get_first_containing_point(at, exception_metadata);
	return nullptr;
}

Array QuadTree::get_all(const Variant &at, const Variant &exception_metadata) const {
	ERR_FAIL_NULL_V_MSG(this->root, TypedArray<QuadTree::QuadRect>(), "'root' is null.");
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_RECT2I)
		return this->root->get_all_under_rect(at, exception_metadata);
	if (UtilityFunctions::type_of(at) == GDEXTENSION_VARIANT_TYPE_VECTOR2I)
		return this->root->get_all_containing_point(at, exception_metadata);
	return TypedArray<QuadTree::QuadRect>();
}

void QuadTree::add(const Rect2i &rect, const Variant &metadata) {
    ERR_FAIL_NULL_MSG(root, "'root node' is null.");
	Ref<QuadTree::QuadRect> new_quad_rect = memnew(QuadTree::QuadRect());
	new_quad_rect->_init(rect, metadata);
	root->add(new_quad_rect);
}

bool QuadTree::remove(const Variant &metadata) {
    ERR_FAIL_NULL_V_MSG(root, false, "'root node' is null.");
	return root->remove(metadata);
}

bool QuadTree::is_empty() const {
    ERR_FAIL_NULL_V_MSG(root, true, "'root node' is null.");
	return root->is_empty();
}

void QuadTree::set_root(const Ref<QuadNode> &new_root) {
	root = new_root;
}

Ref<QuadTree::QuadNode> QuadTree::get_root() const {
	return root;
}

void QuadTree::set_size(const Vector2i &new_size) {
	size = new_size;
}

Vector2i QuadTree::get_size() const {
	return size;
}