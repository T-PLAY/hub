API Reference
=============

.. //////////////////////////////////// Sensor /////////////////////////////////////

----

Sensor
------

.. image:: ../_build/html/classhub_1_1sensor_1_1Sensor.png
   :align: center

|

.. doxygenclass:: hub::sensor::Sensor
   :members: Sensor

SensorSpec
***********

.. doxygenclass:: hub::sensor::SensorSpec
   :members: SensorNameType, MetaData, SensorSpec

**Instantiate examples :**

.. literalinclude:: ../../../doc/sensor/doc-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../doc/sensor/doc-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../doc/sensor/doc-sensor-sensorSpec.cpp
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

.. doxygenenum:: hub::Types::Format

.. InputOutput
.. ~~~~~~~~~~~
..
..
..
.. .. doxygenclass:: hub::io::InputOutput
..     :members: isOpen, isEnd, close
..     :protected-members:
..
.. ..
    .. doxygenclass:: hub::io::OutputInterface
    .. doxygenclass:: hub::io::InputInterface
    .. doxygenclass:: hub::io::InputOutputInterface

InputOutput
***********

.. image:: ../_build/html/classhub_1_1io_1_1InputOutput.png
   :align: center

|

.. .. doxygenclass:: hub::io::InputOutput

Output
~~~~~~

.. image:: ../_build/html/classhub_1_1io_1_1Output.png
   :align: center

|

.. .. doxygenclass:: hub::io::Output

OutputStream
############

.. doxygenclass:: hub::io::OutputStream

**Instantiate example :**

.. literalinclude:: ../../../examples/example-outputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

Input
~~~~~

.. image:: ../_build/html/classhub_1_1io_1_1Input.png
   :align: center

|

.. .. doxygenclass:: hub::io::InputInterface


InputStream
###########

.. doxygenclass:: hub::io::InputStream

**Instantiate example :**

.. literalinclude:: ../../../examples/example-inputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

Socket
******

.. image:: ../_build/html/classhub_1_1net_1_1Socket.png
   :align: center

.. doxygenclass:: hub::net::Socket

ClientSocket
************

.. doxygenclass:: hub::net::ClientSocket
   :members: ClientSocket(ipv4, port, autoConnect)

.. doxygenfunction:: hub::net::ClientSocket::ClientSocket(const std::string &, int, bool)


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
************

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
***********

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
***********

.. doxygenclass:: hub::Acquisition
   :members: Acquisition, isInterpolable, clone, getSize, getMeasures, slerp, operator<<

Format
------

.. .. doxygenclass:: hub::data

Dof6
####

.. doxygenclass:: hub::format::Dof6
   :members: Dof6

Mat4
####

.. doxygenclass:: hub::format::Mat4

|

.. //////////////////////////////////// Client /////////////////////////////////////

----

Client
------

.. .. doxygennamespace:: hub::client

Streamer
********

.. doxygenclass:: hub::client::Streamer
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/example-streamer.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

**Functional example :**

.. literalinclude:: ../../../examples/example-streamer.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional
 

Viewer
******

.. doxygenclass:: hub::client::Viewer
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/example-viewer.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction
 
Asker
*****

.. doxygenclass:: hub::client::Asker
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/viewers/example-asker.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

