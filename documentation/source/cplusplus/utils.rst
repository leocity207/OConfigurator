Utilities
=========

This page describes two lightweight utility components used throughout the
configuration system: :code:`O::Expected` and :code:`O::For_Each_In_Tuple`.

These utilities are header-only and intentionally minimal. They do *not*
produce Doxygen output, so this document explains their purpose and usage
directly.

.. contents::
   :local:
   :depth: 2


Expected (O::Expected)
----------------------
Overview
^^^^^^^^
:code:`O::Expected<T, E>` is a tiny alternative to ``std::expected`` intended
for simple error-handling without exceptions.

It stores either:

* a **value** of type ``T``  
* or an **error** of type ``E``

Only one of them is active at a time. The type does not use dynamic memory.

Basic Usage
^^^^^^^^^^^
Construct an ``Expected`` from a value:

.. code-block:: cpp

    O::Expected<int, std::string> good = O::Expected<int, std::string>::Make_Value(42);

Check for success:

.. code-block:: cpp

    if (good) {
        int v = good.Value();
    }

Construct from an error:

.. code-block:: cpp

    auto bad = O::Expected<int, std::string>::Make_Error("something went wrong");

Inspect an error:

.. code-block:: cpp

    if (!bad) {
        std::string msg = bad.Error();
    }

Moving values
^^^^^^^^^^^^^
``Value()`` and ``Error()`` are available in ``&`` and ``&&`` overloads, so you
can efficiently move the stored object:

.. code-block:: cpp

    int v = std::move(good).Value();


Why Expected?
^^^^^^^^^^^^^
It is primarily used for:

* **Configuration parsing**, where a module may produce a structured error.  
* **Replacing exceptions** in low-level code where exceptions are not desired.  
* **Aggregating application-level errors** (such as JSON parser errors).


Tuple Helper (O::For_Each_In_Tuple)
-----------------------------------
Overview
^^^^^^^^
``O::For_Each_In_Tuple(tuple, fn)`` provides a compact and readable way to apply
a callable to every element of a ``std::tuple``.

It uses ``std::apply`` internally and expands the parameter pack using a fold
expression.

Basic Usage
^^^^^^^^^^^
Iterating over each element:

.. code-block:: cpp

    auto t = std::make_tuple(1, 2.5, std::string("abc"));

    O::For_Each_In_Tuple(t, [](auto& elem) {
        // elem refers to each tuple element in order
        std::cout << elem << "\n";
    });

Modifying elements:

.. code-block:: cpp

    auto t = std::make_tuple(1, 2, 3);

    O::For_Each_In_Tuple(t, [](auto& elem) {
        elem *= 2;   // modifies tuple values in-place
    });

    // t becomes (2, 4, 6)

Why Tuple Helper?
^^^^^^^^^^^^^^^^^
It removes boilerplate and avoids needing recursive templates or manual index
sequences. It is frequently used when iterating module data stored inside:

* ``O::Configuration::Application::Container``  
* or any tuple of module types.

It is particularly visible in JSON builder/writer code.

``For_Each_In_Tuple`` guarantees:

* No run-time overhead (fully optimized away).  
* Deterministic evaluation order (left-to-right).  
* Const/volatile correctness based on the provided tuple.  


Summary
-------
* **Expected** provides simple, exception-free error handling.  
* **For_Each_In_Tuple** provides clean iteration over heterogeneous tuples.  

Both utilities are intentionally small, self-contained, and designed to support
the configuration system without introducing heavy dependencies.
