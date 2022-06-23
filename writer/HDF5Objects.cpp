// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>

#include <bitshuffle/bshuf_h5filter.h>
#include "HDF5Objects.h"
#include "../common/JFJochException.h"

std::mutex hdf5_mutex;

hid_t HDF5Id::GetID() const {
    return id;
}

HDF5Id::HDF5Id(HDF5Id &&other) noexcept {
    id = other.id;
    other.id = -1;
}

HDF5Id::HDF5Id(const HDF5Id &other) {
    if (H5Iis_valid(id)) {
        if (H5Iinc_ref(id) < 0)
            throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot increment HDF5 Counter");
    }
    id = other.id;
}

HDF5DataSpace::HDF5DataSpace(const std::vector<hsize_t> &dims, const std::vector<hsize_t> &max_dims) : HDF5Id() {
    if (dims.empty())
        throw JFJochException(JFJochExceptionCategory::HDF5, "Dimension vector empty");

    if ((dims.size() == 1) && (dims[0] == 1))
        id = H5Screate(H5S_SCALAR);
    else {
        if (max_dims.empty()) {
            if (dims[0] == 0)
                throw JFJochException(JFJochExceptionCategory::HDF5, "Value dimension cannot be 0");

            id = H5Screate_simple(dims.size(), dims.data(), nullptr);
        } else {
            if (max_dims.size() != dims.size())
                throw JFJochException(JFJochExceptionCategory::HDF5, "Discrepancy in size of dims/max_dims");
            id = H5Screate_simple(dims.size(), dims.data(), max_dims.data());
        }
    }
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot create dataspace");

    ndims = dims.size();
}

HDF5DataSpace::HDF5DataSpace(const HDF5DataSet &data_set) : HDF5Id() {
    id = H5Dget_space(data_set.GetID());

    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot open dataspace");

    int tmp = H5Sget_simple_extent_ndims(id);

    if (tmp < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot check dimensions of dataspace");

    ndims = tmp;
}

std::vector<hsize_t> HDF5DataSpace::GetDimensions() const {
    std::vector<hsize_t> dims(ndims);
    int tmp = H5Sget_simple_extent_dims(id, dims.data(), nullptr);
    if ((tmp < 0) || (tmp != ndims))
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot check dimensions of dataspace");
    return dims;
}

void HDF5DataSpace::SelectHyperslab(const std::vector<hsize_t> &start, const std::vector<hsize_t> &size) {
    if ((start.size() != ndims) || (size.size() != ndims))
        throw JFJochException(JFJochExceptionCategory::HDF5, "Arrays are inconsistent with dataspace dimension number");

    if (H5Sselect_hyperslab(id, H5S_SELECT_SET, start.data(), NULL, size.data(), NULL) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot select hyperslab");
}

void HDF5DataSpace::SelectHyperslabWithStride(const std::vector<hsize_t> &start, const std::vector<hsize_t> &size,
                                              const std::vector<hsize_t> &stride) {
    if ((start.size() != ndims) || (stride.size() != ndims) || (size.size() != ndims))
        throw JFJochException(JFJochExceptionCategory::HDF5, "Arrays are inconsistent with dataspace dimension number");

    if (H5Sselect_hyperslab(id, H5S_SELECT_SET, start.data(), stride.data(), size.data(), NULL) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot select hyperslab");
}

HDF5DataSpace::~HDF5DataSpace() {
    if (id >= 0) H5Sclose(id);
}

uint8_t HDF5DataSpace::GetNumOfDimensions() const {
    return ndims;
}

HDF5DataType::HDF5DataType(double val) : HDF5Id() {
    id = H5Tcopy(H5T_IEEE_F64LE);
}

HDF5DataType::HDF5DataType(float val) : HDF5Id() {
    id = H5Tcopy(H5T_IEEE_F32LE);
}

HDF5DataType::HDF5DataType(bool val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_HBOOL);
}

HDF5DataType::HDF5DataType(uint16_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_UINT16);
}

HDF5DataType::HDF5DataType(uint32_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_UINT32);
}

HDF5DataType::HDF5DataType(uint64_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_UINT64);
}

HDF5DataType::HDF5DataType(int16_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_INT16);
}

HDF5DataType::HDF5DataType(int32_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_INT32);
}

HDF5DataType::HDF5DataType(int64_t val) : HDF5Id() {
    id = H5Tcopy(H5T_NATIVE_INT64);
}

HDF5DataType::HDF5DataType(const std::string& str) : HDF5Id() {
    id = H5Tcopy(H5T_C_S1);
    H5Tset_size(id, str.length() + 1);
}

HDF5DataType::HDF5DataType(const char * str) : HDF5Id() {
    id = H5Tcopy(H5T_C_S1);
    size_t length = strnlen(str,512);
    H5Tset_size(id, length + 1);
}

HDF5DataType::HDF5DataType(const DiffractionExperiment &experiment) : HDF5Id() {
    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        if (experiment.GetPixelDepth() == 2) id = H5Tcopy(H5T_NATIVE_INT16);
        else id = H5Tcopy(H5T_NATIVE_INT32);
    } else
        // Raw data with unsigned int
        id = H5Tcopy(H5T_NATIVE_UINT16);
}

HDF5DataType::HDF5DataType(const HDF5DataSet &data_set) :HDF5Id() {
    id = H5Dget_type(data_set.GetID());
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot open HDF5 data type");
}

HDF5DataType::~HDF5DataType() {
    if (id >= 0) H5Tclose(id);
}

size_t HDF5DataType::GetElemSize() const {
    return H5Tget_size(id);
}

HDF5Dcpl::HDF5Dcpl() : HDF5Id() {
    id = H5Pcreate(H5P_DATASET_CREATE);
}

HDF5Dcpl::~HDF5Dcpl() {
    if (id >= 0) H5Pclose(id);
}

void HDF5Dcpl::SetChunking(const std::vector<hsize_t> &dims) {
    if ((dims.empty()) || dims[0] == 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Value dimension cannot be 0");
    ndim = dims.size();
    if (H5Pset_chunk(id, dims.size(), dims.data()) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot set chunking");
}

uint8_t HDF5Dcpl::GetNumOfDimensions() const {
    return ndim;
}

void HDF5Dcpl::SetFillValue32(int32_t val) {
    H5Pset_fill_value(id, H5T_NATIVE_INT32, &val);
}

void HDF5Dcpl::SetFillValue16(int16_t val) {
    H5Pset_fill_value(id, H5T_NATIVE_INT16, &val);
}

void HDF5Dcpl::SetVirtual(const std::string &path, const std::string &dataset, const HDF5DataSpace &src_dataspace, const HDF5DataSpace &dest_dataspace) {
    std::string filename = ExtractFilename(path);
    if (H5Pset_virtual(id, dest_dataspace.GetID(), filename.c_str(), dataset.c_str(), src_dataspace.GetID()) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot set virtual mapping");
}

HDF5Fapl::HDF5Fapl() : HDF5Id() {
    id = H5Pcreate(H5P_FILE_ACCESS);
}

HDF5Fapl::~HDF5Fapl() {
    H5Pclose(id);
}

void HDF5Fapl::SetVersionToLatest() {
    H5Pset_libver_bounds(id, H5F_LIBVER_V110, H5F_LIBVER_LATEST);
}

HDF5Object & HDF5Object::Attr(const std::string &name, const std::string &val) {
    HDF5DataSpace dataspace;
    HDF5DataType datatype(val);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), val.c_str());
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Attribute write unsuccessful");
    return *this;
}

HDF5Object & HDF5Object::Attr(const std::string &name, int32_t val) {
    HDF5DataSpace dataspace;
    HDF5DataType datatype(val);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), &val);
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Atrribute write unsucessful");
    return *this;
}

HDF5Object & HDF5Object::Attr(const std::string &name, uint32_t val) {
    HDF5DataSpace dataspace;
    HDF5DataType datatype(val);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), &val);
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Atrribute write unsucessful");
    return *this;
}

HDF5Object & HDF5Object::Attr(const std::string &name, uint64_t val) {
    HDF5DataSpace dataspace;
    HDF5DataType datatype(val);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), &val);
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Atrribute write unsucessful");
    return *this;
}

HDF5Object & HDF5Object::Attr(const std::string &name, double val) {
    HDF5DataSpace dataspace;
    HDF5DataType datatype(val);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), &val);
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Atrribute write unsucessful");
    return *this;
}

HDF5Object & HDF5Object::Attr(const std::string &name, const Coord &val) {
    return Attr(name, std::vector<double>{val.x, val.y, val.z});
}

HDF5Object & HDF5Object::Attr(const std::string &name, const std::vector<double> &val) {
    HDF5DataSpace dataspace({val.size()});
    if (val.empty()) throw JFJochException(JFJochExceptionCategory::HDF5, "Empty array cannot be written");
    HDF5DataType datatype(val[0]);

    hid_t attr_id = H5Acreate2(id, name.c_str(), datatype.GetID(), dataspace.GetID(), H5P_DEFAULT, H5P_DEFAULT);
    herr_t ret = H5Awrite(attr_id, datatype.GetID(), val.data());
    H5Aclose(attr_id);

    if (ret < 0) throw JFJochException(JFJochExceptionCategory::HDF5, "Atrribute write unsucessful");
    return *this;
}

HDF5Object & HDF5Object::Units(const std::string &val) {
    if (!val.empty()) Attr("units", val);
    return *this;
}

HDF5Object & HDF5Object::NXClass(const std::string &val) {
    if (!val.empty()) Attr("NX_class", val);
    return *this;
}

HDF5Object& HDF5Object::Transformation(const std::string& units, const std::string& depends_on,
                           const std::string& equipment, const std::string& equipment_component,
                           const std::string& transformation_type, const Coord& vector) {
    if (!units.empty()) Attr("units", units);
    if (!depends_on.empty()) Attr("depends_on", depends_on);
    if (!equipment.empty()) Attr("equipment", equipment);
    if (!equipment_component.empty()) Attr("equipment_component", equipment);
    if (!transformation_type.empty()) Attr("transformation_type", transformation_type);
    Attr("vector", vector);
    return *this;
}

HDF5Object& HDF5Object::Transformation(const std::string& units, const std::string& depends_on,
                           const std::string& equipment, const std::string& equipment_component,
                           const std::string& transformation_type, const Coord& vector, const Coord &offset,
                           const std::string& offset_units) {
    Transformation(units, depends_on, equipment, equipment_component, transformation_type, vector);
    Attr("offset", offset);
    if (!offset_units.empty()) Attr("offset_units", offset_units);
    return *this;
}

HDF5Object& HDF5Object::ExternalLink(std::string filename, const std::string &external_dataset, const std::string &local_name) {
    // Linked file is assumed to be in the same directory as current file, so directory information is stripped
    int pos = filename.rfind('/');
    if (pos != std::string::npos) filename = filename.substr(pos+1);

    if (H5Lcreate_external(filename.c_str(),external_dataset.c_str(), GetID(),
                           local_name.c_str(), H5P_DEFAULT, H5P_DEFAULT) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot create external link in HDF5 file");
    return *this;
}

HDF5Object & HDF5Object::HardLink(const std::string &source, const std::string &dest) {
    if (H5Lcreate_hard(GetID(), source.c_str(), GetID(), dest.c_str(), H5P_DEFAULT, H5P_DEFAULT) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot create link in HDF5 file");
    return *this;
}

std::unique_ptr<HDF5DataSet> HDF5Object::SaveScalar(const std::string &name, const std::string &val) {
    return SaveScalar(name, val.c_str());
}

std::unique_ptr<HDF5DataSet> HDF5Object::SaveScalar(const std::string &name, const char *val) {
    HDF5DataType data_type(val);
    HDF5DataSpace data_space({1});
    auto dataset = std::make_unique<HDF5DataSet>(*this, name, data_type, data_space);
    dataset->Write(data_type, val);
    return dataset;
}

std::unique_ptr<HDF5DataSet> HDF5Object::SaveVector(const std::string &name, const std::vector<std::string> &val) {
    if (val.empty())
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot write empty vector");

    int pos = 0;
    size_t len = val[0].length();
    for (int i = 1; i < val.size(); i++) { if (val[i].length() > len) {pos = i; len = val[i].length();}}
    HDF5DataType data_type(val[pos]);
    HDF5Dcpl dcpl;
    HDF5DataSpace data_space({val.size()});
    auto dataset = std::make_unique<HDF5DataSet>(*this, name, data_type, data_space, dcpl);

    char buffer[(len+1) * val.size()];
    for (int i = 0; i < val.size(); i++) strncpy(buffer + i * (len+1), val[i].c_str(), len+1);
    dataset->Write(data_type, buffer);
    return dataset;
}

HDF5Group::HDF5Group(const HDF5Object& parent, const std::string &name) : HDF5Group(parent, name.c_str()) {
}

HDF5Group::HDF5Group(const HDF5Object& parent, const char *name) : HDF5Object() {
    id = H5Gcreate(parent.GetID(), name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
}

HDF5Group::~HDF5Group() {
    H5Gclose(id);
}

HDF5File::HDF5File(const std::string& filename, bool create, bool hdf5_latest, bool swmr) : HDF5Object() {
    HDF5Fapl fapl;
    if (create) {
        if (hdf5_latest) fapl.SetVersionToLatest();
        id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC | (swmr ? H5F_ACC_SWMR_WRITE : 0), H5P_DEFAULT, fapl.GetID());
    } else
        id = H5Fopen(filename.c_str(), H5F_ACC_RDWR | (swmr ? H5F_ACC_SWMR_WRITE : 0), H5P_DEFAULT);
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot open/create data HDF5 file");
}

HDF5File::~HDF5File() {
    if (id >= 0) H5Fclose(id);
}

void HDF5File::Delete(const std::string& path) {
    H5Ldelete(id, path.c_str(), H5P_DEFAULT);
}

void HDF5File::StartSWMR() {
    if (H5Fstart_swmr_write(id) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot start SWMR mode");
}

HDF5ReadOnlyFile::HDF5ReadOnlyFile(const std::string &filename) {
    id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot open/create data HDF5 file");
}

HDF5ReadOnlyFile::~HDF5ReadOnlyFile() {
    if (id >= 0) H5Fclose(id);
}

HDF5DataSet::HDF5DataSet(const HDF5Object &parent, const std::string &name, const HDF5DataType &data_type,
        const HDF5DataSpace &data_space, const HDF5Dcpl &dcpl) {

    ndim = data_space.GetNumOfDimensions();
    if ((dcpl.GetNumOfDimensions() != 0) && (dcpl.GetNumOfDimensions() != data_space.GetNumOfDimensions()))
        throw JFJochException(JFJochExceptionCategory::HDF5, "Inconsistent dimension settings");

    id = H5Dcreate2(parent.GetID(), name.c_str(), data_type.GetID(), data_space.GetID(), H5P_DEFAULT, dcpl.GetID(), H5P_DEFAULT);
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot create HDF5 dataset");
}

HDF5DataSet::HDF5DataSet(const HDF5Object &parent, const std::string &name, const HDF5DataType &data_type,
                         const HDF5DataSpace &data_space) {

    ndim = data_space.GetNumOfDimensions();

    id = H5Dcreate2(parent.GetID(), name.c_str(), data_type.GetID(), data_space.GetID(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot create HDF5 dataset");
}

HDF5DataSet::HDF5DataSet(const HDF5Object &parent, const std::string &name) : ndim(0) {
    id = H5Dopen2(parent.GetID(), name.c_str(), H5P_DEFAULT);

    ndim = HDF5DataSpace(*this).GetNumOfDimensions();

    if (id < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot open HDF5 dataset");
}

void HDF5DataSet::SetExtent(const std::vector<hsize_t> &dims) {
    if (dims.size() != ndim)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Mismatch in dataset dimensions");
    herr_t ret = H5Dset_extent(id, dims.data());
    if (ret < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Error in H5Dset_extent");
}

HDF5DataSet& HDF5DataSet::Write(const HDF5DataType &data_type, const void *val) {
    if (H5Dwrite(id, data_type.GetID(), H5S_ALL, H5S_ALL, H5P_DEFAULT, val) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot write to HDF5 file");
    return *this;
}

HDF5DataSet& HDF5DataSet::Flush() {
    if (H5Dflush(id) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Error flushing HDF5 dataset");
    return *this;
}

HDF5DataSet& HDF5DataSet::WriteDirectChunk(const void *data, hsize_t data_size, const std::vector<hsize_t> &offset) {
    if (offset.size() != ndim)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Inconsistent dimension settings");
    if (H5Dwrite_chunk(id, H5P_DEFAULT, 0, offset.data(), data_size, data) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Error writing dataset to HDF5 file");
    return *this;
}

std::string HDF5DataSet::ReadString() const {
    HDF5DataSpace file_space(*this);
    if (file_space.GetNumOfDimensions() != 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Dataset tries to read string (scalar) from vector dataset");
    HDF5DataType file_data_type(*this);

    if (file_data_type.GetElemSize() <= 0)
        return "";

    char tmp[file_data_type.GetElemSize()+1];
    tmp[file_data_type.GetElemSize()] = 0;

    if (H5Dread(id, file_data_type.GetID(), H5S_ALL, H5S_ALL, H5P_DEFAULT, tmp) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Dataset read unsuccessful");

    return std::string(tmp);
}

HDF5DataSet::~HDF5DataSet() {
    if (id >= 0) H5Dclose(id);
}


void RegisterHDF5Filter() {
    if (H5Zregister(bshuf_H5Filter) < 0)
        throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot register Bitshuffle filter");
}

void HDF5Dcpl::SetCompression(CompressionAlgorithm c, size_t elem_size, size_t block_size) {
    unsigned int params[2];
    switch (c) {
        case CompressionAlgorithm::BSHUF_LZ4:
            params[0] = block_size;
            params[1] = BSHUF_H5_COMPRESS_LZ4;
            if (H5Pset_filter(id, (H5Z_filter_t)BSHUF_H5FILTER, H5Z_FLAG_MANDATORY, elem_size, params) < 0)
                throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot set bshuf/lz4 filter");
            break;
        case CompressionAlgorithm::BSHUF_ZSTD:
#ifdef USE_ZSTD
            params[0] = block_size;
            params[1] = BSHUF_H5_COMPRESS_ZSTD;
            if (H5Pset_filter(id, (H5Z_filter_t)BSHUF_H5FILTER, H5Z_FLAG_MANDATORY, elem_size, params) < 0)
                throw JFJochException(JFJochExceptionCategory::HDF5, "Cannot set bshuf/zstd filter");
            break;
#else
            throw SLSException(SLSExceptionCategory::Compression, "ZSTD support not available");
#endif
        case CompressionAlgorithm::None:
            break;
    }

}

std::string ExtractFilename(const std::string& str) {
    if (str.find('/') == std::string::npos)
        return str;
    else
        return str.substr(str.find_last_of("/") + 1);
}