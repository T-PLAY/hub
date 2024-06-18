API Reference
=============

.. //////////////////////////////////// Sensor /////////////////////////////////////

----

Basics
------

Sensor
******

.. image:: ../_build/html/classhub_1_1sensor_1_1Sensor.png
   :align: center

|

.. doxygenclass:: hub::sensor::Sensor
   :members: Sensor
..   :members: Sensor(const SensorSpec& sensorSpec)

.. doxygenfunction:: hub::sensor::Sensor::getSpec
.. .. doxygenfunction:: hub::sensor::Sensor::acqMsg

SensorSpec
***********

.. doxygenclass:: hub::sensor::SensorSpec
   :members: SensorNameType, MetaData, SensorSpec

**Instantiate examples :**

.. literalinclude:: ../../../examples/sensor/example-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. literalinclude:: ../../../examples/sensor/example-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction2
   :end-before: endConstruction2

.. literalinclude:: ../../../examples/sensor/example-sensor-sensorSpec.cpp
   :language: cpp
   :start-after: startConstruction3
   :end-before: endConstruction3

.. .. doxygentypedef:: hub::Matrix

Resolution
**********

Dims
####

.. doxygentypedef:: hub::Dims

Matrix
######

.. doxygenclass:: hub::Matrix
    :members: getDims

**Instantiate examples :**

.. literalinclude:: ../../../examples/core/example-core-matrix.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

MatrixXD
########

.. doxygenclass:: hub::MatrixXD
    :members: getDims

**Instantiate examples :**

.. literalinclude:: ../../../examples/core/example-core-matrixXD.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

MatrixTs
########

.. doxygenclass:: hub::MatrixTs
    :members: getDims

**Instantiate examples :**

.. literalinclude:: ../../../examples/core/example-core-matrixTs.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

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



OutputSensor
************

.. doxygenclass:: hub::sensor::OutputSensor
   :members: OutputSensor, operator<<

**Instantiate examples :**

.. literalinclude:: ../../../examples/sensor/example-sensor-outputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction


InputSensor
***********

.. doxygenclass:: hub::sensor::InputSensor
   :members: InputSensor, operator>>

**Instantiate examples :**

.. literalinclude:: ../../../examples/sensor/example-sensor-inputSensor.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction


Acquisition
***********

.. doxygenclass:: hub::sensor::Acquisition
   :members: Acquisition, isInterpolable, clone, getSize, getMeasures, slerp, operator<<

|

.. //////////////////////////////////// IO /////////////////////////////////////

----

IO
--


Input
*****


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

.. literalinclude:: ../../../examples/io/example-io-inputStream.cpp
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

Output
******


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

.. literalinclude:: ../../../examples/io/example-io-outputStream.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

|


InputOutput
***********

.. image:: ../_build/html/classhub_1_1io_1_1InputOutput.png
   :align: center

|

.. .. doxygenclass:: hub::io::InputOutput

ClientSocket
############

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

.. literalinclude:: ../../../examples/io/example-io-file.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction


.. //////////////////////////////////// Format /////////////////////////////////////

----

Format
------

.. .. doxygenclass:: hub::data

Dof6
****

.. doxygenstruct:: hub::format::Dof6

Mat4
****

.. doxygenstruct:: hub::format::Mat4


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
#############

.. doxygenclass:: hub::client::ViewerServer2
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/client/example-client-viewer.cpp
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
############

.. doxygenclass:: hub::client::AskerServer2
   :members:

**Instantiate examples :**

.. literalinclude:: ../../../examples/client/example-client-asker.cpp
   :language: cpp
   :start-after: startConstruction
   :end-before: endConstruction

.. AskerMqtt
.. ~~~~~~~~~~~~

.. .. doxygenclass:: hub::client::AskerMqtt
   :members:

