
#include <thread>

#include <sensor/OutputSensor.hpp>

class Test
{
  public:
    Test() {

        hub::MetaData metaData;
        metaData["manufactor"] = "My Company";
        constexpr int width    = 640;
        constexpr int height   = 480;
        // using Resolution       = hub::MatrixXD<hub::format::Y8, width, height>;
        auto resolution = hub::make_matrix<hub::format::Y8>( 640, 480 );
        hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );
        // m_streamViews->onNewStream( "stream 0", sensorSpec );

        // bool m_exitThread = false;
        // m_thread = std::thread( [this, resolution = std::move(resolution)]() {
        m_thread = std::thread( [this]() {
            auto resolution2 = hub::make_matrix<hub::format::Y8>( 640, 480 );
            // hub::sensor::AcquisitionT<Resolution> acq;
            // auto acq = hub::sensor::AcquisitionT<Resolution>().getMatrix();
            auto acq                 = hub::sensor::make_acquisition( resolution2 );
            auto& start              = acq.start();
            auto& end                = acq.end();
            hub::format::Y8* imgData = acq.get<hub::format::Y8*>();
            constexpr size_t imgSize = width * height;

            constexpr auto maxFps = 40.0;

            int dec = 0;
            while ( !m_exitThread ) {
                const auto startClock = std::chrono::high_resolution_clock::now();
                start                 = hub::sensor::getClock();

                for ( int i = 0; i < height; ++i ) {
                    for ( int j = 0; j < width; ++j ) {
                        assert( i * width + j < imgSize );
                        imgData[i * width + j].y = ( i + j + dec ) / 5 % 256;
                    }
                }

                const auto endClock =
                    startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );

                end = hub::sensor::getClock();
                dec += 50;

                // m_streamViews->onNewAcquisition("streamName", acq);
                std::cout << "acq: " << acq << std::endl;

                std::this_thread::sleep_until( endClock );
            }
        } );
    }

    ~Test() {
        m_exitThread = true;
        m_thread.join();
    }

  private:
    bool m_exitThread = false;
    std::thread m_thread;
};

int main() {
    // std::cout << "resolution: " << resolution << std::endl;
    Test test;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    return 0;
}
