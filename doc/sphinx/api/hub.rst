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

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

.. .. doxygentypedef:: hub::Matrix

Resolution
**********

.. doxygenclass:: hub::Matrix
    :members: Matrix

.. Dims
.. ****
..
.. .. doxygentypedef:: hub::Dims
..
.. Format
.. ******
..
.. .. doxygenenum:: hub::Types::Format

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

OutputBase
##########

.. image:: ../_build/html/classhub_1_1OutputBase.png
   :align: center

.. doxygenclass:: hub::OutputBase


.. .. image:: ../_build/html/classhub_1_1OutputT.png
   :align: center

|

.. .. doxygenclass:: hub::io::Output

OutputStream
############

.. doxygenclass:: hub::output::OutputStreamServer2
   :members: OutputStreamServer2(header, streamPort)
..   :members: OutputStreamServer2(header, streamName, port, ipv4)
.. .. doxygentypedef:: hub::ouptut::OutputStream

**Instantiate example :**

.. literalinclude:: ../../../doc/cpp/io/doc-io-outputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

Input
~~~~~

.. .. image:: ../_build/html/classhub_1_1input_1_1Input.png
..    :align: center
.. image:: ../_build/html/classhub_1_1InputBase.png
   :align: center

.. image:: ../_build/html/classhub_1_1InputT.png
   :align: center

|

.. .. doxygenclass:: hub::io::InputInterface


InputStream
###########

.. doxygenclass:: hub::input::InputStreamServer2

**Instantiate example :**

.. literalinclude:: ../../../doc/cpp/io/doc-io-inputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. Socket
.. ******
..
.. .. image:: ../_build/html/classhub_1_1net_1_1Socket.png
..    :align: center
..
.. .. doxygenclass:: hub::net::Socket

ClientSocket
************

.. doxygenclass:: hub::net::ClientSocketI

ClientSocketSystem
~~~~~~~~~~~~~~~~~~

.. doxygenclass:: hub::net::system::ClientSocketSystem
   :members: ClientSocketSystem(ipv4, port, autoConnect)

.. .. doxygenfunction:: hub::net::ClientSocketSystem::ClientSocketSystem(const std::string &, int, bool)

.. ClientSocketBoost
.. ~~~~~~~~~~~~~~~~~~

.. .. doxygenclass:: hub::net::boost::ClientSocketBoost
   :members: ClientSocketBoost(ipv4&, port, autoConnect)

.. .. doxygenfunction:: hub::net::ClientSocketBoost::ClientSocketBoost(const std::string &, int, bool)


File
####

.. doxygenclass:: hub::io::File
   :members: File(const std::string& filePath)

.. doxygenfunction:: hub::io::File::File(const char* filePath)

.. literalinclude:: ../../../doc/cpp/io/doc-io-file.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction


OutputSensor
************

.. doxygenclass:: hub::sensor::OutputSensor

**Instantiate examples :**

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-outputSensor.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional

InputSensor
***********

.. doxygenclass:: hub::sensor::InputSensor
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

**Functional example :**

.. literalinclude:: ../../../doc/cpp/sensor/doc-sensor-inputSensor.cpp
   :language: cpp
   :start-after: startFunctional
   :end-before: endFunctional

Acquisition
***********

.. doxygenclass:: hub::sensor::Acquisition
   :members: Acquisition, isInterpolable, clone, getSize, getMeasures, slerp, operator<<

Format
------

.. .. doxygenclass:: hub::data

Dof6
####

.. doxygenstruct:: hub::format::Dof6

Mat4
####

.. doxygenstruct:: hub::format::Mat4

|

.. //////////////////////////////////// Client /////////////////////////////////////

----

Client
------

.. .. doxygennamespace:: hub::client


Viewer
******

.. doxygenclass:: hub::client::ViewerInterface
   :members:


ViewerServer2
~~~~~~~~~~~~~

.. doxygenclass:: hub::client::ViewerServer2
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../doc/cpp/client/doc-client-viewer.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. ViewerMqtt
.. ~~~~~~~~~~~~~

.. .. doxygenclass:: hub::client::ViewerMqtt
   :members:

 
Asker
*****

.. doxygenclass:: hub::client::AskerInterface

AskerServer2
~~~~~~~~~~~~

.. doxygenclass:: hub::client::AskerServer2
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../doc/cpp/client/doc-client-asker.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. AskerMqtt
.. ~~~~~~~~~~~~

.. .. doxygenclass:: hub::client::AskerMqtt
   :members:

