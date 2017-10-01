# mu

`mu` is a micro framework for unit testing c code. The entire framework is a single header file.

## Example

```c
#include "mu.h"

static void
test_math (void)
{
	mu_assert_int_eq (1 + 2, 3);
}

static void
test_bad_math (void)
{
	mu_assert_int_eq (1 + 2, 4);
}

int
main (void)
{
	mu_init ("math");
	mu_run (test_math);
	mu_run (test_bad_math);
	return 0;
}
```

```bash
$ gcc -o test-math test-math.c
$ ./test-math
test-math.c:12 '1 + 2==4' failed: 1 + 2=3, 4=4
    math: failed 1 of 2 assertions
$ MU_SKIP=test_bad_math ./test-math
    math: passed 1 assertion
```

## Environment Variable Options

| Option | Description |
| --- | --- |
| `MU_NOFORK=1` | Do not fork when running test functions. Without forking, the first error will halt the test. |
| `MU_VERBOSE=1` | Print verbose information about the test being run. |
| `MU_SKIP=name[:name]` | Do not run tests in the list. |
| `MU_RUN=name[:name]` | Only run tests in the list. |

## Pre-processor Macro Options

| Option | Description |
| --- | --- |
| `MU_SKIP_SUMMARY` | Do not print any pass/fail summary mesages. |
| `MU_SKIP_PASS_SUMMARY` | Do not print any pass summary mesages. |
| `MU_SKIP_FAIL_SUMMARY` | Do not print any fail summary mesages. |
| `MU_OUT` | Output `FILE *` object. Defaults to `stderr`. |

