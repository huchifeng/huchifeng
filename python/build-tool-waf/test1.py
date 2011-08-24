def foo():
	from waflib.Context import WAFVERSION
	print("This is Waf %s" % WAFVERSION)