using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class QuadRect : RefCounted
{
    public static readonly StringName GDExtensionName = "QuadRect";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying RefCounted), please use the Instantiate() method instead.")]
    protected QuadRect() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="QuadRect"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static QuadRect Instantiate()
    {
        return GDExtensionHelper.Instantiate<QuadRect>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="QuadRect"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="QuadRect"/> wrapper type,
    /// a new instance of the <see cref="QuadRect"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="QuadRect"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static QuadRect Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<QuadRect>(godotObject);
    }
#region Properties

    public Variant Rect
    {
        get => (Variant)Get("rect");
        set => Set("rect", Variant.From(value));
    }

    public Variant? Metadata
    {
        get => Get("metadata") is { VariantType: not Variant.Type.Nil } _result ? _result : (Variant?)null;
        set => Set("metadata", value is not null ? Variant.From(value) : new Variant());
    }

#endregion

}