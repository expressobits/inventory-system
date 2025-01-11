using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class GridInventory : Inventory
{
    public new static readonly StringName GDExtensionName = "GridInventory";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Node), please use the Instantiate() method instead.")]
    protected GridInventory() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="GridInventory"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public new static GridInventory Instantiate()
    {
        return GDExtensionHelper.Instantiate<GridInventory>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="GridInventory"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="GridInventory"/> wrapper type,
    /// a new instance of the <see cref="GridInventory"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="GridInventory"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public new static GridInventory Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<GridInventory>(godotObject);
    }
#region Properties

    public Vector2I Size
    {
        get => (Vector2I)Get("size");
        set => Set("size", Variant.From(value));
    }

    public Godot.Collections.Array<GridInventoryConstraint> GridConstraints
    {
        get => GDExtensionHelper.Cast<GridInventoryConstraint>((Godot.Collections.Array<Godot.GodotObject>)Get("grid_constraints"));
        set => Set("grid_constraints", Variant.From(value));
    }

    public Godot.Collections.Array StackPositions
    {
        get => (Godot.Collections.Array)Get("stack_positions");
        set => Set("stack_positions", Variant.From(value));
    }

    public Godot.Collections.Array StackRotations
    {
        get => (Godot.Collections.Array)Get("stack_rotations");
        set => Set("stack_rotations", Variant.From(value));
    }

#endregion

#region Signals

    public delegate void SizeChangedHandler();

    private SizeChangedHandler _sizeChanged_backing;
    private Callable _sizeChanged_backing_callable;
    public event SizeChangedHandler SizeChanged
    {
        add
        {
            if(_sizeChanged_backing == null)
            {
                _sizeChanged_backing_callable = Callable.From(
                    () =>
                    {
                        _sizeChanged_backing?.Invoke();
                    }
                );
                Connect("size_changed", _sizeChanged_backing_callable);
            }
            _sizeChanged_backing += value;
        }
        remove
        {
            _sizeChanged_backing -= value;
            
            if(_sizeChanged_backing == null)
            {
                Disconnect("size_changed", _sizeChanged_backing_callable);
                _sizeChanged_backing_callable = default;
            }
        }
    }

#endregion

#region Methods

    public void SetQuadTree(QuadTree quadTree) => Call("set_quad_tree", (RefCounted)quadTree);

    public QuadTree GetQuadTree() => GDExtensionHelper.Bind<QuadTree>(Call("get_quad_tree").As<GodotObject>());

    public Vector2I GetStackPosition(ItemStack stack) => Call("get_stack_position", (Resource)stack).As<Vector2I>();

    public bool SetStackPosition(ItemStack stack, Vector2I newPosition) => Call("set_stack_position", (Resource)stack, newPosition).As<bool>();

    public Vector2I GetStackSize(ItemStack stack) => Call("get_stack_size", (Resource)stack).As<Vector2I>();

    public Rect2I GetStackRect(ItemStack stack) => Call("get_stack_rect", (Resource)stack).As<Rect2I>();

    public bool IsStackRotated(ItemStack stack) => Call("is_stack_rotated", (Resource)stack).As<bool>();

    public void Rotate(ItemStack stack) => Call("rotate", (Resource)stack);

    public bool CanRotateItem(ItemStack stack) => Call("can_rotate_item", (Resource)stack).As<bool>();

    public ItemStack GetStackAt(Vector2I position) => GDExtensionHelper.Bind<ItemStack>(Call("get_stack_at", position).As<GodotObject>());

    public int GetStackIndexAt(Vector2I position) => Call("get_stack_index_at", position).As<int>();

    public int AddAtPosition(Vector2I position, string itemId, int amount, Godot.Collections.Dictionary properties, bool isRotated) => Call("add_at_position", position, itemId, amount, properties, isRotated).As<int>();

    public Godot.Collections.Array<ItemStack> GetStacksUnder(Rect2I rect) => GDExtensionHelper.Cast<ItemStack>(Call("get_stacks_under", rect).As<Godot.Collections.Array<Godot.GodotObject>>());

    public int TransferTo(Vector2I fromPosition, GridInventory destination, Vector2I destinationPosition, int amount, bool isRotated) => Call("transfer_to", fromPosition, (Node)destination, destinationPosition, amount, isRotated).As<int>();

    public bool SwapStacks(Vector2I position, GridInventory otherInventory, Vector2I otherPosition) => Call("swap_stacks", position, (Node)otherInventory, otherPosition).As<bool>();

    public bool RectFree(Rect2I rect, ItemStack exception) => Call("rect_free", rect, (Resource)exception).As<bool>();

    public bool Sort() => Call("sort").As<bool>();

#endregion

}