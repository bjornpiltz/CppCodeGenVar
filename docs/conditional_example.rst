======================
Conditional statements
======================

Code containing conditional statements poses a challenge. Conditional statement
are created by invoking one of the following boolean operators, or their 
overloads where one size is a primitive number type.

.. literalinclude:: ../include/codegenvar/Symbol.h
  :language: c++
  :start-after: // Booleans:
  :end-before: // Symbol op Scalar

Take for example the following function:


.. literalinclude:: ../examples/conditional.cpp
  :language: c++
  :start-after: template
  :end-before: // main:

We cannot evaluate this function by simply calling it once, since
there are three different paths through the code depending how
the expressions ``y == 0`` and ``x/y < 0`` evaluate. Trying to do 
so will cause a run time exception to be thrown:

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/conditional.cpp
       :language: c++
       :start-after: main
       :end-before: // Fix: 
	   
  .. tab:: Output

    .. literalinclude:: conditional_example.txt
       :language: none
       :lines: 1,2

------------------------------
BooleanEvaluator to the rescue
------------------------------
Here is how to fix the above example:

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/conditional.cpp
       :language: c++
       :start-after: // Fix:
       :end-before: return 
	   
  .. tab:: Output

    .. literalinclude:: conditional_example.txt
       :language: none
       :lines: 4-

You can find the full source code of this example `here 
<https://github.com/bjornpiltz/CppCodeGenVar/blob/master/examples/conditional.cpp>`_.