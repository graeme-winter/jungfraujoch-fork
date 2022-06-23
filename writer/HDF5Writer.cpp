// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HDF5Writer.h"
#include "HDF5MasterFile.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include "../common/ImageMetadata.h"

HDF5Writer::HDF5Writer(const DiffractionExperiment &in_experiment) :
experiment(in_experiment), images_remaining(in_experiment.GetFilesNum()), files(in_experiment.GetFilesNum()) {

    if (experiment.GetErrorWhenOverwriting()) {
        ErrorIfFileExists(experiment.GenerateMasterFilename());
        ErrorIfFileExists(experiment.GenerateDataFilename(0));
    }

    MakeDirectory(experiment.GenerateMasterFilename());

    for (int i = 0; i < experiment.GetFilesNum(); i++)
        images_remaining[i] = experiment.GetImagesInFile(i);

    images_per_file = (experiment.GetImagesPerFile() == 0) ? experiment.GetImageNum() : experiment.GetImagesPerFile();
    total_images = experiment.GetImageNum();

    for (int i = 0; i < experiment.GetFilesNum(); i++)
        files[i] = std::make_unique<HDF5DataFile>(experiment, i);
}

void HDF5Writer::WriteFromStream(const uint8_t *msg, int64_t msg_size) {
    if (msg_size <= sizeof(ImageMetadata))
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Image smaller than metadata structure");
    auto image_metadata = (const ImageMetadata *) msg;
    Write(msg + sizeof(ImageMetadata),
          msg_size - sizeof(ImageMetadata),
          image_metadata->frameid);
}

void HDF5Writer::Write(const void *data, size_t data_size, size_t image_number) {
    size_t file_number = experiment.GetFileForImage(image_number);

    if (image_number >= total_images)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Image number above total number of images");
    if (images_remaining[file_number] == 0)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "All images written for the data file");
    if (!files[file_number])
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Error with image numbers");

    // Zero size images won't be written, but will be included in remaining images count
    if (data_size > 0) {
        if (experiment.GetTimeResolvedMode())
            files[file_number]->Write(data, data_size, image_number / experiment.GetFilesNum());
        else
            files[file_number]->Write(data, data_size, image_number % experiment.GetImagesPerFile());
    }
    // Close file if all images written
    if ((--images_remaining[file_number]) == 0)
        files[file_number].reset();
}

size_t HDF5Writer::GetRemainingImageCount() const {
    return std::accumulate(images_remaining.begin(), images_remaining.end(), 0);
}

void HDF5Writer::ErrorIfFileExists(const std::string &path) {
    std::ifstream file(path);
    if (file)
        throw JFJochException(JFJochExceptionCategory::FileWriteError, "File already exists");
}

void HDF5Writer::MakeDirectory(const std::string &path) {
    // If first character is /, then it makes no sense to attempt to create root directory anyway
    size_t pos = path.find('/' , 1);

    while (pos != std::string::npos) {
        std::string dir_name = path.substr(0, pos);

        struct stat info{};

        if( stat( dir_name.c_str(), &info ) == 0) {
            // Something exists in the filesystem, need to check if this is directory or file
            if ( (info.st_mode & S_IFDIR) == 0)
                throw JFJochException(JFJochExceptionCategory::FileWriteError,
                                      "Cannot create directory - file with this name exists");
        } else {
            int ret = mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (ret == -1)
                throw JFJochException(JFJochExceptionCategory::FileWriteError, "Cannot create directory");
        }
        pos = path.find('/', pos + 1);
    }
}