/*
 * mu.h
 *
 * Copyright (c) 2015, Jeremy Larkin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MU_INCLUDED
#define MU_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static size_t mu_assert_count = 0, mu_failure_count = 0;

#define MU_CAT2(n, v) n##v
#define MU_CAT(n, v) MU_CAT2(n, v)
#define MU_TMP(n) MU_CAT(mu_##n, __LINE__)

#define mu_assert_msg(exp, ...) do {                      \
	__sync_fetch_and_add (&mu_assert_count, 1);           \
	if (!(exp)) {                                         \
		__sync_fetch_and_add (&mu_failure_count, 1);      \
		fprintf (stderr, "%s:%d: ", __FILE__, __LINE__ ); \
		fprintf (stderr, __VA_ARGS__);                    \
		fputc ('\n', stderr);                             \
	}                                                     \
} while (0);

#define mu_assert(exp) mu_assert_msg(exp, "'%s' failed", #exp)

#define mu_assert_int(a, OP, b) do {                                               \
	intmax_t MU_TMP(A) = (a);                                                      \
	intmax_t MU_TMP(B) = (b);                                                      \
	mu_assert_msg(MU_TMP(A) OP MU_TMP(B),                                          \
			"'%s' failed: %s=%jd, %s=%jd", #a#OP#b, #a, MU_TMP(A), #b, MU_TMP(B)); \
} while (0)
#define mu_assert_int_eq(a, b) mu_assert_int(a, ==, b)
#define mu_assert_int_ne(a, b) mu_assert_int(a, !=, b)
#define mu_assert_int_lt(a, b) mu_assert_int(a, <,  b)
#define mu_assert_int_le(a, b) mu_assert_int(a, <=, b)
#define mu_assert_int_gt(a, b) mu_assert_int(a, >,  b)
#define mu_assert_int_ge(a, b) mu_assert_int(a, >=, b)

#define mu_assert_uint(a, OP, b) do {                                              \
	uintmax_t MU_TMP(A) = (a);                                                     \
	uintmax_t MU_TMP(B) = (b);                                                     \
	mu_assert_msg(MU_TMP(A) OP MU_TMP(B),                                          \
			"'%s' failed: %s=%ju, %s=%ju", #a#OP#b, #a, MU_TMP(A), #b, MU_TMP(B)); \
} while (0)
#define mu_assert_uint_eq(a, b) mu_assert_uint(a, ==, b)
#define mu_assert_uint_ne(a, b) mu_assert_uint(a, !=, b)
#define mu_assert_uint_lt(a, b) mu_assert_uint(a, <,  b)
#define mu_assert_uint_le(a, b) mu_assert_uint(a, <=, b)
#define mu_assert_uint_gt(a, b) mu_assert_uint(a, >,  b)
#define mu_assert_uint_ge(a, b) mu_assert_uint(a, >=, b)

#define mu_assert_str(a, OP, b) do {                                                     \
	const char *MU_TMP(A) = (const char *)(a);                                           \
	const char *MU_TMP(B) = (const char *)(b);                                           \
	mu_assert_msg (MU_TMP(A) == MU_TMP(B) ||                                             \
			(MU_TMP(A) && MU_TMP(B) &&  0 OP strcmp (MU_TMP(A), MU_TMP(B))),             \
			"'%s' failed: %s=\"%s\", %s=\"%s\"", #a#OP#b, #a, MU_TMP(A), #b, MU_TMP(B)); \
} while (0)
#define mu_assert_str_eq(a, b) mu_assert_str(a, ==, b)
#define mu_assert_str_ne(a, b) mu_assert_str(a, !=, b)
#define mu_assert_str_lt(a, b) mu_assert_str(a, <,  b)
#define mu_assert_str_le(a, b) mu_assert_str(a, <=, b)
#define mu_assert_str_gt(a, b) mu_assert_str(a, >,  b)
#define mu_assert_str_ge(a, b) mu_assert_str(a, >=, b)

#define mu_assert_ptr(a, OP, b) do {                                             \
	const void *MU_TMP(A) = (a);                                                 \
	const void *MU_TMP(B) = (b);                                                 \
	mu_assert_msg(MU_TMP(A) OP MU_TMP(B),                                        \
			"'%s' failed: %s=%p, %s=%p", #a#OP#b, #a, MU_TMP(A), #b, MU_TMP(B)); \
} while (0)
#define mu_assert_ptr_eq(a, b) mu_assert_ptr(a, ==, b)
#define mu_assert_ptr_ne(a, b) mu_assert_ptr(a, !=, b)

static int
mu_final (const char *name)
{
	size_t fails = mu_failure_count, asserts = mu_assert_count;
	if (fails == 0) {
		fprintf (stderr, "%8s: passed %zu assertion%s\n", name, asserts, asserts == 1 ? "" : "s");
		return EXIT_SUCCESS;
	}
	fprintf (stderr, "%8s: failed %zu of %zu assertion%s\n", name, fails, asserts, asserts == 1 ? "" : "s");
	return EXIT_FAILURE;
}

static void __attribute__((unused))
mu_exit (const char *name)
{
	exit (mu_final (name));
}

#endif

