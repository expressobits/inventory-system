extends Node

func _ready():
	print("Running tests")
	for test_suite in get_children():
		if test_suite is TestSuite:
			test_suite.run()
