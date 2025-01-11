using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class Slot : RefCounted
{
    public static readonly StringName GDExtensionName = "Slot";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying RefCounted), please use the Instantiate() method instead.")]
    protected Slot() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="Slot"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static Slot Instantiate()
    {
        return GDExtensionHelper.Instantiate<Slot>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="Slot"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="Slot"/> wrapper type,
    /// a new instance of the <see cref="Slot"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="Slot"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static Slot Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<Slot>(godotObject);
    }
}