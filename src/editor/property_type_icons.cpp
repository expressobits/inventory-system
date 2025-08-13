#ifdef TOOLS_ENABLED

#include "property_type_icons.h"

namespace godot {

Dictionary PropertyTypeIcons::get_icons_name() {
    Dictionary icons_name;
    icons_name.clear();
    icons_name[Variant::NIL] = "null";
    icons_name[Variant::BOOL] = "bool";
    icons_name[Variant::INT] = "int";
    icons_name[Variant::FLOAT] = "float";
    icons_name[Variant::STRING] = "String";
    icons_name[Variant::VECTOR2] = "Vector2";
    icons_name[Variant::VECTOR2I] = "Vector2i";
    icons_name[Variant::RECT2] = "Rect2";
    icons_name[Variant::RECT2I] = "Rect2i";
    icons_name[Variant::VECTOR3] = "Vector3";
    icons_name[Variant::VECTOR3I] = "Vector3i";
    icons_name[Variant::TRANSFORM2D] = "Transform2D";
    icons_name[Variant::VECTOR4] = "Vector4";
    icons_name[Variant::VECTOR4I] = "Vector4i";
    icons_name[Variant::PLANE] = "Plane";
    icons_name[Variant::QUATERNION] = "Quaternion";
    icons_name[Variant::AABB] = "AABB";
    icons_name[Variant::BASIS] = "Basis";
    icons_name[Variant::TRANSFORM3D] = "Transform3D";
    icons_name[Variant::PROJECTION] = "Projection";
    icons_name[Variant::COLOR] = "Color";
    icons_name[Variant::STRING_NAME] = "StringName";
    icons_name[Variant::NODE_PATH] = "NodePath";
    icons_name[Variant::RID] = "RID";
    icons_name[Variant::OBJECT] = "Object";
    icons_name[Variant::CALLABLE] = "Callable";
    icons_name[Variant::SIGNAL] = "Signal";
    icons_name[Variant::DICTIONARY] = "Dictionary";
    icons_name[Variant::ARRAY] = "Array";
    icons_name[Variant::PACKED_BYTE_ARRAY] = "PackedByteArray";
    icons_name[Variant::PACKED_INT32_ARRAY] = "PackedInt32Array";
    icons_name[Variant::PACKED_INT64_ARRAY] = "PackedInt64Array";
    icons_name[Variant::PACKED_FLOAT32_ARRAY] = "PackedFloat32Array";
    icons_name[Variant::PACKED_FLOAT64_ARRAY] = "PackedFloat64Array";
    icons_name[Variant::PACKED_STRING_ARRAY] = "PackedStringArray";
    icons_name[Variant::PACKED_VECTOR2_ARRAY] = "PackedVector2Array";
    icons_name[Variant::PACKED_VECTOR3_ARRAY] = "PackedVector3Array";
    icons_name[Variant::PACKED_COLOR_ARRAY] = "PackedColorArray";
    return icons_name;
}

String PropertyTypeIcons::get_icon_name_for_type(Variant::Type type) {
    Dictionary icons_name = get_icons_name();
    if (icons_name.has(type)) {
        return icons_name[type];
    }
    return "null";
}

} // namespace godot

#endif // TOOLS_ENABLED