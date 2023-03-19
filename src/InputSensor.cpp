#include "InputSensor.hpp"

// #define DEBUG_STREAM

namespace hub {


//#ifdef DEBUG
//    assert( m_input->isOpen() );
//    assert( !m_input->isEnd() );
//#endif

//    Acquisition acq = m_input->getAcquisition( m_spec );

//#ifdef DEBUG
//    assert( !acq.hasFixedSize() || acq.getSize() == m_spec.getAcquisitionSize() );
//    const auto& resolutions = m_spec.getResolutions();
//    const auto& measures    = acq.m_measures;
//    assert( resolutions.size() == measures.size() );
//    assert( resolutions.size() > 0 );
//    for ( size_t i = 0; i < resolutions.size(); ++i ) {
//        assert( !res::format2hasFixedSize( resolutions.at( i ).second ) ||
//                res::computeAcquisitionSize( resolutions.at( i ) ) == measures.at( i ).getSize() );
//        assert( measures.at( i ).getResolution() == resolutions.at( i ) );
//        assert( !measures.at( i ).getResolution().first.empty() );
//        assert( measures.at( i ).getResolution().second != Format::NONE );
//    }

//#ifdef DEBUG_STREAM
//    Acquisition acquisition( start, end, data, spec.m_acquisitonSize );
//    std::cout << "[InputSensor] read acq :  " << acquisition << std::endl;
//#endif
//#endif

//    return acq;
//}

Acquisition InputSensor::operator>>( InputSensor& inputSensor ) {

    io::Input & masterInput = *m_input;
    io::Input & input = inputSensor.getInput();
    Acquisition acq;
    masterInput >> input >> acq;
    return acq;

//    //        InputSensor ret(*this);
//    //        InputSensor ret(inputSensor);
//    auto masterAcq = getAcquisition();

////    const auto & input = inputSensor.getInput();

//    //    auto & left = inputSensor.m_lastAcq;
//    auto& lastAcqs = inputSensor.m_lastAcqs;
//    assert( lastAcqs.size() < 10 );

//    if ( lastAcqs.empty() ) {
//        //        left = inputSensor.getAcquisition();
//        lastAcqs.push_back( inputSensor.getAcquisition() );
//        //        lastAcqs.push_back(inputSensor.getAcquisition());
//    }

//    while ( masterAcq.m_start < lastAcqs.front().m_start ) {
//        masterAcq = getAcquisition();
//        std::cout << "[InputSensor] operator>>(InputSensor&) shift masterAcq : " << masterAcq
//                  << std::endl;
//    }

//    //    auto right =
//    while ( lastAcqs.back().m_start < masterAcq.m_start ) {
//        lastAcqs.push_back( inputSensor.getAcquisition() );
//    }

//    if (lastAcqs.back().m_start == masterAcq.m_start) {
//        masterAcq << lastAcqs.back().getMeasures();
//        return masterAcq;
//    }


//    while ( lastAcqs.size() > 2 ) {
//        lastAcqs.pop_front();
//    }


////    assert( lastAcqs.size() == 2 );
//    const auto& left  = lastAcqs.front();
//    const auto& right = lastAcqs.back();

//    assert( left.m_start <= masterAcq.m_start );
//    assert( masterAcq.m_start <= right.m_start );

////    if ( lastAcqs.back().m_start == masterAcq.m_start ) {
////        masterAcq << right.getMeasures();
////        return masterAcq;
////    }

//    const auto& closestAcq = ( std::abs( left.m_start - masterAcq.m_start ) >
//                               std::abs( right.m_start - masterAcq.m_start ) )
//                                 ? ( right )
//                                 : ( left );

////    if (input.isEnd())
////        lastAcqs.pop_front();

////    if (lastAcqs.empty())
////        return Acquisition();

//    masterAcq << closestAcq.getMeasures();
//    return masterAcq;

//    //    const auto dist = std::abs( closestAcq.m_start - masterAcq.m_start );

//    //    // assert(minDist < 20'000); // 20 ms
//    //    // if too far then abort synchronize
//    //    // consider constant period of acquistion rate
//    //    auto maxDist = ( right.m_start - left.m_start ) / 2;

//    //    // find acceptable corresponding acquisition if interpolation is not possible
//    //    if ( !left.isInterpolable() ) {
//    //        const auto& lastMasterAcq = getLastAcq( syncViewerName );
//    //        if ( lastMasterAcq != nullptr ) {
//    //            maxDist =
//    //                std::min( dist, std::abs( lastMasterAcq->m_start() - masterAcq.m_start() ) );
//    //        }
//    //    }

//    //        return ret;
    //        return *this;
}

void InputSensor::operator>>( Acquisition& acquisition ) {
    //    assert(acquisition.m_start == 0);
    //    assert(acquisition.m_end == 0);

    //    assert(acquisition.m_measures.empty());
    //    assert(acquisition.m_size == 0);

    acquisition = m_input->getAcquisition();
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;

    try {
//        int nReadAcqs = 0;
        while ( !m_input->isEnd() ) {
            acqs.emplace_back( m_input->getAcquisition() );
//            ++nReadAcqs;
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[InputSensor] catch exception : " << e.what() << std::endl;
        throw;
    }

    return acqs;
}

io::Input& InputSensor::getInput() const {
    return *m_input;
}

} // namespace hub
