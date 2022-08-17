#include "Frame.h"

Snapshot::Snapshot(const std::string &sensorName, Header::Format format, const std::vector<int> dims, const Acquisition &acquisition)
    : mSensorName(sensorName)
    , mFormat(format)
    , mDims(dims)
    , mAcq(new Acquisition(acquisition.clone()))
{

}

Snapshot::Snapshot(const InputStream &inputStream, const Acquisition &acq)
    : mSensorName(inputStream.getHeader().getSensorName())
    , mFormat(inputStream.getHeader().getFormat())
    , mDims(inputStream.getHeader().getDims())
    , mAcq(new Acquisition(acq.clone()))
{

}

Snapshot::Snapshot(const Snapshot &snapshot)
    : mSensorName(snapshot.mSensorName)
    , mFormat(snapshot.mFormat)
    , mDims(snapshot.mDims)
    , mAcq(new Acquisition(snapshot.mAcq->clone()))
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

const Acquisition &Snapshot::getAcq() const
{
    return *mAcq;
}

Header::Format Snapshot::getFormat() const
{
    return mFormat;
}

const std::vector<int> &Snapshot::getDims() const
{
    return mDims;
}
