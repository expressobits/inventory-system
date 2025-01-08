using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class QuadTree : RefCounted
{
    public static readonly StringName GDExtensionName = "QuadTree";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying RefCounted), please use the Instantiate() method instead.")]
    protected QuadTree() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="QuadTree"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static QuadTree Instantiate()
    {
        return GDExtensionHelper.Instantiate<QuadTree>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="QuadTree"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="QuadTree"/> wrapper type,
    /// a new instance of the <see cref="QuadTree"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="QuadTree"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static QuadTree Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<QuadTree>(godotObject);
    }
#region Properties

    public QuadNode Root
    {
        get => (QuadNode)Get("root");
        set => Set("root", Variant.From(value));
    }

    public Vector2I Size
    {
        get => (Vector2I)Get("size");
        set => Set("size", Variant.From(value));
    }

#endregion

#region Methods

    public QuadRect GetFirst(Variant? at, Variant? exceptionMetadata) => GDExtensionHelper.Bind<QuadRect>(Call("get_first", at ?? new Variant(), exceptionMetadata ?? new Variant()).As<GodotObject>());

    public Godot.Collections.Array GetAll(Variant? at, Variant? exceptionMetadata) => Call("get_all", at ?? new Variant(), exceptionMetadata ?? new Variant()).As<Godot.Collections.Array>();

    public void Add(Rect2I rect, Variant? metadata) => Call("add", rect, metadata ?? new Variant());

    public bool Remove(Variant? metadata) => Call("remove", metadata ?? new Variant()).As<bool>();

    public bool IsEmpty() => Call("is_empty").As<bool>();

    public void Init(Vector2I size) => Call("init", size);

#endregion

}