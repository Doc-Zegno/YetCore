## Yet ABI

### Method names mangling
The utilized mangling scheme is a trade-off between a verbosity
and readability. It is also fully compatible with C since it
doesn't use any *special* symbols like `$`, `@`, `#`.
However, this makes it necessary to introduce lots of special
prefices used to decode mangled name thus leading to a poorer
readability.

#### Trivial cases
The most straight-forward encoding is for names that obey
Re:Lite naming conventions and use primitive argument types only.
Consider the example below:
```swift
    // Assuming it's a part of the package `CompanyName.ProjectName`
    class Image {
        func save(name: String, directory: String, format: String)
    }
```
This method yields the following name:
```
    yet_CompanyName_ProjectName_Image_save__s_S_S_S
```

So, as you can see:
 1) Prepend name with a `yet_` prefix
 2) Write down method's full qualification joined with an underscore:
 `CompanyName_ProjectName_Image_save`
 3) Put two underscores in order to separate an argument types' list
 4) Finally specify types of arguments:

| Letters |  Corresponding type  |
| ------- | -------------------- |
| `s`     | `self`               |
| `V`     | `Void`               |
| `N`     | `Never`              |
| `B`     | `Bool`               |
| `C`     | `Char`               |
| `C8`    | `Char8`              |
| `I`     | `Int`                |
| `I32`   | `Int32`              |
| `U`     | `UInt`               |
| `U64`   | `UInt64`             |
| `F`     | `Float`              |
| `F32`   | `Float32`            |
| `S`     | `String`             |
| `R`     | `Any`                |
| `t1`    | template type #1     |
| `OI`    | `Optional<Int>`      |
| `AI`    | `Array<Int>`         |
| `EI`    | `Iterable<Int>`      |
| `MSI`   | `Map<String, Int>`   |
| `HS`    | `Set<String>`        |
| `T2IS`  | `Tuple<Int, String>` |
| `X2IV`  | `Function<Int, Void>`|
| `J2CB`  | `Variant<Char, Bool>`|

Please, note:
 * `s` is treated as `self` only when it is **the first** character
 * `V` is used when function has no parameters at all (static function):
```swift
    func printNewLine() <=> yet_printNewLine__V
```
 * `Any` is denoted with `R` (for `Ref`)
 * `Iterable` is denoted with `E` and not `I`
   (alternative name is `Enumerable`)
 * `Set` is denoted with `H` (think as of `HashSet`)
 * Template types are **not** translated as is. Instead, they get
   an ordinal value:
```swift
    func find<E, T>(e: E, t: T) <=> yet_find__t1_t2
```
 * `Tuple` is followed by a number of its template type arguments:
```swift
    Tuple<Int, Array<String>> <=> T2IAS
    Tuple<Bool, Map<String, Int>, Set<T>> <=> T3BMSIHt1
```
 * The same is true for `Function<Arg1, ... Result>` which is denoted
   with `X` (for `eXecutable`)
 * ...and for `Variant<Type1, ...>` which uses `J` (`Joined`) letter.

#### More complicated cases
Technically, Yet doesn't put any limitations on the naming possibilities.
That means, you can name your method exactly in the same manner as you do
when writing in C. So it can contain underscores and class name doesn't
have to start with a capitalized letter.

In order to address such cases, the following rules are used:
 1) When a name contains underscores, it should be prepended with a special
    prefix `Nw` where `N` is a *number of words*:
```swift
    create_map <=> 2wcreate_map
    create___map <=> 2wcreate___map
```
 2) In case a name also contains either leading or trailing underscores,
    it also has to be prepended with a prefix `Nu` where `N` is a
    *total number of underscores*:
```swift
    _Boolean_t <=> 2u2w_Boolean_t
```
 3) When using a fully qualified name, it should be prepended with a prefix
    `Np` where `N` is a *number of parts* whilst the parts should be
    delimited with an underscore:
```swift
    custom_package.graphics.Device <=> 3p2wcustom_package_graphics_Device
```
 4) Types with the template arguments make use of prefix `Nt` where
    `N` is a *number of mentioned types* (counting the outermost only)
    whilst the type arguments are separated with an underscore:
```swift
    // Three outermost types: `DataFrame`, `Int` and `String`
    DataFrame<Int, String> <=> 3tDataFrame_I_S

    // Nested one.
    // Three outermost types: `DataFrame`, `Int` and `Array<String>`
    DataFrame<Int, Array<String>> <=> 3tDataFrame_I_AS
```
 5) All the prefices are applied in **the reversed order** (so prefix #4
    goes before prefix #3):
```swift
    koalas.DataFrame<Int, String> <=> 3t2pkoalas_DataFrame_I_S
```
 6) Builtin containers are written in the expanded form when used with
    non-builtin types:
```swift
    // Short
    Array<Int> <=> AI

    // Short nested
    Array<Array<Int>> <=> AAI

    // Expanded
    Array<Images.Filter> <=> 2tArray_2pImages_Filter

    // Expanded nested (note: two outermost types!)
    Array<Array<Images.Filter>> <=> 2tArray_2tArray_2pImages_Filter
```
 7) Shorthand notation can be used for parameter types that have common
    parts with the full qualification of the method. Replace them with
    a `Nc` mnenomic (`N` common parts):
```swift
    // Two parts in common: `MegaApp` and `Models`.
    // Please, note: `MegaApp.Models.User` actually has 3 parts
    //   but with a `2c` it becomes `2c.User` and thus has only 2 parts
    MegaApp.Models.Util.createFrom(user: MegaApp.Models.User) <=>
        yet_MegaApp_Models_Util_createFrom__2p2c_User()
```

Now let's try it out:
```swift
    koalas.util.print_header(df: koalas.DataFrame<Int, Array<Images.Filter>>, rows: Int) <=>
        yet_koalas_util_2wprint_header__3t2p1c_DataFrame_I_2tArray_2pImages_Filter_I()
```
~~Still pretty readable~~.

#### Specials
There are some cases when the *special* kind of functions are used.
To make it possible to distinguish them, arbitrary additional attributes
can be injected between a method name and a parameter list: 
 1) Getter/setter for some property with `get` and `set` respectively:
```swift
    // Re:Lite
    class Widget {
        var isVisible: Bool
    }

    // Yet (standard prefices are omitted). Also note `self` parameter
    Widget_isVisible__get__s
    Widget_isVisible__set__s
```
 2) Operators with `operator`:
 ```swift
    // Re:Lite -- invoke
    operator ()(x: Int)

    // Yet
    invoke__operator__s_I

    // Re:Lite -- subscript
    operator [](index: Int) {
        get { ... }
        set { ... }
    }

    // Yet
    get__operator__s
    set__operator__s_I
 ```
  3) (Re:Lite special) extensions:
```swift
    // Re:Lite
    func ui.Color.darker(): ui.Color

    // Yet. Also note implicit `ui.Color` parameter
    yet_darker__extension__2pui_Color
```

#### Type variables
Type variables are used to store meta information about types
which can be retrieved at runtime. Most importantly, they contain tables
for virtual method invocation mechanism. Such variables have special suffix
`type`:
```swift
    // Type variable for the class `Images.Filter`
    yet_Images_Filter__type
```

---
### Function invocation
This section describes how to properly pass parameters, get a result
and check for errors.

#### Parameter passing
The following rules are used:
 1) Scalar primitives (i.e. `Bool`, `Char`, `IntN`, `UIntN` and `FloatN`)
    are passed as is
 2) Structures are always passed by a **non-const** C-pointer
 3) Automatic references are not C-structs and thus unmanaged `Ptr`
    (which is ultimately just an `uintptr_t` in disguise) is passed.
    The calling side **must ensure** that reference will stay valid
    (i.e. someone will point to the managed object) during all the
    function execution time. The quickiest and safest way to do that
    is to copy automatic reference onto your stack before invoking target
    method
 4) Top-level `Optional<T>` where `T` is a value type is reduced to a
    C-pointer which can be `NULL`:
```swift
    func setResolution(value: Int?)
    <=>
    yet_setResolution__OI(Int* value)
```
 5) The most nested `Optional<T>` where `T` is a referential type is reduced
    to a `Ptr` which can be `0`

Let's find out how the latest rules work in a bit complicated scenario:
```swift
    func pass(value: Any???)
```
The inner most optional is wrapped around the referential type `Any` thus gets reduced to a `Ptr`. The outer most one is wrapped around the
*value type* `Optional<Optional<Any>>` thus gets replaced with a vanilla
C-pointer:
```swift
    // Don't forget about mangling
    yet_pass__OOOR(Optional<Ptr>* value)
```

#### Retrieving the results of invocation and error checking for static functions
Technically, there is no possibility to use standard C++ exception mechanism
since its internal implementation differs from one compiler to another.
In order to make it possible to combine dynamic libraries produced with
the different compilers, a *manual* error handling mechanism is used.

Effectively, this means that **every Yet function must return** a some kind
of `Result` structure instance. Such an instance **must contain** a member
called `error` of type `Ptr`. In particular, functions with a `Void`
return value will actually return a structure with an `error` member only.
 * When an invocation is successful, it returns
a `Result` with `error` equals to zero
 * In case of failure, `error` points to the instance of type `Error`
   which contains information relative to the occurred failure.

It's a responsibility of each client of Yet functions to check its result
for errors.

That's it for error handling. The actual function's result is passed
according to the following rules:
 1) Scalar primitive types are passed as is inside the aforementioned
    `Result` structure (as its `value` member). There are dedicated
    structures like `IntResult`, `BoolResult` and so on
 2) Structure results are not returned. Instead, a client has to pass
    a C-pointer to the destination structure as the **last** parameter.
    This structure is usually allocated at client's stack before the
    function invocation. The function has to fill its fields with
    appropriate values:
```swift
    // Re:Lite
    func shift(point: Point, offset: Point): Point {
        return Point {
            x: point.x + offset.x,
            y: point.y + offset.y,
        }
    }

    // Yet
    Result yet_shift__Point_Point(Point* point, Point* offset, Point* result) {
        result->x = point->x + offset->x;
        result->y = point->y + offset->y;
        return okResult();  // inline for `Result{ 0 }`
    }
```
 3) Objects on heap are returned with a `PtrResult` which has its
    `value` member set to `Ptr` pointing to the object. The *callee side*
    must ensure reference counter of this object is not less than 1 in order
    to keep it from early deallocation. There are three things that you have
    to keep in mind so as to prevent memory leaks:
     * The safest way of dealing with a referential results is to wrap
       them with a `Ref` **immediately** using `refOf()` function.
       `Ref` has appropriate destructor which will automatically decrease
       a reference counter on leaving the scope. Please, note: `refOf()`
       **doesn't** increase a counter so it won't become 2
     * If for some reason you are not going to use `Ref` wrapper,
       you can decrease the reference counter manually using `release()`
     * Since `Ref` dtor will decrease the counter, you have to increase it
       manually if you're going to return the pointer further. This is done
       with a `retain()` method:
```swift
    PtrResult outer(Int argument) {
        PtrResult result = inner(argument);
        if (result.error) {
            // Error handling
        }
        Ref ref = refOf(result.value);
        // Some actions
        return okResult(ref.retain());
    }
```
Well, actually, if "Some actions" part is not present, you can just return
the result of inner function:
```swift
    PtrResult outer(Int argument) {
        PtrResult result = inner(argument);
        if (result.error) {
            // Error handling
        }
        return result;
    }
```