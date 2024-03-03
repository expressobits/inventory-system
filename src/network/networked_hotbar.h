#ifndef NETWORKED_HOTBAR_CLASS_H
#define NETWORKED_HOTBAR_CLASS_H

#include "core/hotbar.h"

using namespace godot;

class NetworkedHotbar : public Hotbar {
	GDCLASS(NetworkedHotbar, Hotbar);

protected:
	static void _bind_methods();

public:
	virtual void _ready() override;
	void change_selection(const int &new_index) override;
	void change_selection_rpc(const int new_index);
	void change_selection_response_rpc(const int new_index);
};

#endif // NETWORKED_HOTBAR_CLASS_H