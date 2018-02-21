====================
Eigen matrix inverse
====================

This is an example of how to use the Symbol class to see what is going on
inside the `Eigen Library <http://eigen.tuxfamily.org>`_ Eigen is a 
templated matrix library which takes an arbitrary scalar type. Through 
the include ``<codegenvar/Eigen>`` we can access the following typedefs:

.. literalinclude:: ../include/codegenvar/SymbolWithEigenBindings.h
  :language: c++
  :start-after:   // namespace Eigen
  :end-before: /* For example:

First we will test Eigen's 2x2 fixed size implementation of the inverse function.

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/eigen_inverse.cpp
      :language: c++
      :start-after: using namespace codegenvar
      :end-before: BooleanEvaluator
	  
  .. tab:: Output
  
    .. literalinclude:: eigen_inverse_example.txt
      :end-before: m11' =
	  
We can also test the inverse function for dynamically sized matrices.
This gets a little bit more involved since the code contains 
several :doc:`conditional_example`.

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/eigen_inverse.cpp
      :language: c++
      :start-after: std::cout << "m' = "
      :end-before: return 0;
	  
  .. tab:: Output
  
    .. literalinclude:: eigen_inverse_example.txt
      :start-after: -m_21/(-m_21

You can find the full source code of this example `here 
<https://github.com/bjornpiltz/CppCodeGenVar/blob/master/examples/eigen_inverse.cpp>`_.