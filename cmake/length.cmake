set(a "a;b")
list(LENGTH a len)
message(STATUS "${len}")
set(b "a\;b")
list(LENGTH b len)
message(STATUS "${len}")
