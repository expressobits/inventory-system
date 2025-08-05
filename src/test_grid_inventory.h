/**
 * test_grid_inventory.h
 * =============================================================================
 * Copyright (c) 2023-present ExpressoBits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#ifndef TEST_GRID_INVENTORY_H
#define TEST_GRID_INVENTORY_H

#include "tests.h"
#include "core/grid_inventory.h"
#include "base/inventory_database.h"

using namespace godot;

class TestGridInventory : public InventoryTestBase {
private:
	String wood = "wood";
	String stone_pickaxe = "stone_pickaxe";
	String campfire = "campfire";
	Ref<InventoryDatabase> database;

public:
	TestGridInventory() {
		database.instantiate();
		// Database would be properly initialized in real setup
	}

	virtual const char* get_test_name() const override {
		return "Grid Inventory Tests";
	}

	virtual bool run_tests() override {
		log_test_start("Grid Inventory Tests");
		
		bool all_passed = true;
		all_passed &= test_has_place_for();
		all_passed &= test_add_item_automerge();
		all_passed &= test_stack_split();
		all_passed &= test_stack_cant_split();
		all_passed &= test_stack_join();
		all_passed &= test_stack_cant_join();
		all_passed &= test_automerge();
		all_passed &= test_autosplitmerge();
		all_passed &= test_wrong_stack_type();
		all_passed &= test_clear();
		all_passed &= test_serialize();
		all_passed &= test_full();
		all_passed &= test_get_stack_at();
		all_passed &= test_transfer_to_with_stack();
		all_passed &= test_has_space_for();

		log_test_end("Grid Inventory Tests", all_passed);
		return all_passed;
	}

private:
	TEST_CASE(test_has_place_for)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->has_space_for(wood));
		CHECK(inventory_3x3->has_space_for(stone_pickaxe));

		CHECK(inventory_3x3->add(wood) == 0);
		CHECK(inventory_3x3->has_space_for(stone_pickaxe));

		CHECK(inventory_3x3->add(campfire) == 0);
		CHECK(!inventory_3x3->has_space_for(stone_pickaxe));

		inventory_3x3->clear();
		CHECK(inventory_3x3->get_stacks().size() == 0);
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_add_item_automerge)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add(stone_pickaxe) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 1);

		inventory_3x3->clear();
		CHECK(inventory_3x3->add(campfire, 8) == 0);
		CHECK(inventory_3x3->add(campfire, 1) == 1);
		CHECK(inventory_3x3->add(wood) == 0);

		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_stack_split)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add(wood, 2) == 0);
		CHECK(inventory_3x3->split(0, 1));
		CHECK(inventory_3x3->get_stacks().size() == 2);
		CHECK(inventory_3x3->contains(wood, 2));
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_stack_cant_split)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add(campfire) == 0);
		CHECK(!inventory_3x3->split(0, 1));
		CHECK(inventory_3x3->get_stacks().size() == 1);
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_stack_join)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add_on_new_stack(wood) == 0);
		CHECK(inventory_3x3->add_on_new_stack(wood) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 2);
		CHECK(inventory_3x3->transfer_at(1, inventory_3x3, 0, 1) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 1);
		// TODO: Add stack amount verification when stack API is available
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_stack_cant_join)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 1) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 2);
		CHECK(inventory_3x3->transfer_at(1, inventory_3x3, 0, 1) == 1);
		CHECK(inventory_3x3->get_stacks().size() == 2);
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_automerge)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);
		GridInventory* inventory_3x3_2 = memnew(GridInventory);
		inventory_3x3_2->set_size(Vector2i(3, 3));
		inventory_3x3_2->set_database(database);

		CHECK(inventory_3x3->add(stone_pickaxe) == 0);
		CHECK(inventory_3x3_2->add(campfire) == 0);
		CHECK(inventory_3x3_2->add(wood) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 1);
		CHECK(inventory_3x3_2->get_stacks().size() == 2);

		CHECK(inventory_3x3_2->transfer(0, inventory_3x3, 1) == 1);
		CHECK(inventory_3x3_2->get_stacks().size() == 2);
		CHECK(inventory_3x3->get_stacks().size() == 1);

		CHECK(inventory_3x3_2->transfer(0, inventory_3x3, 1) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 2);
		CHECK(inventory_3x3_2->get_stacks().size() == 1);

		memdelete(inventory_3x3);
		memdelete(inventory_3x3_2);
	TEST_CASE_END

	TEST_CASE(test_autosplitmerge)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);
		GridInventory* inventory_3x3_2 = memnew(GridInventory);
		inventory_3x3_2->set_size(Vector2i(3, 3));
		inventory_3x3_2->set_database(database);

		CHECK(inventory_3x3->add(campfire, 6) == 0);
		CHECK(inventory_3x3_2->add(campfire, 4) == 0);
		// TODO: Add specific stack amount checks when stack API is available
		CHECK(inventory_3x3_2->transfer(0, inventory_3x3, 4) == 2);
		CHECK(inventory_3x3->get_stacks().size() == 1);
		CHECK(inventory_3x3_2->get_stacks().size() == 1);

		memdelete(inventory_3x3);
		memdelete(inventory_3x3_2);
	TEST_CASE_END

	TEST_CASE(test_get_stack_at)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add_at_position(Vector2i(1, 1), wood, 4) == 0);
		CHECK(inventory_3x3->add_at_position(Vector2i(2, 1), "stone", 3) == 0);
		auto stack = inventory_3x3->get_stack_at(Vector2i(1, 1));
		CHECK(stack != nullptr);
		int index = inventory_3x3->get_stacks().find(stack);
		CHECK(!(index < 0 || index >= inventory_3x3->get_stacks().size()));
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_transfer_to_with_stack)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add_at_position(Vector2i(1, 1), wood, 4) == 0);
		CHECK(inventory_3x3->add_at_position(Vector2i(2, 1), "stone", 3) == 0);
		CHECK(inventory_3x3->transfer_to(Vector2i(1, 1), inventory_3x3, Vector2i(2, 1), 4) == 0);
		// TODO: Add verification of stack item_id when stack API is available
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_wrong_stack_type)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);
		GridInventory* inventory_3x3_2 = memnew(GridInventory);
		inventory_3x3_2->set_size(Vector2i(3, 3));
		inventory_3x3_2->set_database(database);

		Dictionary props1;
		props1["ok"] = "2";
		Dictionary props2;
		props2["teste"] = "3";
		
		CHECK(inventory_3x3->add(wood, 1, props1) == 0);
		CHECK(inventory_3x3_2->add(wood, 1, props2) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 1);
		CHECK(inventory_3x3_2->get_stacks().size() == 1);
		CHECK(inventory_3x3_2->transfer(0, inventory_3x3, 1) == 0);
		CHECK(inventory_3x3->get_stacks().size() == 2);
		CHECK(inventory_3x3_2->get_stacks().size() == 0);

		memdelete(inventory_3x3);
		memdelete(inventory_3x3_2);
	TEST_CASE_END

	TEST_CASE(test_clear)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->add(wood, 1) == 0);
		// TODO: Add quad tree checks when API is available
		CHECK(!inventory_3x3->is_empty());
		inventory_3x3->clear();
		CHECK(inventory_3x3->get_stacks().size() == 0);
		CHECK(inventory_3x3->is_empty());
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_full)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);

		CHECK(inventory_3x3->has_free_place(Vector2i(1, 1)));
		CHECK(!inventory_3x3->is_full());
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(!inventory_3x3->is_full());
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(!inventory_3x3->is_full());
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->add(wood, 16) == 0);
		CHECK(inventory_3x3->is_full());
		
		memdelete(inventory_3x3);
	TEST_CASE_END

	TEST_CASE(test_serialize)
		GridInventory* inventory_3x3 = memnew(GridInventory);
		inventory_3x3->set_size(Vector2i(3, 3));
		inventory_3x3->set_database(database);
		GridInventory* inventory_3x3_2 = memnew(GridInventory);
		inventory_3x3_2->set_size(Vector2i(3, 3));
		inventory_3x3_2->set_database(database);

		CHECK(inventory_3x3->add_at_position(Vector2i(0, 1), wood, 1) == 0);
		// TODO: Add stack position verification when API is available
		Dictionary data = inventory_3x3->serialize();
		inventory_3x3->clear();
		inventory_3x3_2->deserialize(data);
		inventory_3x3->deserialize(data);
		CHECK(inventory_3x3_2->contains(wood, 1));
		// TODO: Add quad tree verification when API is available
		
		memdelete(inventory_3x3);
		memdelete(inventory_3x3_2);
	TEST_CASE_END

	TEST_CASE(test_has_space_for)
		GridInventory* inventory_8x1 = memnew(GridInventory);
		inventory_8x1->set_size(Vector2i(8, 1));
		inventory_8x1->set_database(database);
		GridInventory* inventory_8x5 = memnew(GridInventory);
		inventory_8x5->set_size(Vector2i(8, 5));
		inventory_8x5->set_database(database);

		CHECK(!inventory_8x1->has_space_for("workbench"));
		CHECK(inventory_8x5->has_space_for("workbench"));

		memdelete(inventory_8x1);
		memdelete(inventory_8x5);
	TEST_CASE_END
};

#endif // TEST_GRID_INVENTORY_H