API Reference
=============

Sensor
------

.. image:: ../_build/html/classhub_1_1Sensor.png
   :align: center

.. doxygenclass:: hub::Sensor
   :members: Sensor


SensorSpec
~~~~~~~~~~

.. doxygenclass:: hub::SensorSpec
   :members: SensorNameType, MetaData, SensorSpec

**Instantiate examples :**

.. literalinclude:: ../../../examples/sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

.. doxygentypedef:: hub::Resolutions

Resolution
**********

.. doxygentypedef:: hub::Resolution

Dims
****

.. doxygentypedef:: hub::Dims

Format
******

.. doxygenenum:: hub::Format

Interface
~~~~~~~~~



.. doxygenclass:: hub::io::Interface
    :members: isOpen, isEnd, close
    :protected-members:

..
    .. doxygenclass:: hub::io::OutputInterface
    .. doxygenclass:: hub::io::InputInterface
    .. doxygenclass:: hub::io::InputOutputInterface

OutputInterface
***************

.. image:: ../_build/html/classhub_1_1io_1_1OutputInterface.png
   :align: center

|

.. doxygenclass:: hub::io::OutputInterface

OutputStream
############

.. doxygenclass:: hub::io::OutputStream

**Instantiate example :**

.. literalinclude:: ../../../examples/outputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

InputInterface
**************

.. image:: ../_build/html/classhub_1_1io_1_1InputInterface.png
   :align: center

|

.. doxygenclass:: hub::io::InputInterface


InputStream
###########

.. doxygenclass:: hub::io::InputStream

**Instantiate example :**

.. literalinclude:: ../../../examples/inputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction


Socket
************

.. image:: ../_build/html/classhub_1_1net_1_1Socket.png
   :align: center

.. doxygenclass:: hub::net::Socket

ClientSocket
************

.. doxygenclass:: hub::net::ClientSocket
   :members: ClientSocket(ipv4, port, autoConnect)

.. doxygenfunction:: hub::net::ClientSocket::ClientSocket(const std::string &, int, bool)

InputOutputInterface
********************

.. image:: ../_build/html/classhub_1_1io_1_1InputOutputInterface.png
   :align: center

|

.. doxygenclass:: hub::io::InputOutputInterface

File
####

.. doxygenclass:: hub::io::File
   :members: File(std::fstream && file)

.. doxygenfunction:: hub::io::File::File(std::fstream && file)

.. literalinclude:: ../../../examples/file.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

Ram
###

.. doxygenclass:: hub::io::Ram

.. doxygenclass:: hub::io::CyclicBuff
   :members: hello

.. doxygenfunction:: hub::io::CyclicBuff::CyclicBuff(size_t)

.. literalinclude:: ../../../examples/ram.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

OutputSensor
------------

.. doxygenclass:: hub::OutputSensor

**Instantiate examples :**

.. literalinclude:: ../../../examples/outputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/outputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/outputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../examples/outputSensor.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional

InputSensor
-----------

.. doxygenclass:: hub::InputSensor
   :members:

.. literalinclude:: ../../../examples/inputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/inputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/inputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../examples/inputSensor.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional

Acquisition
-----------

.. doxygenclass:: hub::Acquisition
   :members: Acquisition, isInterpolable, clone, getSize, getMeasures, slerp, operator<<

Measure
~~~~~~~

.. doxygenclass:: hub::Measure
   :members: Measure, clone, isInterpolable, getResolution, slerp

Dof6
****

.. doxygenclass:: hub::Dof6
   :members: Dof6

Mat4
****

.. doxygenclass:: hub::Mat4



