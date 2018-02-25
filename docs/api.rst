================
The Symbol class
================

.. doxygenclass:: codegenvar::Symbol

------------
Constructors
------------
A Symbol object can be initialized as a named symbol or a constant.

.. code-block:: c++

   Symbol x("x"), y("y"), zero(0), c3(3.0);
   auto vars = Symbol::Array("a", "b", "c", 0, 3.14);

.. doxygengroup:: symbol_ctors
  :content-only:
  
-----------------------
Mathematical operations
-----------------------
The :cpp:class:`codegenvar::Symbol` class has support for the following mathematical operations. 

.. doxygengroup:: symbol_math_ops
  :content-only:

----------------------
Mathematical functions
----------------------
The :cpp:class:`codegenvar::Symbol` class has support for the following mathematical operations. 

.. doxygengroup:: symbol_math_functions
  :content-only:

------------------
Logical operations
------------------
The :cpp:class:`codegenvar::Symbol` class has support for the following boolean operations. 

.. NOTE::

  If you plan to use these boolean operations, have a look at :doc:`conditional_example`.

.. doxygengroup:: symbol_logical_ops
  :content-only:
