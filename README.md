# mu

`mu` is a micro framework for unit testing c code. The entire framework is a single header file.

## Example

```c
#include "mu/mu.h"

int
main (void)
{
	int val = 5;
	mu_assert_int_eq (val, 5);
	mu_exit ("example")
}
```