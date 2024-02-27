#ifndef INTERACT_ACTION_CLASS_H
#define INTERACT_ACTION_CLASS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class InteractAction : public Resource {
	GDCLASS(InteractAction, Resource);

private:
	String input = "interact";
	String description = "to Interact";
	int code = 0;

protected:
	static void _bind_methods();

public:
	InteractAction();
	~InteractAction();
	void set_input(const String &new_input);
	String get_input() const;
	void set_description(const String &new_description);
	String get_description() const;
	void set_code(const int &new_code);
	int get_code() const;
};

#endif // INTERACT_ACTION_CLASS_H