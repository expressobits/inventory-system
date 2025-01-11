using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class QuadNode : RefCounted
{
    public static readonly StringName GDExtensionName = "QuadNode";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying RefCounted), please use the Instantiate() method instead.")]
    protected QuadNode() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="QuadNode"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static QuadNode Instantiate()
    {
        return GDExtensionHelper.Instantiate<QuadNode>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="QuadNode"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="QuadNode"/> wrapper type,
    /// a new instance of the <see cref="QuadNode"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="QuadNode"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static QuadNode Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<QuadNode>(godotObject);
    }
#region Properties

    public Godot.Collections.Array Quadrants
    {
        get => (Godot.Collections.Array)Get("quadrants");
        set => Set("quadrants", Variant.From(value));
    }

    public int QuadrantCount
    {
        get => (int)Get("quadrant_count");
        set => Set("quadrant_count", Variant.From(value));
    }

    public Godot.Collections.Array QuadRects
    {
        get => (Godot.Collections.Array)Get("quad_rects");
        set => Set("quad_rects", Variant.From(value));
    }

    public Variant Rect
    {
        get => (Variant)Get("rect");
        set => Set("rect", Variant.From(value));
    }

#endregion

#region Methods

    public static bool CanSubdivide(Vector2I size) => GDExtensionHelper.Call("QuadNode", "can_subdivide", size).As<bool>();

    public static Godot.Collections.Array<Rect2I> GetQuadrantRects(Rect2I rect) => GDExtensionHelper.Call("QuadNode", "get_quadrant_rects", rect).As<Godot.Collections.Array<Godot.Rect2I>>();

    public bool IsEmpty() => Call("is_empty").As<bool>();

    public QuadRect GetFirstUnderRect(Rect2I testRect, Variant? exceptionMetadata) => GDExtensionHelper.Bind<QuadRect>(Call("get_first_under_rect", testRect, exceptionMetadata ?? new Variant()).As<GodotObject>());

    public QuadRect GetFirstContainingPoint(Vector2I point, Variant? exceptionMetadata) => GDExtensionHelper.Bind<QuadRect>(Call("get_first_containing_point", point, exceptionMetadata ?? new Variant()).As<GodotObject>());

    public Godot.Collections.Array GetAllUnderRect(Rect2I testRect, Variant? exceptionMetadata) => Call("get_all_under_rect", testRect, exceptionMetadata ?? new Variant()).As<Godot.Collections.Array>();

    public Godot.Collections.Array GetAllContainingPoint(Vector2I point, Variant? exceptionMetadata) => Call("get_all_containing_point", point, exceptionMetadata ?? new Variant()).As<Godot.Collections.Array>();

    public void Add(QuadRect quadRect) => Call("add", (RefCounted)quadRect);

    public bool Remove(Variant? metadata) => Call("remove", metadata ?? new Variant()).As<bool>();

    public void Collapse() => Call("_collapse");

#endregion

}