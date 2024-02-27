#ifndef INVENTORY_SYSTEM_CLASS_H
#define INVENTORY_SYSTEM_CLASS_H

#include "core/hotbar.h"
#include "core/inventory_handler.h"
#include "craft/crafter.h"
#include "interact/interactor.h"
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class InventorySystem : public Node {
	GDCLASS(InventorySystem, Node);

private:
	InventoryHandler *inventory_handler;
	Hotbar *hotbar;
	Crafter *crafter;
	Interactor *interactor;
	bool console_mode = false;

protected:
	static void _bind_methods();

public:
	void set_console_mode(const bool new_console_mode);
	bool is_console_mode() const;
	void setup_inventory_handler(InventoryHandler *new_inventory_handler);
	void setup_crafter(Crafter *new_crafter);
	void setup_hotbar(Hotbar *new_hotbar);
	void setup_interactor(Interactor *new_interactor);
    InventoryHandler* get_inventory_handler() const;
	Crafter* get_crafter() const;
	Hotbar* get_hotbar() const;
	Interactor* get_interactor() const;
};

#endif // INVENTORY_SYSTEM_CLASS_H