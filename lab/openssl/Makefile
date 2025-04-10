################################################################
CC := gcc
CFLAGS := -ggdb
LDLIBS := -lssl -lcrypto
################################################################
.DEFAULT_GOAL := encrypt
################################################################
.PHONY: clean
################################################################
clean:
	@$(RM) -v $(.DEFAULT_GOAL)
################################################################
# 6. Let a, b, c, and q be as in Exercise 5( Let a be any
# integer and let b and c be positive integers. Suppose that
# when a is divided by b, the quotient is q and ... ). Suppose
# that when q is divided by c, the quotient is k. Prove that
# when a is divided by bc, then the quotient is also k.
################################################################
# a = qb + r, 0 <= r < b
# q = kc + s, 0 <= s < c
################################################################
# a = (kc + s)b + r = k(cb) + sb + r
################################################################
# the maximum values for s is c - 1, and for r is b - 1, so the
# maximum value for sb + r is (c - 1)b + b - 1 = cb - 1 < cb, so
# sb + r is the remainder and k is the quotient
################################################################
