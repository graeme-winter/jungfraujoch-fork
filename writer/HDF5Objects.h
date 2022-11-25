// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HDF5OBJECTS_H
#define JUNGFRAUJOCH_HDF5OBJECTS_H

#include <hdf5.h>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "../common/JFJochException.h"
#include "../compression/CompressionAlgorithmEnum.h"

extern std::mutex hdf5_mutex;

class HDF5DataSet;

class HDF5Id {
protected:
    hid_t id;
    HDF5Id() = default;
    HDF5Id(HDF5Id &&other) noexcept;
    HDF5Id(const HDF5Id& other);
public:
    // Assignment would require closing ID's first and cannot be generalized easily
    HDF5Id& operator=(HDF5Id &&other) noexcept = delete;
    HDF5Id& operator=(const HDF5Id &other) noexcept = delete;
    hid_t GetID() const;
};

class HDF5DataSpace : public HDF5Id {
    uint8_t ndims = 1;
public:
    explicit HDF5DataSpace(const std::vector<hsize_t>& dims = {1}, const std::vector<hsize_t> &max_dims = {});
    explicit HDF5DataSpace(const HDF5DataSet& data_set);
    uint8_t GetNumOfDimensions() const;
    std::vector<hsize_t> GetDimensions() const;
    void SelectHyperslab(const std::vector<hsize_t>& start, const std::vector<hsize_t>& size);
    void SelectHyperslabWithStride(const std::vector<hsize_t>& start, const std::vector<hsize_t>& size,
                                   const std::vector<hsize_t>& stride);
    ~HDF5DataSpace();
};

class HDF5DataType : public HDF5Id {
public:
    HDF5DataType(uint64_t size_in_bytes, bool is_signed);
    explicit HDF5DataType(double val);
    explicit HDF5DataType(float val);
    explicit HDF5DataType(bool val);
    explicit HDF5DataType(int32_t val);
    explicit HDF5DataType(uint32_t val);
    explicit HDF5DataType(int64_t val);
    explicit HDF5DataType(uint64_t val);
    explicit HDF5DataType(int16_t val);
    explicit HDF5DataType(uint16_t val);
    explicit HDF5DataType(int8_t val);
    explicit HDF5DataType(uint8_t val);
    explicit HDF5DataType(const std::string &val);
    explicit HDF5DataType(const char *val);
    explicit HDF5DataType(const HDF5DataSet& data_set);
    size_t GetElemSize() const;
    ~HDF5DataType();
};

class HDF5Dcpl : public HDF5Id {
    uint8_t ndim = 0;
public:
    HDF5Dcpl();
    ~HDF5Dcpl();
    void SetFillValue32(int32_t val);
    void SetFillValue16(int16_t val);
    void SetChunking(const std::vector<hsize_t> & dims);
    void SetCompression(CompressionAlgorithm algorithm, size_t elem_size, size_t block_size);
    void SetVirtual(const std::string& filename, const std::string& dataset, const HDF5DataSpace& src_dataspace,
            const HDF5DataSpace& virtual_dataspace);

    uint8_t GetNumOfDimensions() const;
};

class HDF5Fapl : public HDF5Id {
public:
    HDF5Fapl();
    ~HDF5Fapl();
    void SetVersionToLatest();
};

class HDF5Object : public HDF5Id {
public:
    HDF5Object& Attr(const std::string& name, double val);
    HDF5Object& Attr(const std::string& name, const std::string& val);
    HDF5Object& Attr(const std::string& name, int32_t val);
    HDF5Object& Attr(const std::string& name, uint32_t val);
    HDF5Object& Attr(const std::string& name, int64_t val);
    HDF5Object& Attr(const std::string& name, uint64_t val);
    HDF5Object& Attr(const std::string& name, const std::vector<double> &val);
    HDF5Object& Units(const std::string& val);
    HDF5Object& NXClass(const std::string& val);
    HDF5Object& Transformation(const std::string& units, const std::string& depends_on,
            const std::string& equipment, const std::string& equipment_component,
            const std::string& transformation_type, const std::vector<double> &vec);
    HDF5Object& Transformation(const std::string& units, const std::string& depends_on,
                               const std::string& equipment, const std::string& equipment_component,
                               const std::string& transformation_type, const std::vector<double>& vector,
                               const std::vector<double> &offset, const std::string& offset_units);
    HDF5Object& ExternalLink(std::string filename, const std::string &external_dataset, const std::string &local_name);
    HDF5Object& HardLink(const std::string &source, const std::string &dest);
    std::unique_ptr<HDF5DataSet> SaveScalar(const std::string &name, const char *val);
    std::unique_ptr<HDF5DataSet> SaveScalar(const std::string &name, const std::string& val);
    std::unique_ptr<HDF5DataSet> SaveVector(const std::string &name, const std::vector<std::string> &val);

    template <class T> std::unique_ptr<HDF5DataSet> SaveScalar(const std::string &name, T val);
    template <class T> std::unique_ptr<HDF5DataSet> SaveVector(const std::string &name, const std::vector<T> &val,
            std::vector<hsize_t> dim = {}, CompressionAlgorithm algorithm = CompressionAlgorithm::NO_COMPRESSION);
};

class HDF5Group : public HDF5Object {
public:
    HDF5Group(const HDF5Object& parent, const std::string& name);
    HDF5Group(const HDF5Object& parent, const char *name);
    ~HDF5Group();

};

class HDF5File : public HDF5Object {
public:
    HDF5File(const std::string& filename,
             bool create, bool hdf5_latest, bool swmr);
    ~HDF5File();
    void StartSWMR();
    void Delete(const std::string& path);
};

class HDF5ReadOnlyFile : public HDF5Object {
public:
    HDF5ReadOnlyFile(const std::string& filename);
    ~HDF5ReadOnlyFile();
};

class HDF5DataSet : public HDF5Object {
    uint8_t ndim;
public:
    HDF5DataSet(const HDF5Object& parent, const std::string& name); // Open existing dataset
    HDF5DataSet(const HDF5Object& parent, const std::string& name, const HDF5DataType &data_type,
                const HDF5DataSpace &data_space, const HDF5Dcpl &dcpl);
    HDF5DataSet(const HDF5Object &parent, const std::string &name, const HDF5DataType &data_type,
                const HDF5DataSpace &data_space);
    ~HDF5DataSet();
    HDF5DataSet& Write(const HDF5DataType &data_type, const void *val);
    HDF5DataSet& WriteDirectChunk(const void *val, hsize_t data_size, const std::vector<hsize_t>& offset);

    HDF5DataSet& Flush();
    void SetExtent(const std::vector<hsize_t>& dims);
    template<class T> T ReadScalar() const {
        HDF5DataSpace mem_space({1});
        HDF5DataSpace file_space(*this);
        if (file_space.GetNumOfDimensions() != 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Dataset tries to read scalar from vector dataset");;

        T output;
        if (H5Dread(id, HDF5DataType(output).GetID(), mem_space.GetID(), H5S_ALL, H5P_DEFAULT, &output) < 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Dataset read unsuccessful");;
        return output;
    }

    template<class T> void ReadVector(std::vector<T> &v) const {
        HDF5DataSpace file_space(*this);
        if (file_space.GetNumOfDimensions() != 1)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Dataset tries to read multi-dimensional value into 1D vector");;
        v.resize(file_space.GetDimensions()[0]);

        if (H5Dread(id, HDF5DataType(v[0]).GetID(), H5S_ALL, H5S_ALL, H5P_DEFAULT, v.data()) < 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "1D vector dataset read unsuccessful");
    }

    template<class T> void ReadVector(std::vector<T> &v, const std::vector<hsize_t>& start, const std::vector<hsize_t>& size) const {
        HDF5DataSpace mem_space({v.size()});
        HDF5DataSpace file_space(*this);
        file_space.SelectHyperslab(start, size);

        if (H5Dread(id, HDF5DataType(v[0]).GetID(), mem_space.GetID(), file_space.GetID(), H5P_DEFAULT, v.data()) < 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Multidimensional vector dataset read unsuccessful");
    }

    template<class T> void ReadVector(std::vector<T> &v, const std::vector<hsize_t>& start,
            const std::vector<hsize_t>& size, const std::vector<hsize_t>& stride) const {
        HDF5DataSpace mem_space({v.size()});
        HDF5DataSpace file_space(*this);
        file_space.SelectHyperslabWithStride(start, size, stride);

        if (H5Dread(id, HDF5DataType(v[0]).GetID(), mem_space.GetID(), file_space.GetID(), H5P_DEFAULT, v.data()) < 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Multidimensional vector dataset read unsuccessful");
    }
    std::string ReadString() const;
};

inline std::unique_ptr<HDF5DataSet> SaveScalar(const HDF5Object& parent, const std::string &name, const char* val) {
    HDF5DataType data_type(val);
    HDF5DataSpace data_space({1});
    auto dataset = std::make_unique<HDF5DataSet>(parent, name, data_type, data_space);
    dataset->Write(data_type, val);
    return dataset;
}

inline std::unique_ptr<HDF5DataSet> SaveScalar(const HDF5Object& parent, const std::string &name, const std::string& val) {
    return SaveScalar(parent, name, val.c_str());
}

template <class T> std::unique_ptr<HDF5DataSet> SaveScalar(const HDF5Object& parent, const std::string &name, T val) {
    HDF5DataType data_type(val);
    HDF5DataSpace data_space({1});
    auto dataset = std::make_unique<HDF5DataSet>(parent, name, data_type, data_space);
    dataset->Write(data_type, &val);
    return dataset;
}

template <class T> std::unique_ptr<HDF5DataSet> HDF5Object::SaveScalar(const std::string &name, T val) {
    HDF5DataType data_type(val);
    HDF5DataSpace data_space({1});
    auto dataset = std::make_unique<HDF5DataSet>(*this, name, data_type, data_space);
    dataset->Write(data_type, &val);
    return dataset;
}


template <class T> std::unique_ptr<HDF5DataSet> SaveVector(const HDF5Object& parent, const std::string &name, const std::vector<T> &val,
                                                           std::vector<hsize_t> dims = {},
                                                           CompressionAlgorithm algorithm = CompressionAlgorithm::NO_COMPRESSION) {
    if (dims.empty()) dims = {val.size()};

    if (val.empty())
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot write empty vector");

    HDF5DataType data_type(val[0]);
    HDF5Dcpl dcpl;
    if (algorithm != CompressionAlgorithm::NO_COMPRESSION) {
        dcpl.SetCompression(algorithm, H5Tget_size(data_type.GetID()), 0);
        dcpl.SetChunking(dims);
    }

    HDF5DataSpace data_space(dims);
    auto dataset = std::make_unique<HDF5DataSet>(parent, name, data_type, data_space, dcpl);
    dataset->Write(data_type, val.data());
    return dataset;
}

template <class T> std::unique_ptr<HDF5DataSet> HDF5Object::SaveVector(const std::string &name, const std::vector<T> &val,
                                                           std::vector<hsize_t> dims, CompressionAlgorithm algorithm) {
    if (val.empty())
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot write empty vector");

    if (dims.empty()) dims = {val.size()};

    HDF5DataType data_type(val[0]);
    HDF5Dcpl dcpl;

    if (algorithm != CompressionAlgorithm::NO_COMPRESSION) {
        dcpl.SetCompression(algorithm, H5Tget_size(data_type.GetID()), 0);
        dcpl.SetChunking(dims);
    }

    HDF5DataSpace data_space(dims);
    auto dataset = std::make_unique<HDF5DataSet>(*this, name, data_type, data_space, dcpl);
    dataset->Write(data_type, val.data());
    return dataset;
}

inline std::string hdf5_version() {
    unsigned majnum, minnum, relnum;
    H5get_libversion(&majnum, &minnum, &relnum);
    return "hdf5-" + std::to_string(majnum) + "." + std::to_string(minnum) + "." + std::to_string(relnum);
}

void RegisterHDF5Filter();
std::string ExtractFilename(const std::string& str);

#endif //JUNGFRAUJOCH_HDF5OBJECTS_H
