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

#### 8.2.7 Fields
* Fields (also called *bit-field*) are members of struct field by specified number of bits it is to occupy.

```c++
struct color_16 {
	const char* name;
	bool is_visible: 1; // Occupies 1 bit
	unsigned short r: 4; // Occupies 4 bits
	unsigned short g: 4; // Occupies 4 bits
	unsigned short b: 4; // Occupies 4 bits
};
```

* Using fields to pack several variables into a single byte does not necessarily save space. It saves data space, but the size of the code needed to manipulate these variables increases on most machines. Programs have been known to shrink significantly when binary variables were converted from bit-fields to characters! Furthermore, it is typically much faster to access a **char** or an **int** than to access a field.

### 8.3 Unions
* A **union** is a **struct** in which all members are allocated at the same address so that the **union** occupies only as much space as its largest member.
* Naturally, a **union** can hold a value for only one member at a time.

```c++
union value {
	const char *s;
	long double d;
	bool i;
};

value val;
val.i = 6;
std::cout << sizeof(value) << std::endl; // 16
std::cout << val.i << std::endl; // 1
```

#### 8.3.1 Unions and Classes
* **union** has a lot of restrictions comparing to **class**.
* **union** cannot have or be used as a base class.
* If a **union** has a member with a user-defined constructor, a copy operation, a move operation, or a destructor, then that special function is deleted for that **union**; that is, it cannot be used for an object of the **union** type.
* It is possible to specify an in-class initializer for at most one member. If so, this initializer will be used for default initialization.

```c++
union zip_code {
	unsigned int int_val;
	const char* str_val {""};
};

zip_code nyc; // Default initialized to str_val == ""
std::cout << "nyc zip code is: " << nyc.str_val << std::endl; // nyc zip code is:
nyc.int_val = 10010;
std::cout << "nyc zip code is: " << nyc.int_val << std::endl; // nyc zip code is: 10010
	
zip_code ames {50010}; // error: no matching constructor for initialization of 'zip_code'

```

#### 8.3.2 Anonymous unions
* An anonymous **union** is an object, not a type, and its members can be accessed without mentioning an object name.
* We can use members of an anonymous **union** exactly as we use other members of a class - as long as we remember that **union** members really can be used only one at a time.

```c++
class address {
private:
	const char *street_addr;
	union { // Anonymous union
		const char *zip_str;
		unsigned int zip_int;
	};
	bool zip_is_str;

public:
	address(const char* addr, const char* zip):
		street_addr{addr},
		zip_str{zip},
		zip_is_str{true} { };
	
	address(const char* addr, const unsigned int zip):
		street_addr{addr},
		zip_int{zip},
		zip_is_str{false} { };
	
	std::string street_address() {
		return std::string(street_addr);
	};
	
	std::string zip_code() {
		if (zip_is_str) {
			return std::string(zip_str);
		} else {
			return std::to_string(zip_int);
		}
	};
};

address home {"Mars", "MRS-265128"};
address work {"NYC", 10010};
std::cout << home.street_address() << ", " << home.zip_code() << std::endl; // Mars, MRS-265128
std::cout << work.street_address() << ", " << work.zip_code() << std::endl; // NYC, 10010
```

### 8.4 Enumerations
* An *enumeration* is a type that can hold a set of integer values specified by the user.
* Some of an enumeration's possible values are named and called *enumerators*.
* Cannot initialize an *enumerator* with list initializer (but can with **enum**s).
* There are two kinds of enumerations:
	1. **enum class**es, for which the enumerator names are *local* to the **enum** and their values do *not* implicitly convert to integers.
	2. "Plain **enum**s", for which the enumerator names are in the *same scope* as the **enum** and their values implicitly convert to integers.

```c++
enum class color {
	red, green, blue // Must use "color::red" to access; NOT converted to integers.
};

color c1 {blue}; // error: use of undeclared identifier 'blue'
color c2 {color::blue}; // OK

std::cout << c2 << std::endl;
// error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'color')

enum sorting_order {
	ascending=9, descending // Can be accessed directly; converted to integers, 0 is default, and goes up.
};

sorting_order order1 {sorting_order::ascending}; // OK
sorting_order order2 {descending}; // OK
std::cout << order1 << std::endl; // 9
std::cout << order2 << std::endl; // 10
```

#### 8.4.1 enum classes
* An enumeration is represented by some integer type and each enumerator by some integer value.
* The type used to represent an enumeration is called its *underlying type*.
* The underlying type must be one of the signed or unsigned integer types; the default is **int**.
* Even if there is an underlying type, it will not be implicitly converted; to get the value of underlying type, use casting.

```c++
enum class color: char {
	red='r', green='g', blue='b'
};

color light {color::red};
std::cout << light << std::endl;
// error: invalid operands to binary expression ('ostream' (aka 'basic_ostream<char>') and 'color')
std::cout << static_cast<char>(light) << std::endl; // r
```

* Using bit-wise operators (e.g., **|**, **&**) needs explicit casting of **enum class**.

```c++
enum class options {
	a = 1, b = 2, c = 4, d = 8
};
	
options opt1 {options::a};
options opt2 {options::c};
std::cout << (opt1 | opt2) << std::endl;
// error: invalid operands to binary expression ('options' and 'options')
std::cout << (static_cast<int>(opt1) | static_cast<int>(opt2)) << std::endl; // 5
```

* Even if two enumerators of **enum class**es have the same underlying value, they cannot be compared with each other (which is not true for plain **enum**).

```c++
enum class light {
	red, green, yellow
};
	
enum class color {
	red, green, blue
};
	
light red_light {light::red};
color red_color {color::red};
bool same = red_light == red_color;
// warning: comparison of two values with different enumeration types ('light' and 'color')


enum state {
	new_york, iowa, illinois
};

enum city {
	new_york, // error: redefinition of enumerator 'new_york'
	nyc, // OK
	ames, chicago
};

state new_york_state {new_york};
city new_york_city {nyc};
bool same = new_york_city == new_york_state; // true (NOT GOOD!)
```

* Bit-wise operations for plain **enum**s does not need explicit casting, since they are implicitly converted to their underlying types.

#### 8.4.3 Unnamed enums
* A plain **enum** can be unnamed, we use that when all we need is a set of integer constants, rather than a type to use for variables.

```c++
enum { up=1, left, down, right };
```

___

## 9. Statements

### 9.1 Introduction
* Expressions have values, statements don't.

### 9.2 Statements Summary
* A (possibly empty) sequence of statements within "curly braces" (i.e., **{** and **}**) is called a *block* or a *compound statement*.
* A name declared in a block goes out of scope at the end of its block.

#### 9.4.2 switch Statements
* No need for terminating semicolon at the end of **switch**-statement.
* **switch**-statements needs **break** at the end of each **case**, otherwise it'll fall through like C (unlike Swift).

##### 9.4.2.1 Declarations in Cases
* It is possible, and common, to declare variables within the block of a **switch**-statement. However, it is not possible to bypass an initialization.

```c++
void foo(int x) {
	switch (x) {
		case 0:
			int y {9}; // error: declaration can be bypassed
			int y; // OK, but really bad
			y = 9;
			std::cout << y << std::endl;
			break;
		case 5:
			++x;
			++y; // Bad, accessing uninitialized variable
			std::cout << x << " " << y << std::endl; // 
			break;
		default:
			break;
	}
}

// If executed in (5, 0) order:
foo(0); // 9
foo(5); // 6 10

// If executed in (5, 0) order:
foo(5); // 6 289686817
foo(0); // 9
```

* If we need a variable within a **switch**-statement, we can limit its scope by enclosing its declaration and its use in a block.

#### 9.4.3 Declaration in Conditions

* Purpose: limit the scope of that variable to only that block.
* A declaration in a condition must declare and initialize a single variable or **const**.

```c++
int x {5};
if (bool even = is_even(x)) {
	std::cout << "x is even, is_even = " << even << std::endl;
} else {
	std::cout << "x is odd, is_even = " << even << std::endl;
}
// output: x is odd, is_even = 0
```

#### 9.5.1 Range-for Statements
* For-each loop


```c++
for (T x: v) { /* ... */ }
for (T& x: v) { /* Use reference to modify elements in the vector. */ }
for (const T& x: v) { /* Use const reference for potentially large elements. */ }
```

### 9.6 goto Statements
* Use **goto** only to jump forward to exit complicated loops, otherwise it's too confusing (even better: not using **goto** at all).

```c++
for (const T& x: v) {
	for (/* another for loop */) {
		for (/* and... one more */) {
			// Find element...
			if (found) {
				goto found; // Source of evil code!!!
			}
			// Some other stuff...
		}
		// Some other stuff...
	}
	// Some other stuff...
}

found:
// Blah blah blah...
```

### 9.7 Comments and Indentation
* **/*** **\*/** style comments do NOT nest (unlike Swift).


___

## 10. Expressions

#### 10.3.1 Results
* C++ follows "the usual arithmetic conversions": to produce a result of the "largest" operand type.
* Assignment is an expression.
* Underflow, overflow, and division by zero do not throw standard exceptions.

#### 10.3.2 Order of Evaluation

```c++
int i = 1;
v[i] = i++; // undefined result
// May be evaluated as v[1] = 1 or v[2] = 1, or other undefined result
```

#### 10.3.3 Operator Precedence
* If you start feeling the need for parentheses, you might consider breaking up the expression by using an extra variable.

#### 10.3.4 Temporary Objects
* A temporary object is created by compiler to hold an intermediate result of an expression. (e.g., **val = x + y * z**, there is a temporary project to hold the value of **y * z**)
* Unless bound to a reference or used to initialize a named object, a temporary object is destroyed at the end of the full expression in which is was created. (sometimes this does not work well with certain ways of writing code)

```c++
std::string str1 = "Hello, ";
std::string str2 = "world!";
// c_str() is holding a reference to (str1 + str2), which may be deleted at the end of expression.
const char *c_str = (str1 + str2).c_str();
std::cout << c_str << std::endl;
```

* Solution to this problem: bind the variables (in this case, **str1** and **str2**) to a **const** reference.

### 10.4 Constant Expressions
* C++ offers two related meanings of "constant":
	* **constexpr**: Evaluate at compile time (enable and ensure compile-time evaluation).
	* **const**: Do not modify in this scope (specify immutability in interfaces).
* A constant expression must start out with an integral value, a floating point value, or an enumerator, and we can combine those using operators and **constexpr** functions that in turn produce values.
* **constexpr** is a better choice for defining simple constants.

### 10.4.3 Literal Types
* A class with a **constexpr** constructor is called a *literal type*. To be simple enough to be **constexpr**, a constructor must have an empty body and all members must be initialized by potentially constant expressions.

```c++
struct Point {
	float x, y, z;
	// The "const" in the next line is not required for C++11, but is required for C++14.
	// C++11 implicitly implies the "const", so to be safe it should always be added.
	constexpr Point change(float dist) const {
		return {x + dist, y + dist, z + dist};
	}
	void print() const {
		std::cout<< "Point(" << x << ", " << y << ", " << z << ")" << std::endl;
	}
};

constexpr Point a {1, 2, 3};
constexpr Point b = a.change(9.63f);

a.print(); // Point(1, 2, 3)
b.print(); // Point(10.63, 11.63, 12.63)
```

#### 10.4.5 Address Constant Expressions
* The address of a statically allocated object, such as a global variable, is a constant.

```c++
constexpr const char *p1 = "asdf";
```

#### 10.5 Implicit Type Conversion
* *promotions*: preserve value.
* *narrowing*: not value preserving.
* The **{}**-initializer syntax prevents narrowing.
* **narrow_cast<>()** is a run-time checked conversion function. (use only if not avoidable or intended)
* Any pointer to an object type can be implicitly converted to a **void\***
* Pointers, integral, and floating-point values can be implicitly converted to **true**. A nonzero value is converted to **true**, a zero value is converted to **false**.
* When a floating-point value is converted to an integer value (that's big enough to hold the integer part), the fractional part is discarded.

___

## 11. Select Operations

#### 11.1.2 Bitwise Logical Operators
* They can be used on integral types (signed or unsigned).

### 11.2 Free Store
* **new** and **delete** are used to create/destroy objects on the *free store* respectively.
* An object created by **new** exists until it is explicitly destroyed by **delete**.
* The **delete** and **delete[]** operator may be applied only to a pointer returned by **new** or to the **nullptr**. Applying them to the **nullptr** has no effect.
* If the deleted object is of a class with a destructor, that destructor is called by **delete** before the object's memory is released for reuse.
* To deallocate space allocated by **new**, **delete** and **delete[]** must be able to determine the size of the object allocated (which requires extra space). This overhead is not significant when we allocate many objects or large objects, but it can matter if we allocate lots of small objects on the free store.
* A **return** or an exception thrown before the **delete** will cause a memory leak (unless even more code is added).

#### 11.2.1 Memory Management
* Main problems with free store: *leaked objects*, *premature deletion*, and *double deletion*.
* Don't put objects on the free store if you don't have to; prefer scoped variables.
* When you construct an object on the free store, place its pointer into a *manager object* (*handle*) with a destructor that will destroy it. (e.g., **string**, **vector**, all standard library containers, **unique_ptr**, and **shared_ptr**)
* Rule of thumb: no naked **new**s. **new** belongs in constructors and similar operations, **delete** belongs in destructors.

#### 11.2.2 Arrays
* **delete[]** is used to delete arrays created by **new**.

#### 11.2.3 Getting Memory Space
* The standard implementations of **operator new()** and **operator new\[]()** do not initialize the memory returned. They take arguments or return values of type **void***.

#### 11.2.4 Overloading new
* Placing an object in an area that is not (directly) controlled by the standard free-store manager implies that some care is required when destroying the object.

##### 11.2.4.1 nothrow new
* Use **nothrow** version of **new** and **delete** if no exceptions must be avoided.
* **nothrow** version of **new** returns **nullptr**, rather than throwing **bad_alloc** if the object cannot be allocated.

```c++
std::string* str_ptr {new(std::nothrow) std::string {}};
operator delete(str_ptr, std::nothrow);
```

### 11.3 Lists
* **{}**-list can be used as expressions in many (but not all) places:
	* *Qualified* by a type **T{...}**, meaning "create an object of type **T** initialized by **T{...}**".
	* *Unqualified* **{...}**, for which the type must be determined from the context of use.

#### 11.3.1 Implementation Model
* If the **{}**-list is used to construct an **initializer_list**, elements are typically copied from the **initializer_list** to wherever we use them. Not copied otherwise, except as by-value constructor arguments.

#### 11.3.3 Unqualified Lists
* When used as the initializer for a named object without the use of a **=**, an unqualified **{}**-list performs direct initialization. In all other cases, it performs copy initialization.

```c++
int x {8}; // initializer (direct initialization)
int y = {9}; // initializer (copy initialization)
```

* Use **initializer_list\<T>** to allow initializer lists for built-in or user-defined containers.

```c++
struct my_float {
    float val;
    
    my_float(const std::initializer_list<const std::string> init_lst) {
        const std::string& first {*init_lst.begin()};
        this->val = std::stof(first);
    }
    
    void print_floating_point() {
        std::cout << this->val - std::floor(this->val) << std::endl;
    }
};

float max_ele(const std::initializer_list<const float> init_lst) {
    float cur = std::numeric_limits<float>::min();
    for (auto ele: init_lst) {
        if (ele > cur) {
            cur = ele;
        }
    }
    return cur;
}

my_float f {"3.14159"};
f.print_floating_point(); // 0.14159
    
const float max {max_ele({1, 2.5, - 1.0, -9.9, 3.14159})};
std::cout << max << std::endl; // 3.14159
```

* The type of a **{}**-list can be deduced (only) if all elements are of the same type.
* Unfortunately, C++ does NOT deduce the type of an unqualified list for a plain template argument.

### 11.4 Lambda Expressions
* A lambda expression consists of a sequence of parts:
	* A possibly empty capture *list*.
	* An optional *parameter list*.
	* An optional **mutable** specifier.
	* An optional **noexcept** specifier.
	* An optional return type declaration of the form **->** type.
	* A *body*, specifying the code to be executed.

```c++
// A lambda function that captures output stream and precision.
// Then rounds the argument, prints it and returns it.
[&output_stream, precision](double x) -> double { // output_stream is stored as reference; precision is copy
	double rounded = my_round(x, precision);
	output_stream << rounded << std::endl;
	return rounded;
}
```

* Lambda expressions typically do not yield performance overhead (e.g., identical to replacing the code).

#### 11.4.3 Capture
* Lambda introducers:
	* **[]**: an empty capture list.
	* **[&]**: capture by reference.
	* **[=]**: capture by value.
	* **[***capture-list***]**: explicit capture; the *capture-list* is the list of names of local variables to be captured.
	* **[&,** *capture-list***]**: implicitly capture by reference all local variables with names NOT mentioned in the list.
	* **[=,** *capture-list***]**: implicitly capture by value all local variables with names NOT mentioned in the list.
* Local name preceded by **&** is always captured by reference and a local name not preceded by **&** is always captured by value.
* Lambda might outlive its caller.
* When passing a lambda to another thread, capturing by value (**[=]**) is typically best.

##### 11.4.3.2 Namespace Names
* Don't need to capture namespace variables (including global variables).

##### 11.4.3.3 Lambda and this
* Use **[this]** to capture member **this** from class.
* Members are always captured by reference (i.e., **[this]** and **[=]** are incompatible).

##### 11.4.3.4 mutable Lambdas

```c++
[] mutable { /* lambda body */ }
```

#### 11.4.5 The Type of a Lambda
* *Closure type* is unique to lambda, so no two lambdas have the same type.
* **std::function\<R(AL)>** where **R** is the lambda's return type and **AL** is its argument list of types.

```c++
// Example of a lambda function type (takes a double and an int, returns a boolean.)
std::function<bool (double, int)>
```
* A lambda that captures nothing can be assigned to a pointer to function of an appropriate type.

### 11.5 Explicit Type Conversion
* Named conversions:
	* **const_cast** for getting write access to something declared **const**
	* **static_cast** for reversing a well-defined implicit conversion
	* **reinterpret_cast** for changing the meaning of bit patterns
	* **dynamic_cast** for dynamically checked class hierarchy navigation
* Prefer **T{v}** conversions for well-behaved construction and the named casts (e.g., **static_cast**) for other conversions.

#### 11.5.1 Construction
* The construction of a value of type **T** from a value **e** can be expressed by the notation **T(e)**.


___

## 12. Functions

* The semantics of argument passing are identical to the semantics of copy initialization.

#### 12.1.2 Parts of a Function Declaration
* Function specifiers and modifiers:
	* The name of the function; required
	* The argument list, which may be empty **()**; required
	* The return type, which may be **void** and which may be prefix or suffix (using **auto**); required
	* **inline**, indicating a desire to have function calls implemented by inlining the function body
	* **constexpr**, indicating that it should be possible to evaluate the function at compile time if given constant expressions as arguments
	* **noexcept**, indicating that the function may not throw an exception
	* A linkage specification, for example, **static**
	* **[[noreturn]]**, indicating that the function will not return using the normal call/return mechanism
	* For member functions:
		* **virtual**, indicating that it can be overridden in a derived class
		* **override**, indicating that it must be overriding a virtual function from a base class
		* **final**, indicating that it cannot be overridden in a derived class
		* **static**, indicating that it is not associated with a particular object
		* **const**, indicating that it may not modify its object

#### 12.1.3 Function Definitions
* A **const** is ignored at the highest level of an argument type.

```c++
void foo(int); 			// type is void(int)
void bar(const int); 	// type is void(int)
```

* Other things can be called:
	* *Constructors* are technically not functions; in particular, they don't return a value, can initialize bases and members, and can't have their address taken.
	* *Destructors* can't  be overloaded and can't have their address taken.
	* *Function objects* are not functions (they are objects) and can't be overloaded, but their **operator()**s are functions.
	* *Lambda expressions* are basically a shorthand for defining function objects.

#### 12.1.4 Returning Values
* Can use suffix return type.

```c++
double square(double x);
auto square(double x) -> double; // equivalent to above line declaration

// Use for function template declaration, where the return type depends on the argument
template<class T, class U>
auto product(const vector<T>& x, const vector<U>& y) -> decltype(x * y);
auto square(const T& x) -> decltype(x * x);
```
* The semantics of function value return are identical to the semantics of copy initialization.
* The store is reused after the function returns, so a pointer to a local non-**static** variable should never be returned.
* Five ways to exit a function:
	* Use **return**.
	* Falling off the end in functions return **void** and **main()**.
	* Throwing an exception that isn't caught locally.
	* Terminating because an exception was thrown and not caught locally in a **noexcept** function.
	* Directly or indirectly invoking a system function that doesn't return.

#### 12.1.5 inline Functions
* **inline** specifier tells the compiler it should attempt to generate code inline rather than actually calling the function.

```c++
inline auto fact(const unsigned int n) -> unsigned long long {
    return (n < 2) ? 1 : n * fact(n - 1);
}
```
* To make inlining possible in the absence of unusually clever compilation and linking facilities, the definition - and not just the declaration - of an inline function must be in scope.
* An **inline** specifier does not affect the semantics of a function. In particular, an inline function still has a unique address, and so do **static** variables of an inline function.

#### 12.1.6 constexpr Functions
* When used in an object definition, it means "evaluate the initializer at compile time."
* A **constexpr** function must consist of a single **return**-statement, no loops and no local variables are allowed, cannot have side effects.
* Allows member initialization, recursion, and conditional expressions.
* A **constexpr** function can refer to nonlocal objects as long as it does not write to them.

#### 12.1.7 [[noreturn]] Functions
* A construct **[[...]]** is called an *attribute* and can be placed just about anywhere in the C++ syntax (e.g., in front of a declaration).
* There are only two standard attributes: **[[noreturn]]** and **[[carries_dependency]]**.

```c++
[[noreturn]] inline auto terminate() -> void {
    std::cout << "terminating program..." << std::endl;
    exit(0);
}
```

#### 12.1.8 Local Variables
* If a local variable is declared **static**, a single, statically allocated object will be used to represent that variable in all calls of the function.
* Initialization of **static** local variable does not lead to a data race unless you enter the function containing it recursively or a deadlock occurs.
* The effect of initializing a local **static** recursively is undefined.

### 12.2 Argument Passing
* Unless a formal argument is a reference, a copy of the actual argument is passed to the function.

#### 12.2.1 Reference Arguments
* The absence of **const** in the declaration of a reference argument is taken as a statement of intent to modify the variable.
* Rule of thumb:
	1. Use pass-by-value for small objects
	2. Use pass-by-**const**-reference to pass large values that you don't need to modify
	3. Return a result as a **return** value rather than modifying an object through an argument
	4. Use rvalue references to implement move and forwarding
	5. Pass a pointer if "no object (**nullptr**)" is a valid alternative
	6. Use pass-by-reference only if you have to

#### 12.2.2 Array Arguments
* If an array is used as a function argument, a pointer to its initial element is passed.

```c++
// Three identical function declarations
void odd(int* p);
void odd(int a[]);
void odd(int buf[1024]);
```
* You can pass a reference to array, with number of elements as part of the type.

```c++
void foo(int(&arr)[1024]);
```

#### 12.2.3 List Arguments
* A **{}**-delimited list can be used as an argument to a parameter of:
	1. Type **std::initializer_list\<T>**, where the values of the list can be implicitly converted to **T**
	2. A type that can be initialized with the values provided in the list
	3. A reference to an array of **T**, where the values of the list can be implicitly converted to **T**

#### 12.2.4 Unspecified Number of Arguments
* Three ways to do it:
	1. Use a variadic template
	2. Use an **initializer_list**
	3. Terminate the argument list with the ellipsis (**...**), and use some macros from **\<cstdarg>**

```c++
#include <cstdarg>

void print_args(int first ...) {
    va_list ap;
    va_start(ap, first);
    
    for (;;) {
        int val {va_arg(ap, int)};
        if (val == 0) break;
        std::cout << val << ' ';
    }
    
    va_end(ap);
    std::cout << std::endl;
}

void print_args(const std::initializer_list<int> args) {
    for (auto ele: args) {
        std::cout << ele << ' ';
    }
    std::cout << std::endl;
}

// Use <cstdarg> (bad)
print_args(5, 6, 9, -4, 0, 12); // 6 9 -4
// Use initializer_list (good)
print_args({5, 6, 9, -4, 0, 12}); // 5 6 9 -4 0 12
```

#### 12.2.5 Default Arguments
* Default arguments may be provided for trailing arguments only.

```c++
int foo(int a, int b, int c =0, char *c =nullptr);
int bar(int=3);
int wrong(char*=nullptr); // syntax error: *= is an assignment operator
```

### 12.5 Pointer to Function

```c++
double half(int x) {
	return x / 2.0;
}
double (* calculate)(int x); // pointer to function taking an int argument and returning a double

calculate = &half;
calculate(3); // 1.5
```