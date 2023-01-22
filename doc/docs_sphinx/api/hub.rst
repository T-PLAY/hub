API Reference
=============

Sensor
------

.. doxygenclass:: hub::Sensor
   :members: Sensor

SensorSpec
~~~~~~~~~~

.. doxygenclass:: hub::SensorSpec
   :members: SensorNameType, MetaData, SensorSpec

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


.. doxygenclass:: hub::io::InputInterface

.. doxygenclass:: hub::io::OutputInterface

.. doxygenclass:: hub::io::InputOutputInterface

InputInterface
**************

.. doxygenclass:: hub::io::InputInterface

InputStream
###########

.. doxygenclass:: hub::io::InputStream

OutputInterface
***************

.. doxygenclass:: hub::io::OutputInterface

OutputStream
############

.. doxygenclass:: hub::io::OutputStream

ClientSocket
************

.. doxygenclass:: hub::net::ClientSocket
   :members: ClientSocket(ipv4, port, autoConnect)

.. doxygenfunction:: hub::net::ClientSocket::ClientSocket(const std::string &, int, bool)

InputOutputInterface
********************

.. doxygenclass:: hub::io::InputOutputInterface

File
####

.. doxygenclass:: hub::io::File
   :members: File(std::fstream && file)

.. doxygenfunction:: hub::io::File::File(std::fstream && file)

Ram
###

.. doxygenclass:: hub::io::Ram

.. doxygenclass:: hub::io::CyclicBuff
   :members: hello

.. doxygenfunction:: hub::io::CyclicBuff::CyclicBuff(size_t)

InputSensor
-----------

.. doxygenclass:: hub::InputSensor
   :members:


OutputSensor
------------

.. doxygenclass:: hub::OutputSensor


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

Mat4
****

.. doxygenclass:: hub::Mat4



