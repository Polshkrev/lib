# Dynamic Array
Implementation of a classical dynamic array.

## Table Of Contents
1. [Append](#append)
2. [Remove](#remove)
3. [Access](#access)
4. [Implements](#implements)

### Append
To append to an array there is &mdash; of course &mdash; the `append` method. This method appends a singular item to the array. Since this is a fixed-size array, if the array is full, a `ValueError` is thrown.
### Remove
To remove from an array, there is a simple `remove` method. This method takes in an index where the value to be removed is located.
### Access
To access an item in the array, the `at` method is available. This method will take a given index. This method will either return a pointer to the item stored at the given index, or an `IndexError` is thrown.
### Implements
This structure implements the [Collection](/docs/en-UK/cpp/collections/collection.md), and [Sized](/docs/en-UK/cpp/collections/sized.md) interfaces.