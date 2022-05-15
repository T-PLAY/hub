#ifndef FRAME_H
#define FRAME_H

#include <stream.h>
#include <vector>

class Snapshot {
public:

    Snapshot(const std::string & sensorName, Stream::Format format, const std::vector<int> dims, const Stream::Acquisition & acquisition);
    Snapshot(const InputStream & inputStream, const Stream::Acquisition & acq);
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
    const Stream::Acquisition &getAcq() const;
    Stream::Format getFormat() const;
    const std::vector<int> &getDims() const;

private:
    std::string mSensorName;
    Stream::Format mFormat;
    std::vector<int> mDims;
    Stream::Acquisition * mAcq = nullptr;
//    std::unique_ptr<Stream::Acquisition&> mAcq;
};

using Frame = std::vector<Snapshot>;
//struct Frame {
////    std::vector<Snapshot> m_snapshots;
//    std::vector<const Snapshot*> m_snapshots;
//};


#endif // FRAME_H
