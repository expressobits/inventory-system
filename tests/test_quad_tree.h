/**
 * test_quad_tree.h
 * =============================================================================
 * Copyright (c) 2023-present ExpressoBits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#ifndef TEST_QUAD_TREE_H
#define TEST_QUAD_TREE_H

#include "tests.h"
#include "../src/core/quad_tree.h"

using namespace godot;

class TestQuadTree : public InventoryTestBase {
public:
	virtual const char* get_test_name() const override {
		return "Quad Tree Tests";
	}

	virtual bool run_tests() override {
		log_test_start("Quad Tree Tests");
		
		bool all_passed = true;
		all_passed &= can_subdivide_test();
		all_passed &= get_quadrant_rects_test();
		all_passed &= constructor_test();
		all_passed &= add_test();
		all_passed &= remove_test();
		all_passed &= get_first_test();
		all_passed &= get_all_test();

		log_test_end("Quad Tree Tests", all_passed);
		return all_passed;
	}

private:
	TEST_CASE(can_subdivide_test)
		// Test QuadNode subdivision capability
		CHECK(QuadTree::QuadNode::can_subdivide(Vector2i(2, 2)));
		CHECK(QuadTree::QuadNode::can_subdivide(Vector2i(5, 5)));
		CHECK(!QuadTree::QuadNode::can_subdivide(Vector2i(1, 5)));
		CHECK(!QuadTree::QuadNode::can_subdivide(Vector2i(5, 1)));
		CHECK(!QuadTree::QuadNode::can_subdivide(Vector2i(1, 1)));
	TEST_CASE_END

	TEST_CASE(get_quadrant_rects_test)
		// Test data structure for expected quadrant rectangles
		struct TestData {
			Rect2i input;
			Rect2i expected[4];
		};
		
		TestData test_data[] = {
			{Rect2i(0, 0, 2, 2), {Rect2i(0, 0, 1, 1), Rect2i(1, 0, 1, 1), Rect2i(0, 1, 1, 1), Rect2i(1, 1, 1, 1)}},
			{Rect2i(0, 0, 5, 5), {Rect2i(0, 0, 3, 3), Rect2i(3, 0, 2, 3), Rect2i(0, 3, 3, 2), Rect2i(3, 3, 2, 2)}},
			{Rect2i(2, 2, 4, 4), {Rect2i(2, 2, 2, 2), Rect2i(4, 2, 2, 2), Rect2i(2, 4, 2, 2), Rect2i(4, 4, 2, 2)}},
			{Rect2i(2, 2, 5, 5), {Rect2i(2, 2, 3, 3), Rect2i(5, 2, 2, 3), Rect2i(2, 5, 3, 2), Rect2i(5, 5, 2, 2)}}
		};

		for (int i = 0; i < 4; i++) {
			Array quadrant_rects = QuadTree::QuadNode::get_quadrant_rects(test_data[i].input);
			CHECK(quadrant_rects.size() == 4);
			for (int j = 0; j < 4; j++) {
				Rect2i rect = quadrant_rects[j];
				CHECK(rect == test_data[i].expected[j]);
			}
		}
	TEST_CASE_END

	TEST_CASE(constructor_test)
		QuadTree* quadtree = memnew(QuadTree);
		quadtree->init(Vector2i(42, 42));
		CHECK(quadtree->get_size() == Vector2i(42, 42));
		memdelete(quadtree);
	TEST_CASE_END

	TEST_CASE(add_test)
		QuadTree* quadtree = memnew(QuadTree);
		quadtree->init(Vector2i(4, 4));
		
		quadtree->add(Rect2i(0, 0, 1, 1), 42);
		CHECK(!quadtree->is_empty());
		
		// Check root has one quad rect and no quadrants
		QuadTree::QuadNode* root = quadtree->get_root();
		CHECK(root->get_quad_rects().size() == 1);
		for (int i = 0; i < 4; i++) {
			CHECK(root->get_quadrants()[i] == nullptr);
		}

		quadtree->add(Rect2i(1, 1, 1, 1), 43);
		CHECK(!quadtree->is_empty());
		
		// After adding second rect, root should subdivide
		CHECK(root->get_quad_rects().is_empty());
		CHECK(root->get_quadrants()[0] != nullptr);
		
		// Check subdivision structure
		QuadTree::QuadNode* quadrant0 = root->get_quadrants()[0];
		CHECK(quadrant0->get_quad_rects().is_empty());
		CHECK(quadrant0->get_quadrants()[0]->get_quad_rects().size() == 1);
		CHECK(quadrant0->get_quadrants()[1] == nullptr);
		CHECK(quadrant0->get_quadrants()[2] == nullptr);
		CHECK(quadrant0->get_quadrants()[3]->get_quad_rects().size() == 1);
		
		for (int i = 1; i < 4; i++) {
			CHECK(root->get_quadrants()[i] == nullptr);
		}
		
		memdelete(quadtree);
	TEST_CASE_END

	TEST_CASE(remove_test)
		QuadTree* quadtree = memnew(QuadTree);
		quadtree->init(Vector2i(4, 4));
		
		quadtree->add(Rect2i(0, 0, 1, 1), 42);
		quadtree->add(Rect2i(1, 1, 1, 1), 43);
		quadtree->remove(42);
		
		QuadTree::QuadNode* root = quadtree->get_root();
		for (int i = 0; i < 4; i++) {
			CHECK(root->get_quadrants()[i] == nullptr);
		}
		CHECK(root->get_quad_rects().size() == 1);
		
		// Check that the remaining rect has the correct metadata
		auto quad_rects = root->get_quad_rects();
		auto first_rect = quad_rects[0].operator Ref<QuadTree::QuadRect>();
		CHECK(first_rect->get_metadata() == 43);
		
		memdelete(quadtree);
	TEST_CASE_END

	TEST_CASE(get_first_test)
		QuadTree* quadtree = memnew(QuadTree);
		quadtree->init(Vector2i(4, 4));
		
		quadtree->add(Rect2i(0, 0, 1, 1), 42);
		quadtree->add(Rect2i(1, 1, 1, 1), 43);

		Ref<QuadTree::QuadRect> first = quadtree->get_first(Rect2i(0, 0, 2, 2));
		CHECK(first != nullptr);
		CHECK(first->get_rect() == Rect2i(0, 0, 1, 1));
		CHECK(first->get_metadata() == 42);

		first = quadtree->get_first(Rect2i(0, 0, 2, 2), 42);
		CHECK(first != nullptr);
		CHECK(first->get_rect() == Rect2i(1, 1, 1, 1));
		CHECK(first->get_metadata() == 43);
		
		first = quadtree->get_first(Vector2i(0, 0));
		CHECK(first != nullptr);
		CHECK(first->get_rect() == Rect2i(0, 0, 1, 1));
		CHECK(first->get_metadata() == 42);

		first = quadtree->get_first(Vector2i(0, 0), 42);
		CHECK(first == nullptr);
		
		memdelete(quadtree);
	TEST_CASE_END

	TEST_CASE(get_all_test)
		QuadTree* quadtree = memnew(QuadTree);
		quadtree->init(Vector2i(4, 4));
		
		quadtree->add(Rect2i(0, 0, 1, 1), 42);
		quadtree->add(Rect2i(1, 1, 1, 1), 43);

		Array all = quadtree->get_all(Rect2i(0, 0, 2, 2));
		CHECK(all.size() == 2);
		
		Ref<QuadTree::QuadRect> first_rect = all[0];
		CHECK(first_rect->get_rect() == Rect2i(0, 0, 1, 1));
		CHECK(first_rect->get_metadata() == 42);
		
		Ref<QuadTree::QuadRect> second_rect = all[1];
		CHECK(second_rect->get_rect() == Rect2i(1, 1, 1, 1));
		CHECK(second_rect->get_metadata() == 43);

		all = quadtree->get_all(Vector2i(1, 1));
		CHECK(all.size() == 1);
		
		Ref<QuadTree::QuadRect> only_rect = all[0];
		CHECK(only_rect->get_rect() == Rect2i(1, 1, 1, 1));
		CHECK(only_rect->get_metadata() == 43);
		
		memdelete(quadtree);
	TEST_CASE_END
};

#endif // TEST_QUAD_TREE_H