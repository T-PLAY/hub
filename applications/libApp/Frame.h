#ifndef FRAME_H
#define FRAME_H

//#include <vector>

//#include <InputSensor.hpp>

//class Snapshot {
//public:

//    Snapshot(const std::string & sensorName, hub::SensorSpec::Format format, const std::vector<int> dims, const hub::Acquisition & acquisition);
//    Snapshot(const hub::InputSensor & inputSensor, const hub::Acquisition & acq);
//    Snapshot(const Snapshot & snapshot);

//    ~Snapshot();

//    bool operator<(const Snapshot& snapshot) const
//    {
////        return mAcq.m_start < snapshot.mAcq.m_start;
//        return mAcq->m_start < snapshot.mAcq->m_start;
//    }
//    bool operator==(const Snapshot& snapshot) const
//    {
////        return mSensorName == snapshot.mSensorName && mAcq.m_start == snapshot.mAcq.m_start;
//        return mSensorName == snapshot.mSensorName && mAcq->m_start == snapshot.mAcq->m_start;
//    }

//    const std::string & getSensorName() const;
//    const hub::Acquisition &getAcq() const;
//    hub::SensorSpec::Format getFormat() const;
//    const std::vector<int> &getDims() const;

//private:
//    std::string mSensorName;
//    hub::SensorSpec::Format mFormat;
//    std::vector<int> mDims;
//    hub::Acquisition * mAcq = nullptr;
////    std::unique_ptr<Stream::hub::Acquisition&> mAcq;
//};

//using Frame = std::vector<Snapshot>;
////struct Frame {
//////    std::vector<Snapshot> m_snapshots;
////    std::vector<const Snapshot*> m_snapshots;
////};


#endif // FRAME_H
