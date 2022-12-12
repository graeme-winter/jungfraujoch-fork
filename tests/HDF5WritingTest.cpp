// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <iostream>
#include <bitshuffle/bitshuffle.h>

#include "../common/DiffractionExperiment.h"
#include "../writer/HDF5Objects.h"
#include "../writer/HDF5Writer.h"
#include "../writer/HDF5NXmx.h"
#include "../compression/JFJochCompressor.h"

using namespace std::literals::chrono_literals;

TEST_CASE("HDF5DataSet_scalar", "[HDF5][Unit]") {
    uint16_t tmp_scalar = 16788;
    {
        HDF5File file("scratch1.h5", true, true, false);
        file.SaveScalar("scalar", tmp_scalar);
    }
    {
        HDF5ReadOnlyFile file("scratch1.h5");
        HDF5DataSet scalar_dataset(file, "scalar");
        HDF5DataType data_type_scalar(scalar_dataset);
        HDF5DataSpace data_space_scalar(scalar_dataset);
        REQUIRE(data_type_scalar.GetElemSize() == 2);
        REQUIRE(data_space_scalar.GetNumOfDimensions() == 0);
        REQUIRE(scalar_dataset.ReadScalar<uint16_t>() == tmp_scalar);
        REQUIRE(scalar_dataset.ReadScalar<int64_t>() == tmp_scalar);
        std::vector<uint16_t> v;
        REQUIRE_THROWS(scalar_dataset.ReadVector(v));
    }
    remove("scratch1.h5");
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5DataSet_string", "[HDF5][Unit]") {
    std::string tmp_string = "HDF5Content";

    {
        HDF5File file("scratch2.h5", true, true, false);
        file.SaveScalar("str", tmp_string);
    }
    {
        HDF5ReadOnlyFile file("scratch2.h5");
        HDF5DataSet string_dataset(file, "str");
        HDF5DataType data_type_str(string_dataset);
        HDF5DataSpace data_space_str(string_dataset);
        CHECK(data_type_str.GetElemSize() == tmp_string.size() + 1);
        CHECK(data_space_str.GetNumOfDimensions() == 0);
        CHECK(string_dataset.ReadString() == tmp_string);
    }
    remove("scratch2.h5");
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5DataSet_vector", "[HDF5][Unit]") {
    std::vector<double> tmp_vector (16384);
    tmp_vector[0] = 599.88;
    tmp_vector[1000] = 800.12;
    tmp_vector[15000] = 3.1415926;

    {
        RegisterHDF5Filter();
        HDF5File file("scratch3.h5", true, true, false);
        file.SaveVector("vec", tmp_vector);
    }

    {
        HDF5ReadOnlyFile file("scratch3.h5");
        HDF5DataSet vector_dataset(file, "vec");
        HDF5DataType data_type_vec(vector_dataset);
        HDF5DataSpace data_space_vec(vector_dataset);
        CHECK(data_type_vec.GetElemSize() == 8);
        CHECK(data_space_vec.GetNumOfDimensions() == 1);
        CHECK(data_space_vec.GetDimensions()[0] == tmp_vector.size());
        std::vector<double> output;
        REQUIRE_NOTHROW(vector_dataset.ReadVector(output));
        CHECK (output[0] == tmp_vector[0]);
        CHECK (output[1000] == tmp_vector[1000]);
        CHECK (output[15000] == tmp_vector[15000]);
        CHECK (output.size() == tmp_vector.size());

        std::vector<double> output2(10);
        REQUIRE_NOTHROW(vector_dataset.ReadVector(output2, {15000},{10}));
        REQUIRE_THROWS(vector_dataset.ReadVector(output2, {15000},{100}));
        CHECK(output2[0] == tmp_vector[15000]);
    }

    remove("scratch3.h5");
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5File_Delete", "[HDF5][Unit]") {
    uint16_t tmp_scalar = 16788;
    {
        HDF5File file("scratch1.h5", true, true, false);
        file.SaveScalar("scalar", tmp_scalar);
    }
    {
        HDF5File file("scratch1.h5", false, true, false);
        std::unique_ptr<HDF5DataSet> scalar_dataset;
        REQUIRE_NOTHROW(scalar_dataset = std::make_unique<HDF5DataSet>(file, "scalar"));
        file.Delete("/scalar");

    }
    {
        HDF5File file("scratch1.h5", false, true, false);
        std::unique_ptr<HDF5DataSet> scalar_dataset;
        REQUIRE_THROWS(scalar_dataset = std::make_unique<HDF5DataSet>(file, "scalar"));
    }
    remove("scratch1.h5");
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5MasterFile", "[HDF5][Full]") {
    {
        RegisterHDF5Filter();
        DiffractionExperiment x(2, {4,4}, 8, 36);
        x.FilePrefix("test01").ImagesPerTrigger(950);

        JFJochProtoBuf::WriterMetadataInput request;
        x.FillWriterMetadata(request);
        REQUIRE_NOTHROW(WriteHDF5MasterFile(request));

        x.FilePrefix("test02");
        REQUIRE_NOTHROW(WriteHDF5MasterFile(request));
    }
    remove("test01_master.h5");
    remove("test02_master.h5");
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5Writer", "[HDF5][Full]") {
    {
        RegisterHDF5Filter();
        DiffractionExperiment x(2, {4,4}, 8, 36);
        std::vector<SpotToSave> spots;

        x.FilePrefix("test02_1p10").ImagesPerTrigger(5).DataFileCount(2).Compression(JFJochProtoBuf::NO_COMPRESSION);
        HDF5Writer file_set(x);
        std::vector<uint16_t> image(x.GetPixelsNum());

        for (int i = 0; i < x.GetImageNum(); i++) {
            REQUIRE_NOTHROW(file_set.Write((const char *) image.data(), x.GetPixelsNum() * x.GetPixelDepth(),
                                           spots, i));
        }
    }
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5Writer_Spots", "[HDF5][Full]") {
    {
        RegisterHDF5Filter();
        DiffractionExperiment x(2, {4,4}, 8, 36);
        std::vector<SpotToSave> spots;

        spots.push_back({10,10,7});
        spots.push_back({20,50,12});
        spots.push_back({1000,500,3});

        x.FilePrefix("test02_1p10_spots").ImagesPerTrigger(5).DataFileCount(2).Compression(JFJochProtoBuf::NO_COMPRESSION);
        HDF5Writer file_set(x);
        std::vector<uint16_t> image(x.GetPixelsNum());

        for (int i = 0; i < x.GetImageNum(); i++) {
            REQUIRE_NOTHROW(file_set.Write((const char *) image.data(), x.GetPixelsNum() * x.GetPixelDepth(),
                                           spots, i));
        }
    }
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5Writer_VDS", "[HDF5][Full]") {
    DiffractionExperiment x(1, {1});
    x.ImagesPerTrigger(5).DataFileCount(5)
     .Compression(JFJochProtoBuf::NO_COMPRESSION).FilePrefix("vds");

    {
        RegisterHDF5Filter();

        HDF5Writer writer(x);
        std::vector<uint16_t> image(x.GetPixelsNum());
        std::vector<SpotToSave> spots;
        for (int i = 0; i < x.GetImageNum(); i++) {
            for (auto &j: image)
                j = i;
            REQUIRE_NOTHROW(writer.Write((const char *) image.data(), x.GetPixelsNum() * x.GetPixelDepth(),
                                         spots, i));
        }

        JFJochProtoBuf::WriterMetadataInput request;
        x.FillWriterMetadata(request);
        request.set_image_number(x.GetImageNum());
        REQUIRE_NOTHROW(WriteHDF5MasterFile(request));
    }
    {
        HDF5File file("vds_master.h5", false, true, false);
        std::unique_ptr<HDF5DataSet> dataset;
        REQUIRE_NOTHROW(dataset = std::make_unique<HDF5DataSet>(file,"/entry/data/data"));
        HDF5DataSpace file_space(*dataset);
        REQUIRE(file_space.GetNumOfDimensions() == 3);
        REQUIRE(file_space.GetDimensions()[0] == x.GetImageNum());
        REQUIRE(file_space.GetDimensions()[1] == x.GetYPixelsNum());
        REQUIRE(file_space.GetDimensions()[2] == x.GetXPixelsNum());

        std::vector<int16_t> v(x.GetImageNum() * x.GetYPixelsNum() * x.GetXPixelsNum());
        std::vector<hsize_t> start = {0, 0, 0};
        dataset->ReadVector(v, start, file_space.GetDimensions());

        REQUIRE(v[0] == 0);
        REQUIRE(v[x.GetYPixelsNum() * x.GetXPixelsNum()] == 1);
        REQUIRE(v[2 * x.GetYPixelsNum() * x.GetXPixelsNum()] == 2);
        REQUIRE(v[3 * x.GetYPixelsNum() * x.GetXPixelsNum()] == 3);
        REQUIRE(v[4 * x.GetYPixelsNum() * x.GetXPixelsNum()] == 4);
    }
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5Writer_VDS_missing", "[HDF5][Full]") {
    DiffractionExperiment x(1, {1});
    x.ImagesPerTrigger(5).DataFileCount(5)
            .Compression(JFJochProtoBuf::NO_COMPRESSION).FilePrefix("vds_missing");

    {
        RegisterHDF5Filter();

        HDF5Writer writer(x);
        std::vector<uint16_t> image(x.GetPixelsNum());
        std::vector<SpotToSave> spots;

        for (int i = 0; i < x.GetImageNum() - 1; i++) {
            for (auto &j: image)
                j = i;
            REQUIRE_NOTHROW(writer.Write((const char *) image.data(), x.GetPixelsNum() * x.GetPixelDepth(),
                                         spots, i));
        }

        JFJochProtoBuf::WriterMetadataInput request;
        x.FillWriterMetadata(request);
        request.set_image_number(x.GetImageNum()-1);
        REQUIRE_NOTHROW(WriteHDF5MasterFile(request));
    }
    {
        HDF5File file("vds_missing_master.h5", false, true, false);
        std::unique_ptr<HDF5DataSet> dataset;
        REQUIRE_NOTHROW(dataset = std::make_unique<HDF5DataSet>(file,"/entry/data/data"));
        HDF5DataSpace file_space(*dataset);
        REQUIRE(file_space.GetNumOfDimensions() == 3);
        REQUIRE(file_space.GetDimensions()[0] == x.GetImageNum()-1);
        REQUIRE(file_space.GetDimensions()[1] == x.GetYPixelsNum());
        REQUIRE(file_space.GetDimensions()[2] == x.GetXPixelsNum());

        std::vector<int16_t> v((x.GetImageNum() - 1) * x.GetYPixelsNum() * x.GetXPixelsNum());
        std::vector<hsize_t> start = {0, 0, 0};
        dataset->ReadVector(v, start, file_space.GetDimensions());

        REQUIRE(v[0] == 0);
        REQUIRE(v[x.GetYPixelsNum() * x.GetXPixelsNum()] == 1);
        REQUIRE(v[2 * x.GetYPixelsNum() * x.GetXPixelsNum()] == 2);
        REQUIRE(v[3 * x.GetYPixelsNum() * x.GetXPixelsNum()] == 3);
    }
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}


TEST_CASE("HDF5Writer_VDS_zero_images", "[HDF5][Full]") {
    DiffractionExperiment x(1, {1});
    x.ImagesPerTrigger(5).DataFileCount(5)
            .Compression(JFJochProtoBuf::NO_COMPRESSION).FilePrefix("vds_zero");

    {
        RegisterHDF5Filter();

        JFJochProtoBuf::WriterMetadataInput request;
        x.FillWriterMetadata(request);
        request.set_image_number(0);
        REQUIRE_NOTHROW(WriteHDF5MasterFile(request));
    }
    {
        std::unique_ptr<HDF5ReadOnlyFile> file;
        std::unique_ptr<HDF5DataSet> dataset;
        REQUIRE_NOTHROW(file = std::make_unique<HDF5ReadOnlyFile>("vds_zero_master.h5"));
        REQUIRE_THROWS(dataset = std::make_unique<HDF5DataSet>(*file,"/entry/data/data"));
    }
    // No leftover HDF5 objects
    REQUIRE (H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL) == 0);
}

TEST_CASE("HDF5NXmx_DataFileName", "[HDF5]") {
    REQUIRE(HDF5Metadata::DataFileName("z/x", 34) == "z/x_data_034.h5");
    REQUIRE(HDF5Metadata::DataFileName("z/x", 0) == "z/x_data_000.h5");
    REQUIRE_THROWS(HDF5Metadata::DataFileName("z/x", 1000));
    REQUIRE_THROWS(HDF5Metadata::DataFileName("z/x", -1));
}

TEST_CASE("HDF5Objects_ExtractFilename", "[HDF5]") {
    REQUIRE(ExtractFilename("filename_data_000001.h5") == "filename_data_000001.h5");
    REQUIRE(ExtractFilename("dir1/filename_data_000001.h5") == "filename_data_000001.h5");
    REQUIRE(ExtractFilename("dir1/dir2/filename_data_000001.h5") == "filename_data_000001.h5");
}


TEST_CASE("HDF5DataType", "[HDF5]") {
    HDF5DataType type1(1,true);
    REQUIRE(type1.GetElemSize() == 1);

    HDF5DataType type2(2,true);
    REQUIRE(type2.GetElemSize() == 2);

    HDF5DataType type4(4,true);
    REQUIRE(type4.GetElemSize() == 4);

    HDF5DataType type8(8,true);
    REQUIRE(type8.GetElemSize() == 8);

    REQUIRE_THROWS(HDF5DataType(7,true));
}