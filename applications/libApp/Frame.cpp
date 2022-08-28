#include "Frame.h"

//Snapshot::Snapshot(const std::string &sensorName, hub::SensorSpec::Format format, const std::vector<int> dims, const hub::Acquisition &acquisition)
//    : mSensorName(sensorName)
//    , mFormat(format)
//    , mDims(dims)
//    , mAcq(new hub::Acquisition(acquisition.clone()))
//{

//}

//Snapshot::Snapshot(const hub::InputSensor &inputSensor, const hub::Acquisition &acq)
//    : mSensorName(inputSensor.m_spec.m_sensorName)
//    , mFormat(inputSensor.m_spec.m_format)
//    , mDims(inputSensor.m_spec.m_dims)
//    , mAcq(new hub::Acquisition(acq.clone()))
//{

//}

//Snapshot::Snapshot(const Snapshot &snapshot)
//    : mSensorName(snapshot.mSensorName)
//    , mFormat(snapshot.mFormat)
//    , mDims(snapshot.mDims)
//    , mAcq(new hub::Acquisition(snapshot.mAcq->clone()))
//{

//}

//Snapshot::~Snapshot()
//{
//    delete mAcq;
//    mAcq = nullptr;
//}

//const std::string &Snapshot::getSensorName() const
//{
//    return mSensorName;
//}

//const hub::Acquisition &Snapshot::getAcq() const
//{
//    return *mAcq;
//}

//hub::SensorSpec::Format Snapshot::getFormat() const
//{
//    return mFormat;
//}

//const std::vector<int> &Snapshot::getDims() const
//{
//    return mDims;
//}
