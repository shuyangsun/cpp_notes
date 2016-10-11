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
* 
