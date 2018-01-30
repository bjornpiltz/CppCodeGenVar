
===================
Rodrigues' rotation
===================

This is an example of how to use the Symbol class to validate a function. 
The function we are using is `Rodrigues' rotation formula
<https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula>`_ 
which takes a rotation in `Angle-Axis <https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation>`_
form :math:`~(\theta, \mathbf{v})` and transforms a vector :math:`\mathbf{k} \in \mathbb{R}^3`. 

.. math::

   \mathbf{v}_\mathrm{rot} = \mathbf{v} \cos\theta + (\mathbf{k} \times \mathbf{v})
   \sin\theta + \mathbf{k} ~(\mathbf{k} \cdot \mathbf{v}) (1 - \cos\theta)

We use the matrix library `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_ to implement the formula in a way which is efficient while remaining 
almost as expressive as the mathematical notation.

.. literalinclude:: ../examples/rodrigues.cpp
  :language: c++
  :start-after: // https:
  :end-before: using namespace codegenvar

This function can now be used to transform 3D vectors efficiently, but right now we are interested
in testing its mathematical correctness. For reference we will use the class ``Eigen::AngleAxis<T>()``, with ``T=Symbol``.

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/rodrigues.cpp
       :language: c++
       :start-after: // main:
       :end-before: // Check for mathematical equality 
	   
  .. tab:: Output

    .. literalinclude:: rodrigues_example.txt
       :end-before: The two expressions are equal

On a first glance  it is not possible to see if the two expressions are equivalent, 
but the function ``Symbol::equals(const Symbol&)`` tests for mathematical equivalency.

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/rodrigues.cpp
       :language: c++
       :start-after: std::cerr << "\nEigen
       :end-before:  // If we convert
	   
  .. tab:: Output

    .. literalinclude:: rodrigues_example.txt
       :start-after: v_0*(-k_1*sin(theta)
       :end-before: Expanded:
	   
.. NOTE::
  Do not use code like ``a==b`` to test for mathematical equivalency. See :doc:`booleans`.

Finally, we can use the function ``Symbol::expand()`` to transform the expressions to 
their expanded form, which is identical for both expressions.

.. tabs::

  .. tab:: C++

    .. literalinclude:: ../examples/rodrigues.cpp
       :language: c++
       :start-after: << "." << std::endl;
	   
  .. tab:: Output

    .. literalinclude:: rodrigues_example.txt
       :start-after: The two expressions are equal.

You can find the full source code of this example `here 
<https://github.com/bjornpiltz/CppCodeGenVar/blob/master/examples/rotation.cpp>`_.