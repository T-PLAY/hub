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

.. literalinclude:: ../../../examples/example-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/example-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/example-sensorSpec.cpp
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

.. literalinclude:: ../../../examples/example-outputStream.cpp
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

.. literalinclude:: ../../../examples/example-inputStream.cpp
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

.. literalinclude:: ../../../examples/example-file.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

Ram
###

.. doxygenclass:: hub::io::Ram

.. doxygenclass:: hub::io::CyclicBuff
   :members: hello

.. doxygenfunction:: hub::io::CyclicBuff::CyclicBuff(size_t)

.. literalinclude:: ../../../examples/example-ram.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

OutputSensor
------------

.. doxygenclass:: hub::OutputSensor

**Instantiate examples :**

.. literalinclude:: ../../../examples/streamers/example-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/streamers/example-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/streamers/example-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../examples/streamers/example-outputSensor.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional

InputSensor
-----------

.. doxygenclass:: hub::InputSensor
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/viewers/example-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/viewers/example-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/viewers/example-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../examples/viewers/example-inputSensor.cpp
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


Streamer
-----------

.. doxygenclass:: hub::Streamer
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/streamers/example-streamer.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

**Functional example :**

.. literalinclude:: ../../../examples/streamers/example-streamer.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional
 

Viewer
-----------

.. doxygenclass:: hub::Viewer
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/viewers/example-viewer.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction
 
