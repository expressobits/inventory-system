#ifdef TOOLS_ENABLED

#ifndef PROPERTY_TYPE_ICONS_H
#define PROPERTY_TYPE_ICONS_H

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class PropertyTypeIcons {
public:
    static Dictionary get_icons_name();
    static String get_icon_name_for_type(Variant::Type type);
};

} // namespace godot

#endif // PROPERTY_TYPE_ICONS_H

#endif // TOOLS_ENABLED