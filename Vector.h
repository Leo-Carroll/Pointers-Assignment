#pragma once

#include <initializer_list>		// Included for std::initializer_list, which is used for the construct.
#include <stdexcept>			// Included to throw std::runtime_error when something goes wrong.
#include <cassert>				// Included to use assert() which is a quick way of throwing an exception.

/**********************************************************************
* Class Name: Vector
* Class Description:
*	A custom dynamic array implementation that is similar to 
*	std::vector.
*	Supports copy/move semantics, dynamic resizing, and element access
*	with bounds checking.
**********************************************************************/
template<typename T>
class Vector {
private:
	T* m_Data;				// Pointer to the dynamically allocated array.
	size_t m_Capacity;		// Total capacity of the array.
	size_t m_Size;			// Current number of elements in the array.

public:
	// Default Constructor: Creates an empty Vector
	Vector()
		: m_Data(nullptr), m_Capacity(0), m_Size(0) {
	}

	// Constructor with initializer list
	// Allows for initialization like: Vector<int> vec = { 0, 1, 2, 3 }.
	Vector(const std::initializer_list<T>& values)
		: Vector(values.size()) {		// Preallocate memory for the list.
		// Iterate over the initializer list.
		for (const T& v : values) {
			PushBack(v);		// Insert each value.
		}
	}

	// Constructor with size_t argument for capacity
	// Allows for the user to allocate a certain amount of memory before using the Vector.
	Vector(size_t capacity)
		// Set m_Data to a new array of size capacity, set m_Capacity to the capacity, and set m_Size to 0 as there are no elements yet.
		: m_Data(new T[capacity]), m_Capacity(capacity), m_Size(0) {
	}

	// Copy Constructor
	// Allows for a Vector to be copied to the new one.
	Vector(const Vector& other)
		// Set m_Data to a new array with the other's capacity, set m_Capacity to the other's capacity, set m_Size to the other's size.
		: m_Data(new T[other.m_Capacity]), m_Capacity(other.m_Capacity), m_Size(other.m_Size) {
		// Iterate over the other Vector's data array.
		for (size_t i = 0; i < m_Size; ++i) {
			assert(i < m_Capacity);		// Stupid assert to get rid of Intellisense warning. This can never happen.
			m_Data[i] = other.m_Data[i];		// Deep copy the other's data to the new Vector's data.
		}
	}

	// Move Constructor
	// Allows for a vector to be created with std::move().
	Vector(Vector&& other) noexcept
		// Initialize the contents of the new Vector with the contents of the old one.
		: m_Data(other.m_Data), m_Capacity(other.m_Capacity), m_Size(other.m_Size) {
		// Reset the other vector's values.
		other.m_Data = nullptr;
		other.m_Capacity = 0;
		other.m_Size = 0;
	}
	
	// Basic = operator
	// Allows for a vector to be copied using the equal operator.
	Vector& operator=(const Vector& other) {
		if (this == &other) return *this;		// If it is being set to itself, return the dereferenced this pointer instead of creating a whole new array.

		delete[] m_Data;		// Delete the last m_Data.

		m_Capacity = other.m_Capacity;		// Copy the capacity.
		m_Size = other.m_Size;				// Copy the size.
		m_Data = new T[m_Capacity];			// Create a new array with the other Vector's capacity.
		for (size_t i = 0; i < m_Size; ++i) {
			m_Data[i] = other.m_Data[i];		// Deep copy each element from the other Vector to the new Vector.
		}
		return *this;		// Return the dereferenced this pointer. This allows for chained equals like vec1 = vec2 = vec3.
	}

	// Move = operator
	// Allows for vectors to be moved using the equal operator.
	Vector& operator=(Vector&& other) noexcept {
		if (this == &other) return *this;		// If it is being set to itself, return the dereferenced this pointer instead of creating a new array.
		
		delete[] m_Data;		// Delete the old array.
		
		m_Data = other.m_Data;		// Directly copy the data pointer as the other Vector's data is set to nullptr.
		m_Capacity = other.m_Capacity;		// Copy the capacity.
		m_Size = other.m_Size;		// Copy the size.

		other.m_Data = nullptr;		// Set the other Vector's data to nullptr to prevent double frees.
		other.m_Capacity = 0;		// Set the other Vector's capacity to zero.
		other.m_Size = 0;		// Set the other Vector's size to zero.

		return *this;		// Return the dereferenced this pointer to allow for chained equals like vec1 = vec2 = std::move(vec3).
	}
	
	// Destructor
	// Prevents memory leaks by deleting the data array.
	~Vector() {
		if (m_Data) {		// Make sure data is not nullptr.
			delete[] m_Data;		// Delete all elements in m_Data using delete[].
		}
	}

	// Non-const [] operator
	// Allows for the Vector to be indexed and edited without bounds checking.
	T& operator[](size_t idx) {
		return m_Data[idx];		// Return the m_Data pointer at idx without bounds checking.
	}

	// Const [] operator
	// Allows for the Vector to be indexed without bounds checking.
	const T& operator[](size_t idx) const {
		return m_Data[idx];		// Return the m_Data pointer at idx without bounds checking.
	}

	// At() Function
	// Allows for the Vector to be indexed and edited with bounds checking.
	T& At(size_t idx) {
		// Check that the Vector's size is less than the index given.
		if (idx >= m_Size) {
			throw std::runtime_error("Vector Error: Index given to At() is out of bounds.");		// Throw an exception with the error message.
		}
		return m_Data[idx];		// Return the m_Data pointer at idx with bounds checking.
	}

	// Const At() Function
	// Allows for const Vector to be index with bounds checking.
	const T& At(size_t idx) const {
		// Check that the Vector's size is less than the index given.
		if (idx >= m_Size) {
			throw std::runtime_error("Vector Error: Index given to At() is out of bounds.");		// Throw an exception with the error message.
		}
		return m_Data[idx];		// Return the m_Data pointer at idx with bounds checking.
	}

	// PushBack() Function
	// Pushes a new element to the back of the Vector.
	void PushBack(const T& value) {
		// Check if the Vector has enough capacity to push back the element.
		if (m_Size + 1 > m_Capacity) {
			Grow();		// Call the Grow() function to grow the array.
		}
		m_Data[m_Size++] = value;		// Set m_Data at the previous size to value and increment m_Size.
	}

	// PushBack() Move Function
	// Pushes a moved element to the back of the Vector.
	void PushBack(T&& value) {
		// Check if the Vector has enough capacity to move the element to the back.
		if (m_Size + 1 > m_Capacity) {
			Grow();		// Call the Grow() function to grow the array.
		}
		m_Data[m_Size++] = std::move(value);		// Move the value to m_Data at the previous size and increment m_Size.
	}

	// PushFront() Function
	// Pushes a new element to the front of the Vector.
	void PushFront(const T& value) {
		// Check if the Vector has enough capacity to push the element to the front.
		if (m_Size + 1 > m_Capacity) {
			Grow();		// Call the Grow() function to grow the array.
		}
		++m_Size;		// Increment m_Size.
		// Iterate over the array from the back.
		for (size_t i = m_Size - 1; i > 0; --i) {
			m_Data[i] = m_Data[i - 1];		// Move the m_Data at i one space to the right.
		}
		m_Data[0] = value;		// Set the now free m_Data[0] to the given value.
	}

	// PushFront() Move Function
	// Pushes a moved element to the front of the Vector.
	// Note: This function has an O(n) time complexity, and PushBack() should always be preferred for its O(1) time complexity.
	void PushFront(T&& value) {
		// Check if the Vector has enough capacity to push the element to the front.
		if (m_Size + 1 > m_Capacity) {
			Grow();		// Call the Grow() function to grow the array.
		}
		++m_Size;		// Increment m_Size.
		// Iterate over the array from the back.
		for (size_t i = m_Size - 1; i > 0; --i) {
			m_Data[i] = m_Data[i - 1];		// Move the m_Data at i one space to the right.
		}
		m_Data[0] = std::move(value);		// Set the now free m_Data[0] to the moved value.
	}

	// PopBack() Function
	// Pops the element at the back of the Vector and returns the value.
	T PopBack() {
		// Check if m_Size is zero as there would be nothing to pop.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: PopBack() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		return std::move(m_Data[--m_Size]);		// Return the moved value of m_Data at the decremented m_Size.
	}

	// PopFront() Function
	// Pops the element at the front of the Vector and returns the value.
	// Note: This function has an O(n) time complexity, and PopBack() should always be preferred for its O(1) time complexity.
	T PopFront() {
		// Check if m_Size is zero as there would be nothing to pop.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: PopFront() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		T value = std::move(m_Data[0]);		// Save the value of the front of the Vector before overriding it.
		// Iterate over the array from the front.
		for (size_t i = 0; i < m_Size - 1; ++i) {
			m_Data[i] = std::move(m_Data[i + 1]);		// Move the data at i one space to the left.
		}
		--m_Size;		// Decrement size
		return value;		// Return the previously saved value that was at m_Data[0].
	}

	// RemoveAt() Function
	// Remove the element at a given index.
	void RemoveAt(size_t idx) {
		// Check if the index given is greater or equal to m_Size.
		// It is not necessary to check if it is less than 0 as it is unsigned.
		if (idx >= m_Size) {
			throw std::runtime_error("Vector Error: Index given to RemoveAt() is out of bounds.");		// Throw a runtime error with the error message.
		}
		// Iterate over the array from the index given.
		for (size_t i = idx; i < m_Size - 1; ++i) {
			m_Data[i] = std::move(m_Data[i + 1]);		// Move the data at i one space to the left.
		}
		--m_Size;		// Decrement m_Size.
	}

	// Back() Function
	// Returns a reference to the element at the back of the Vector.
	T& Back() {
		// Check if m_Size is 0 as that would indicate that there is no back to the Vector.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: Back() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		return m_Data[m_Size - 1];		// Return m_Data at m_Size - 1 as size is one greater than the index.
	}

	// const Back() Function
	// Return a const reference to the element at the back of the Vector.
	const T& Back() const {
		// Check if m_Size is 0 as that would indicate that there is no back to the Vector.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: Back() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		return m_Data[m_Size - 1];		// Return m_Data at m_Size - 1 as size is one greater than the index.
	}

	// Front() Function
	// Returns a reference to the element at the front of the Vector.
	T& Front() {
		// Check if m_Size is 0 as that would indicate that there is no front of the Vector.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: Front() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		return m_Data[0];		// Return the front of the Vector.
	}

	// const Front() Function
	// Returns a const reference to the element at the front of the vector.
	const T& Front() const {
		// Check if m_Size is 0 as that would indicate that there is no front of the Vector.
		if (m_Size == 0) {
			throw std::runtime_error("Vector Error: Front() called when size is equal to 0.");		// Throw a runtime error with the error message.
		}
		return m_Data[0];		// Return the front of the Vector.
	}

	// Clear() Function
	// Clears the Vector by setting the size to 0.
	void Clear() {
		m_Size = 0;		// Set m_Size to 0, clearing the vector.
	}

	// Empty() Function
	// Returns if the Vector is empty.
	bool Empty() const {
		return m_Size == 0;		// Return if the vector is empty.
	}

	// Size() Function
	// Returns the size of the Vector.
	size_t Size() const {
		return m_Size;		// Return the size.
	}

	// Capacity() Function
	// Returns the capacity of the Vector.
	size_t Capacity() const {
		return m_Capacity;		// Return the capacity.
	}

private:
	// Grow() Function
	// Double the capacity, copy the values of the array, and create a new array.
	void Grow() {
		size_t newCapacity = (m_Capacity == 0) ? 1 : m_Capacity * 2;		// Get the new capacity of the Vector, which is 1 if it is already 0.
		T* copy = new T[newCapacity];		// Create a pointer of type T with the size of the new capacity.
		// Iterate over the original array.
		for (size_t i = 0; i < m_Size; ++i) {
			assert(i < m_Capacity);		// Stupid assert to get rid of Intellisense warning.
			copy[i] = m_Data[i];		// Deep copy the value of m_Data at i to the copy at i.
		}
		delete[] m_Data;		// Delete the old array.
		m_Data = copy;		// Set m_Data to the address of copy.
		m_Capacity = newCapacity;		// Update the capacity to the new capacity.
	}
};
