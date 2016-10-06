# C++ Notes

___


## 6. Types and Declarations

#### 6.3.1 The Structure of Declarations

* * is prefix, **[]** and **()** are postfix; the postfix declarator operators bind tighter than the prefix ones.

```c++
char *arr[]; // Array of pointers to char
char( *arr)[]; // Pointer to an array of char
```

* When it comes to arrays and pointers, read from right to left.

#### 6.3.3 Names
* Nonlocal names starting with an underscore are reserved for special facilities in the implementation and the run-time environment, so such names should not be used in application programs. Similarly, names starting with a double underscore (__) or an underscore followed by an uppercase letter (e.g., _Foo) are reserved.
* Use all capitals for macros and never for non-macros (not even for non-macro constants). Use underscores to separate words in an identifier.
* Language and the standard library use lowercase for types; this can be seen as a standard.

#### 6.3.4 Scope
* A hidden global name can be referred to using the scope resolution operator **::**.

```c++
int x = 6;

void foo() {
	int x = 1;
	x = 2; // Local x := 2
	::x = 8; // Global x := 8
}
```

#### 6.3.5 Initialization

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

#### 6.3.6 Deducing a Type: auto and decltype()
* **auto** and **decltype()** simply report the type of an expression already known to the compiler.
* **decltype(expr)** is the type of **expr**, return type of **expr** must can be known by the compiler.

___
## 7. Pointers, Arrays, and References

#### 7.4.2 Multidimensional Arrays
* Sizes of all dimensions for variables of n-dimensional array has to be known by the compiler

```c++
int arr1[][]; // error: array has incomplete element type 'int []'
int arr2[][5]; // error: definition of variable with array type needs an explicit size or an initializer
int arr3[3][5]; // OK
```

* When n-dimensional array is used as function argument, only the last n - 1 dimensions' sizes has to be known by the compiler.

```c++
void foo1(int arr[][]); // error: array has incomplete element type 'int []'
void foo2(int arr[][5]); // OK
void foo3(int arr[5][]); // error: array has incomplete element type 'int []'
void foo4(int arr[][][5]); // error: array has incomplete element type 'int [][5]'
void foo5(int arr[][3][5]); // OK
```

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

#### 7.7.1 Lvalue References
* **T&** lval;
* The value of a reference cannot be changed after initialization.
* Cannot define an array of references.
* The initializer for a "plain" **T&** must be an lvalue of type **T**.

#### 7.7.2 Rvalue References
* **T&&** rval;
* Both a **const** lvalue reference and a rvalue reference can bind to an rvalue, but their purposes are fundamentally different:
	* We use rvalue references to implement a "destructive read" for optimization of what would otherwise have required a copy.
	* We use a **const** lvalue reference to prevent modification of an argument.
* Use **move()** instead of **static_cast\<T&&\>(x)** to get a rvalue reference.

#### 7.7.4 Pointers and References
* If you need to change which object to refer to, use a pointer.
* If you want to be sure that a name always refers to the same object, use a reference.
* If you want to use a user-defined (overloaded) operator on something that refers to an object, use a reference.

___
## 8. Structures, Unions, and Enumerations

### 8.2 Structures
* There's a terminating **;** (semicolon) at the end of structure definition, each member is also terminated by **;**.
* Structs can be initialized using **{}** notation.

```c++
struct person {
	const char *first_name;
	const char *last_name;
	unsigned short age;
};

person me {"Wolve", "Rine", 658};
```

#### 8.2.1 struct Layout
* Members are allocated in memory in declaration order.
* The size of an object of a **struct** is not necessarily the sum of the sizes of it's members, because they have to be properly *aligned* in the machine memory.
* Can minimize wasted space by simply ordering members by size (largest member first).
* Use of multiple access specifiers (i.e., **public**, **private**, or **protected**) can affect layout.

```c++
// Allocation 1 (on Intel based macOS, assuming word size is 8 bytes)
struct product {
	short year_released;
	const char *name;
	short month_released;
};
// Total: 3 words
```

| Member | Memory Space | Number of Bytes |
| --- | --- | --- |
| year_released | == | 2 |
| name | ======== | 8 |
| month_released | == | 2 |

```c++
// Allocation 2 (on Intel based macOS, assuming word size is 8 bytes)
struct product {
	const char *name;
	short year_released;
	short month_released;
};
// Total: 2 words
```

| Member | Memory Space | Number of Bytes |
| --- | --- | --- |
| name | ======== | 8 |
| (year\_released, month\_released) | ==== | 4 |


#### 8.2.2 struct Names
* The name of a type becomes available for use immediately after it has been encountered and not just after the complete declaration has been seen.
* It is not possible to declare new objects of a **struct** until its complete declaration has been seen, because the compiler is not able to determine the size of that **struct** object (but is able to for the pointer to that **struct** object).

```c++
struct node {
	node* next; // OK, it's a pointer to node.
};

struct node_bad {
	node_bad next; // NOT OK, it's an object of node_bad.
};
```

* To allow multiple **struct**s to refer to each other, we can declare a name to be the name of a **struct**.

```c++
struct lion; // Declaration, will be defined later.

struct deer {
	lion* eaten_by;
};

struct lion {
	deer* lunch;
};
```

* It's possible to declare a **struct** and a non-**struct** with the same name in the same scope.
* Use **struct** <*name*> to refer to the **struct** type, and plane <*name*> to refer to the other.
* Avoid overloading names.

```c++
struct stat { /* ... */ }
int stat(char *name, struct stat* buf);
```

#### 8.2.3 Structures and Classes
* A **struct** is simply a **class** where the members are public by default. So, a **struct** can have member functions. In particular, a **struct** can have constructors.
* Use list initializer to initialize the structure with default values, even if no constructor is defined

```c++
struct person {
	const char *first_name;
	const char *last_name;
	unsigned short age;
	person(const char *f_name, const char *l_name, const unsigned short ag);
};

person::person(const char *f_name, const char *l_name, const unsigned short ag):
	first_name{f_name},
	last_name{l_name}
{
	if (ag < 150) {
		age = ag;
	} else {
		std::cerr << "Trying to initialize an alien." << std::endl;
		exit(1);
	}
}

person me {"Shuyang", "Sun", 23};
person alien {"E", "T", 290};
```

#### 8.2.5 Type Equivalence
* Two structs are different even when they have the same members.

#### 8.2.6 Plain Old Data
* POD can be copied very efficiently by using block move machine instructions (e.g., **std::memcpy()**).
* To be a POD, the object must:
	* Not have a complicated layout (e.g., with a **vptr**).
	* Not have nonstandard (user-defined) copy semantics.
	* Have a trivial default constructor.
* **is_pod** in standard-library defined in **\<type_traits\>** allows us to ask if a type is POD.

```c++
struct S1 { int x; };
struct S2 { int x; S2(int num): x{num} {}; }; // Has customized constructor, no default constructor.
struct S3 { int x; S3(int num): x{num} {}; S3() {}; }; // Has default constructor.
	
std::cout << std::is_pod<int>::value << std::endl; // 1
	
std::cout << std::is_pod<S1>::value << std::endl; // 1
std::cout << std::is_pod<S2>::value << std::endl; // 0
std::cout << std::is_pod<S3>::value << std::endl; // 0
```