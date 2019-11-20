===
bnb
===

.. image:: https://travis-ci.org/steinwurf/bnb.svg?branch=master
    :target: https://travis-ci.org/steinwurf/bnb

bnb is a library for reading bits and bytes. It provides an error resilient API
which is useful for parsing untrusteded data.
Additionally it has build-in validation functionality that lets the user verify
the data.

Use as Dependency in CMake
--------------------------

To depend on this project when using the CMake build system, add the following
in your CMake build script:

::

   add_subdirectory("/path/to/bnb" bnb)
   target_link_libraries(<my_target> steinwurf::bnb)

Where ``<my_target>`` is replaced by your target.
