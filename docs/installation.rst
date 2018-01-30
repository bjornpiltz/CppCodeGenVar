============
Installation
============

For now the you need to build the library yourself. 
The requirements are:

* `CMake <https://cmake.org/>`_
* A C++11 enabled compiler (GCC >= 4.8, Clang >= 3.9, MSVC >= 2015)
* The C++ library `SymEngine <https://github.com/symengine/symengine>`_

.. code-block:: bash

   $ git clone https://github.com/bjornpiltz/CppCodeGenVar.git
   $ cd CppCodeGenVar
   $ cmake . -DCMAKE_INSTALL_DIR=<optional_install_location>
   $ make check
   $ make install