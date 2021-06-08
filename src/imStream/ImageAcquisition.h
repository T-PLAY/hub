#ifndef HUBV2_IMAGEACQUISITION_H
#define HUBV2_IMAGEACQUISITION_H

#include <vector>

enum DataType {
    RAW=0,
    COMPRESSED
};

class ImageAcquisition {
public:
    virtual std::vector<unsigned char> image() = 0;
    virtual bool connected() const = 0;

    virtual bool imagePending() const = 0;

    virtual bool start() = 0;
    virtual void stop() = 0;

    inline int dataType() const {
        return _dataType;
    }

protected:
    DataType _dataType = COMPRESSED;

};


#endif