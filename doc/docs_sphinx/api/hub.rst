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
    :members:


.. doxygenfunction:: hub::InputSensor::InputSensor(InputInterface &&inputInterface)
.. doxygenfunction:: hub::OutputSensor::OutputSensor(SensorSpec &&sensorSpec, OutputInterface &&outputInterface)

.. doxygenclass:: hub::io::InputInterface
.. doxygendefine:: CONSTEXPR20
