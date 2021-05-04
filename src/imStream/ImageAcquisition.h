#ifndef HUBV2_IMAGEACQUISITION_H
#define HUBV2_IMAGEACQUISITION_H

class ImageAcquisition {
public:
    virtual std::vector<unsigned char> image() const = 0;
    virtual bool connected() const = 0;

    virtual bool imagePending() const = 0;
};


#endif