using GDExtension.Wrappers;
using Godot;
using System;

/// <summary>
/// Test that the wrapper is working
/// </summary>
public partial class TestCsharp : Node
{
	[Export]
	private Node inventoryNode;


	// exporting this directly and assigning the node in the editor gives the following error:
	// E 0:00:01:0131   VariantUtils.generic.cs:387 @ T Godot.NativeInterop.VariantUtils.ConvertTo<T>(Godot.NativeInterop.godot_variant&): System.InvalidCastException: Unable to cast object of type 'Godot.Node' to type 'GDExtension.Wrappers.Inventory'.
    private Inventory inventory; 

	public override void _Ready()
	{
        // usually we would cast:
        // inventory = inventoryNode as Inventory;
		// but that doesn't work with the wrapper
		// instead:
        inventory = Inventory.Bind(inventoryNode);
		GD.Print("Inventory amount: " + inventory.GetAmount());
    }

	public override void _Process(double delta)
	{

	}
}
