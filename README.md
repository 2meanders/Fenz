# Fenz

A set of c++ libraries made for maximum **safety** and performance.

## Array (fenz/array.hpp)

This library provides a set of C++ classes for working with fixed-size arrays and non-owning views over array data. It is header-only and designed for performance and safety.

### Features

- Element access with **compile-time bounds checking**.
- Range-based for loop support.
- Efficient subarray views without copying data.

### Class overview:

- **Array**: Owns its data, fixed size, supports initialization with default values.
- **Iterable**: Non-owning view, allows mutable access and iteration.
- **ConstIterable**: Non-owning view, allows read-only access and iteration.

### Usage

Include the header:

```cpp
#include "fenz/array.hpp"
```

Create an array:

```cpp
fenz::Array<int, 5> arr(0); // Array of 5 ints, initialized to 0
```

Iterate and modify:

```cpp
for(int& value : arr) {
    value = 1;
}

arr.enumerate([](int &value, int index) {
    value += index * 2;
});
```

Create a view:

```cpp
auto view = arr.view<1, 4>(); // View of elements 1 to 3
```

Iterate over the view:

```cpp
int sum = 0;

for(int& value : view) {
    sum += value;
}
```

### API Reference

See [fenz/array.hpp](fenz/array.hpp) for full documentation of:

- [`fenz::Array`](fenz/array.hpp)
- [`fenz::Iterable`](fenz/array.hpp)
- [`fenz::ConstIterable`](fenz/array.hpp)

## Time (fenz/time.hpp)

This header-only library provides types and utilities for safe, efficient time handling in C++. It is designed for performance and clarity, with strong type safety and Doxygen-style documentation.

### Overview

- **fenzTimeSource**: A user-defined function returning the current time in milliseconds since an arbitrary start. **Must be defined by the user for the library to work.**
- **fenz::Duration**: Represents a time duration in milliseconds. Supports conversion to seconds, arithmetic, and comparison.
- **fenz::Moment**: Represents a point in time (in milliseconds, as defined by `fenzTimeSource`). Supports arithmetic and comparison with `Duration` and other `Moment` objects.

### Usage

Include the header:

```cpp
#include "fenz/time.hpp"
```

Define the time source (example using `<chrono>`):

```cpp
#include <chrono>

long long fenzTimeSource() {
        auto now = std::chrono::system_clock::now();
        auto duration_since_epoch = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();
        return milliseconds;
}
```

Create and use durations:

```cpp
fenz::Duration d1 = fenz::Duration::fromMillis(500);
fenz::Duration d2 = fenz::Duration::fromSeconds(2.5);
auto total = d1 + d2;
double seconds = total.seconds();
```

Work with moments:

```cpp
fenz::Moment start = fenz::Moment::now();
// ... do work ...
fenz::Moment end = fenz::Moment::now();
fenz::Duration elapsed = end - start;
if (elapsed > fenz::Duration::fromSeconds(1.0)) {
        // More than 1 second has passed
}
```

### API Reference

See [fenz/time.hpp](fenz/time.hpp) for full documentation of:

- [`fenzTimeSource`](fenz/time.hpp): User-defined function returning the current time in milliseconds.
- [`fenz::Duration`](fenz/time.hpp):
  - `fromMillis(long long ms)`: Create from milliseconds.
  - `fromSeconds(double sec)`: Create from seconds.
  - `millis()`: Get milliseconds.
  - `seconds()`: Get seconds as double.
  - Arithmetic and comparison operators.
- [`fenz::Moment`](fenz/time.hpp):
  - `now()`: Get the current moment.
  - Arithmetic and comparison with `Duration` and other `Moment` objects.

All operators and methods are documented in the header file.

## Option (fenz/Option.hpp)

This header-only library provides a simple, type-safe optional value container for C++. It allows you to represent values that may or may not be present, similar to `std::optional` (C++17+), but without the risk of exceptions.

### Overview

- **fenz::Option<T>**: A template class that can either contain a value of type `T` or represent the absence of a value.
  - Provides methods to check for presence, retrieve the value, or supply a fallback.
  - Supports copy construction, assignment, and implicit conversion to `bool`.

### Usage

Include the header:

```cpp
#include "fenz/option.hpp"
```

Create and use an Option:

```cpp
fenz::Option<int> maybeValue; // No value
if (!maybeValue) {
    maybeValue = 42;
}

if (maybeValue.hasValue()) {
    int value = maybeValue.valueOr(0); // value == 42
}

int fallback = maybeValue.valueOr(100); // returns 42 if present, otherwise 100
```

Assign a fallback if none is present:

```cpp
fenz::Option<std::string> maybeName;
std::string& name = maybeName.valueOrAssign("default"); // assigns and returns "default"
```

Unsafe access is clearly marked:

```cpp
// Access the value directly (unsafe, only if you know it's present):
int value = maybeValue.value_unsafely();
```

### API Reference

See [fenz/option.hpp](fenz/option.hpp) for full documentation of:

- [`fenz::Option<T>`](fenz/option.hpp):
  - `Option()`: Constructs an empty Option (no value).
  - `Option(T value)`: Constructs an Option containing a value.
  - `Option(const Option&)`: Copy constructor.
  - `operator=(const Option&)`: Assignment operator.
  - `hasValue()`: Returns true if a value is present.
  - `operator bool()`: Implicit conversion to bool (true if value is present).
  - `valueOrAssign(const T&)`: Returns the value if present, otherwise assigns and returns the fallback.
  - `valueOr(const T&) const`: Returns the value if present, otherwise returns the fallback.
  - `value_unsafely()`: Returns the contained value without checking if present (undefined behavior if empty).

All methods are documented in the header file.

## Queue (fenz/queue.hpp)

This header-only library provides a simple, fixed-capacity circular queue for C++. It is designed for safety and performance, with no dynamic memory allocation and strong type guarantees.

### Dependencies

- [Option](#option-fenzoptionhpp). You must also have `option.hpp` in the same directory as `queue.hpp` in order for `queue.hpp` to compile.

### Features

- Fixed-size circular buffer implementation.
- Efficient enqueue and dequeue operations.
- Uses `fenz::Option` for safe value handling (no exceptions).

### Overview

- **Queue**: Template class with type and capacity parameters. Supports adding, removing, and inspecting items.

### Usage

Include the header:

```cpp
#include "fenz/queue.hpp"
```

Create a queue:

```cpp
fenz::Queue<int, 8> q; // Queue of 8 ints
```

Add items:

```cpp
q.enqueue(42); // Adds 42, returns true if successful, returns false if full
```

Force add (overwrite oldest if full):

```cpp
q.forceEnqueue(99); // Adds 99, overwrites oldest if full
```

Dequeue items:

```cpp
Option<int> item = q.dequeue();
if (item) {
    int value = item.valueOr(0); // Get value, fallback to 0 if empty
}
```

Check status:

```cpp
if (q.isFull()) { /* ... */ }
if (q.isEmpty()) { /* ... */ }
unsigned int n = q.size();
```

### API Reference

See queue.hpp for full documentation of:

- `fenz::Queue<T, Capacity>`:
  - `enqueue(const T&)`: Adds item, returns true if successful.
  - `forceEnqueue(const T&)`: Adds item, overwrites oldest if full.
  - `dequeue()`: Removes and returns item as `Option<T>`.
  - `size()`: Returns current number of items.
  - `capacity()`: Returns maximum capacity.
  - `isFull()`: Checks if queue is full.
  - `isEmpty()`: Checks if queue is empty.

All methods are documented in the header file.

## License

Distributed under the terms of the MIT License. See [LICENSE](LICENSE)
