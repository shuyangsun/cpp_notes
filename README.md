# C++ Notes

## 6. Types and Declarations

### 6.3.1 The Structure of Declarations

* * is prefix, **[]** and **()** are postfix; the postfix declarator operators bind tighter than the prefix ones.

```c++
char *arr[]; // Array of pointers to char
char( *arr)[]; // Pointer to an array of char
```

* When it comes to arrays and pointers, read from right to left.

### 6.3.3 Names
* Nonlocal names starting with an underscore are reserved for special facilities in the implementation and the run-time environment, so such names should not be used in application programs. Similarly, names starting with a double underscore (__) or an underscore followed by an uppercase letter (e.g., _Foo) are reserved.
* Use all capitals for macros and never for non-macros (not even for non-macro constants). Use underscores to separate words in an identifier.
* Language and the standard library use lowercase for types; this can be seen as a standard.

### 6.3.4 Scope
* A hidden global name can be referred to using the scope resolution operator **::**.

```c++
int x = 6;

void foo() {
	int x = 1;
	x = 2; // Local x := 2
	::x = 8; // Global x := 8
}
```

### 6.3.5 Initialization

* Initialize variables with **{}** (list initializer) instead of traditional **=** initializer.
* List initializers does not allow narrowing (check for precision lose and unsafe implicit type casts). They can also be customized for user defined objects.
* When no argument is supplied to a list initializer, an object is initialized with its default constructor or default value.

```c++
class person {
public:
	person(): ag(21) { } // Default age is 21
	unsigned short age() { return this->ag; }
private:
	unsigned short ag;
};

// Initialize with empty list initializers
int x {};
person p {};

std::cout << "x is " << x << std::endl; // x is 0
std::cout << "age is " << p.age() << std::endl; // age is 21

```

* Local variables and objects created on the free store are not initialized by default unless they are of user-defined types with a default constructor.
* Prefer **=** rather than **{}** when using **auto**.

### 6.3.6 Deducing a Type: auto and decltype()
* **auto** and **decltype()** simply report the type of an expression already known to the compiler.
* **decltype(expr)** is the type of **expr**, return type of **expr** must can be known by the compiler.

## 7. Pointers, Arrays, and References

### 7.7 References
* A reference is an alias for an object, and does not impose performance overhead compared to pointers.

``` c++
void increment(int& num) { ++num; }

int x = 5;
increment(x);
std::cout << "x is " << x << std::endl; // x is 6

```

* Differences with pointers:
	* Access with the same syntax as accessing the object.
	* Always refers to the object it was initialized with.
	* There's no "null reference".

### 7.7.1 Lvalue References
* **T&** lval;
* The value of a reference cannot be changed after initialization.
* Cannot define an array of references.
* The initializer for a "plain" **T&** must be an lvalue of type **T**.

### 7.7.2 Rvalue References
* **T&&** rval;
* Both a **const** lvalue reference and a rvalue reference can bind to an rvalue, but their purposes are fundamentally different:
	* We use rvalue references to implement a "destructive read" for optimization of what would otherwise have required a copy.
	* We use a **const** lvalue reference to prevent modification of an argument.
* Use **move()** instead of **static_cast\<T&&\>(x)** to get a rvalue reference.
* 