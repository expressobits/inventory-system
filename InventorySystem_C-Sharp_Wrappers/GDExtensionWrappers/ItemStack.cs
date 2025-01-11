using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class ItemStack : Resource
{
    public static readonly StringName GDExtensionName = "ItemStack";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected ItemStack() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="ItemStack"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static ItemStack Instantiate()
    {
        return GDExtensionHelper.Instantiate<ItemStack>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="ItemStack"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="ItemStack"/> wrapper type,
    /// a new instance of the <see cref="ItemStack"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="ItemStack"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static ItemStack Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<ItemStack>(godotObject);
    }
#region Properties

    public string ItemId
    {
        get => (string)Get("item_id");
        set => Set("item_id", Variant.From(value));
    }

    public int Amount
    {
        get => (int)Get("amount");
        set => Set("amount", Variant.From(value));
    }

    public Godot.Collections.Dictionary Properties
    {
        get => (Godot.Collections.Dictionary)Get("properties");
        set => Set("properties", Variant.From(value));
    }

#endregion

#region Signals

    public delegate void UpdatedHandler();

    private UpdatedHandler _updated_backing;
    private Callable _updated_backing_callable;
    public event UpdatedHandler Updated
    {
        add
        {
            if(_updated_backing == null)
            {
                _updated_backing_callable = Callable.From(
                    () =>
                    {
                        _updated_backing?.Invoke();
                    }
                );
                Connect("updated", _updated_backing_callable);
            }
            _updated_backing += value;
        }
        remove
        {
            _updated_backing -= value;
            
            if(_updated_backing == null)
            {
                Disconnect("updated", _updated_backing_callable);
                _updated_backing_callable = default;
            }
        }
    }

#endregion

#region Methods

    public bool Contains(string item, int amount) => Call("contains", item, amount).As<bool>();

    public bool HasValid() => Call("has_valid").As<bool>();

    public Godot.Collections.Array Serialize() => Call("serialize").As<Godot.Collections.Array>();

    public void Deserialize(Godot.Collections.Array data) => Call("deserialize", data);

#endregion

}