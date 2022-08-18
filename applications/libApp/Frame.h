#ifndef FRAME_H
#define FRAME_H

#include <vector>

#include "Sensor.hpp"

class Snapshot {
public:

    Snapshot(const std::string & sensorName, hub::Header::Format format, const std::vector<int> dims, const hub::Acquisition & acquisition);
    Snapshot(const hub::InputSensor & inputSensor, const hub::Acquisition & acq);
    Snapshot(const Snapshot & snapshot);

    ~Snapshot();

    bool operator<(const Snapshot& snapshot) const
    {
//        return mAcq.mBackendTimestamp < snapshot.mAcq.mBackendTimestamp;
        return mAcq->mBackendTimestamp < snapshot.mAcq->mBackendTimestamp;
    }
    bool operator==(const Snapshot& snapshot) const
    {
//        return mSensorName == snapshot.mSensorName && mAcq.mBackendTimestamp == snapshot.mAcq.mBackendTimestamp;
        return mSensorName == snapshot.mSensorName && mAcq->mBackendTimestamp == snapshot.mAcq->mBackendTimestamp;
    }

    const std::string & getSensorName() const;
    const hub::Acquisition &getAcq() const;
    hub::Header::Format getFormat() const;
    const std::vector<int> &getDims() const;

private:
    std::string mSensorName;
    hub::Header::Format mFormat;
    std::vector<int> mDims;
    hub::Acquisition * mAcq = nullptr;
//    std::unique_ptr<Stream::hub::Acquisition&> mAcq;
};

using Frame = std::vector<Snapshot>;
//struct Frame {
////    std::vector<Snapshot> m_snapshots;
//    std::vector<const Snapshot*> m_snapshots;
//};


#endif // FRAME_H
