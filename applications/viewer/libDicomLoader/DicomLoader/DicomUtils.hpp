#pragma once

namespace Ra {
namespace IO {
namespace DICOM {

/**
 * \brief Loads a volume in pvm format. See The Volume Library at
 * http://schorsch.efi.fh-nuernberg.de/data/volume/
 * @param filename name of the file to load
 * @param width     width of the loaded data
 * @param height    height of the loaded data
 * @param depth     depth  of the loaded data
 * @param bytes     number of byte per density value
 * @param scalex    x scale of the voxel
 * @param scaley    y scale of the voxel
 * @param scalez    z scale of the voxel
 * @return  a pointer to the raw density data, nullptr if the file was not loaded.
 */
unsigned char* readDicomVolume( const char* filename,
                              unsigned int* width,
                              unsigned int* height,
                              unsigned int* depth,
                              unsigned int* bytes,
                              float* scalex,
                              float* scaley,
                              float* scalez);

} // namespace DICOM

} // namespace IO
} // namespace Ra
