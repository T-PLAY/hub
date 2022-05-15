#include "Frame.h"

Snapshot::Snapshot(const std::string &sensorName, Stream::Format format, const std::vector<int> dims, const Stream::Acquisition &acquisition)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    , mAcq(new Stream::Acquisition(acquisition.clone()))
{

}

Snapshot::Snapshot(const InputStream &inputStream, const Stream::Acquisition &acq)
    : mSensorName(inputStream.getSensorName())
    , mFormat(inputStream.getFormat())
    , mDims(inputStream.getDims())
    , mAcq(new Stream::Acquisition(acq.clone()))
{

}

Snapshot::Snapshot(const Snapshot &snapshot)
    : mSensorName(snapshot.mSensorName)
    , mFormat(snapshot.mFormat)
    , mDims(snapshot.mDims)
    , mAcq(new Stream::Acquisition(snapshot.mAcq->clone()))
{

}

Snapshot::~Snapshot()
{
    delete mAcq;
    mAcq = nullptr;
}

const std::string &Snapshot::getSensorName() const
{
    return mSensorName;
}

const Stream::Acquisition &Snapshot::getAcq() const
{
    return *mAcq;
}

Stream::Format Snapshot::getFormat() const
{
    return mFormat;
}

const std::vector<int> &Snapshot::getDims() const
{
    return mDims;
}
