// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include "HDF5NXmx.h"

#include "../common/GitInfo.h"
#include "../include/spdlog/fmt/fmt.h"

#define WVL_1A_IN_KEV           12.39854

void HDF5Metadata::NXmx(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    hdf5_file->Attr("HDF5_Version", hdf5_version());
    HDF5Group(*hdf5_file, "/entry").NXClass("NXentry").SaveScalar("definition", "NXmx");

    LinkToData(hdf5_file, input);
    Facility(hdf5_file, input);
    Time(hdf5_file, input);
    Detector(hdf5_file, input);
    Metrology(hdf5_file, input);
    Beam(hdf5_file, input);
    Sample(hdf5_file, input);
    Calibration(hdf5_file, input);
}

inline std::string time_UTC(int64_t time_ms) {
    char buf1[255], buf2[255];
    time_t time = time_ms / (1000);
    strftime(buf1, sizeof(buf1), "%FT%T", gmtime(&time));
    snprintf(buf2, sizeof(buf2), ".%06ld", time_ms%1000);
    return std::string(buf1) + std::string(buf2) + "Z";
}

void HDF5Metadata::Time(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    hdf5_file->Attr("file_time", time_UTC(time(nullptr)));

    hdf5_file->SaveScalar("/entry/start_time", time_UTC(input.start_time_ms()));
    if (input.has_end_time_ms())
        hdf5_file->SaveScalar("/entry/end_time", time_UTC(input.end_time_ms()));
    int64_t end_time_ms = input.start_time_ms() + input.image_number() * input.detector_metadata().image_time_us() / 1000;
    hdf5_file->SaveScalar("/entry/end_time_estimated", time_UTC(end_time_ms));
}


void HDF5Metadata::Facility(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &facility_metadata = input.facility_metadata();

    HDF5Group(*hdf5_file, "/entry/source").NXClass("NXsource");
    SaveScalar(*hdf5_file, "/entry/source/name", facility_metadata.source_name())
            ->Attr("short_name", facility_metadata.source_name_short());

    HDF5Group(*hdf5_file, "/entry/instrument").NXClass("NXinstrument");
    SaveScalar(*hdf5_file, "/entry/instrument/name", facility_metadata.instrument_name())
            ->Attr("short_name", facility_metadata.instrument_name_short());
}

void HDF5Metadata::Detector(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &det = input.detector_metadata();
    const auto &geom = input.geometry_metadata();
    HDF5Group group(*hdf5_file, "/entry/instrument/detector");
    group.NXClass("NXdetector");
    SaveScalar(group, "beam_center_x", geom.beam_x_pxl())->Units("pixel");
    SaveScalar(group, "beam_center_y", geom.beam_y_pxl())->Units("pixel");
    SaveScalar(group, "distance", geom.distance_mm() / 1e3)->Units("m");
    SaveScalar(group, "detector_distance", geom.distance_mm() / 1e3)->Units("m");

    SaveScalar(group, "count_time", static_cast<double>(det.image_time_us()) / 1e6)->Units("s");
    SaveScalar(group, "frame_time", static_cast<double>(det.count_time_us()) / 1e6)->Units("s");

    SaveScalar(group, "sensor_thickness", static_cast<double>(det.sensor_thickness_um())/1e6)->Units("m");
    SaveScalar(group, "x_pixel_size", static_cast<double>(det.pixel_size_mm())/1e6)->Units("m");
    SaveScalar(group, "y_pixel_size", static_cast<double>(det.pixel_size_mm())/1e6)->Units("m");
    SaveScalar(group, "sensor_material", det.sensor_material());

    switch (det.type()) {
        case JFJochProtoBuf::EIGER:
            SaveScalar(group, "description", "PSI EIGER");
            SaveScalar(group, "type", "photon-counting pixel array detector");
            break;
        case JFJochProtoBuf::JUNGFRAU:
            SaveScalar(group, "description", "PSI JUNGFRAU");
            SaveScalar(group, "type", "adaptive gain switching pixel array detector");
            break;
        default:
            SaveScalar(group, "description", "unknown");
            break;
    }

    SaveScalar(group, "gain_setting", "auto");
    SaveScalar(group, "bit_depth_image", det.pixel_depth_byte() * 8);
    SaveScalar(group, "bit_depth_readout",16);
    SaveScalar(group, "saturation_value", det.overload());
    if (det.has_underload())
        SaveScalar(group, "underload_value", det.underload());
    SaveScalar(group, "flatfield_applied", false);
    SaveScalar(group, "pixel_mask_applied", false);
    SaveScalar(group, "acquisition_type", "triggered");
    SaveScalar(group, "countrate_correction_applied", false);

    HDF5Group(group, "geometry").NXClass("NXgeometry");

    // DIALS likes to have this soft link
    H5Lcreate_soft("/entry/data/data", group.GetID(), "data",
                   H5P_DEFAULT, H5P_DEFAULT);

    HDF5Group det_specific(group, "detectorSpecific");
    det_specific.NXClass("NXcollection");

    SaveScalar(det_specific, "nimages", input.image_number() / det.ntrigger());
    SaveScalar(det_specific, "ntrigger", det.ntrigger());

    SaveScalar(det_specific, "x_pixels_in_detector", static_cast<uint32_t>(det.width_pxl()));
    SaveScalar(det_specific, "y_pixels_in_detector", static_cast<uint32_t>(det.height_pxl()));
    SaveScalar(det_specific, "software_git_commit", jfjoch_git_sha1());
    SaveScalar(det_specific, "software_git_date", jfjoch_git_date());
    SaveScalar(det_specific, "storage_cell_number", det.storage_cell_number());

    if (det.has_efficiency())
        SaveScalar(det_specific, "data_collection_efficiency", det.efficiency());
    if (det.has_collection_cancelled())
        SaveScalar(det_specific, "data_collection_cancelled", det.collection_cancelled());
    if (det.has_max_receiver_delay())
        SaveScalar(det_specific, "max_receiver_delay", det.max_receiver_delay());
}

void HDF5Metadata::Beam(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &beam = input.beam_metadata();

    HDF5Group group(*hdf5_file, "/entry/instrument/beam");
    group.NXClass("NXbeam");
    SaveScalar(group, "incident_wavelength", WVL_1A_IN_KEV / beam.energy_kev())->Units("angstrom");
    if (beam.has_total_flux_phs())
        SaveScalar(group, "total_flux", beam.total_flux_phs())->Units("Hz");
    if (beam.has_beam_size_x_um() && beam.has_beam_size_y_um()) {
        std::vector<double> beam_size = {beam.beam_size_x_um(), beam.beam_size_y_um()};
        SaveVector(group, "incident_beam_size", beam_size);
    }
    if (beam.has_transmission()) {
        HDF5Group(*hdf5_file, "/entry/instrument/filter").NXClass("NXattenuator");
        SaveScalar(*hdf5_file, "/entry/instrument/filter/attenuator_transmission",
                   beam.transmission());
    }
}

void HDF5Metadata::Sample(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &sample_metadata = input.sample_metadata();

    HDF5Group group(*hdf5_file, "/entry/sample");
    group.NXClass("NXsample");
    group.SaveScalar("name", sample_metadata.name());

    if (sample_metadata.has_temperature_k())
        group.SaveScalar("temperature", sample_metadata.temperature_k())->Units("K");

    if (sample_metadata.has_space_group())
        group.SaveScalar("space_group", sample_metadata.space_group());

    if (sample_metadata.has_unit_cell()) {
        const auto &unit_cell = sample_metadata.unit_cell();
        std::vector<double> v = {unit_cell.a(), unit_cell.b(), unit_cell.c(),
                                 unit_cell.alpha(), unit_cell.beta(), unit_cell.gamma()};
        group.SaveVector("unit_cell", v);
    }

    if ((input.image_number() > 0) && sample_metadata.has_omega()) {
        group.SaveScalar("depends_on", "/entry/sample/transformations/omega");
        HDF5Group transformations(group, "transformations");
        transformations.NXClass("NXtransformations");
        std::vector<double> angle_container(input.image_number());

        for (int i = 0; i < input.image_number(); i++) angle_container[i] = sample_metadata.omega().start_angle_deg()
                + i * sample_metadata.omega().angle_incr_per_image_deg();
        SaveVector(transformations, "omega", angle_container)->
                    Transformation("deg", ".", "", "", "rotation",
                                   {sample_metadata.omega().rotation_axis().x(),
                                    sample_metadata.omega().rotation_axis().y(),
                                    sample_metadata.omega().rotation_axis().z()},
                                    {0,0,0}, "");
    } else
        group.SaveScalar("depends_on", ".");
}

void HDF5Metadata::Metrology(HDF5File *hdf5_file,  const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &det = input.detector_metadata();
    const auto &geom = input.geometry_metadata();

    HDF5Group transformations(*hdf5_file, "/entry/instrument/detector/transformations");
    transformations.NXClass("NXtransformations");

    std::vector<double> vector{geom.beam_x_pxl() * det.pixel_size_mm(), geom.beam_y_pxl() * det.pixel_size_mm(),
                               geom.distance_mm()};

    double vector_length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    std::vector<double> vector_norm{vector[0] / vector_length, vector[1]/vector_length, vector[2]/vector_length};

    SaveScalar(transformations, "translation", vector_length / 1e3)->
    Transformation("m", ".", "detector", "detector_arm",
                   "translation", vector_norm);

    std::vector<int32_t> origin = {0, 0};
    std::vector<int32_t> size = {static_cast<int32_t>(det.width_pxl()),
                                 static_cast<int32_t>(det.height_pxl())};

    DetectorModule(hdf5_file, "detector_module", origin, size,
                   {1,0,0}, {0,1,0}, "translation", det.pixel_size_mm());
}

void HDF5Metadata::DetectorModule(HDF5File *hdf5_file, const std::string &name, const std::vector<int32_t> &origin,
                                  const std::vector<int32_t> &size, const std::vector<double> &fast_axis,
                                  const std::vector<double> &slow_axis,
                                  const std::string &nx_axis, double pixel_size_mm) {
    HDF5Group module_group(*hdf5_file, "/entry/instrument/detector/" + name);

    module_group.NXClass("NXdetector_module");

    module_group.SaveVector("data_origin", origin);
    module_group.SaveVector("data_size", size);

    SaveScalar(module_group, "fast_pixel_direction", pixel_size_mm)->
            Transformation("mm", "/entry/instrument/detector/transformations/" + nx_axis,
                           "", "", "translation", fast_axis,
                           {0,0,0}, "");

    SaveScalar(module_group, "slow_pixel_direction", pixel_size_mm)->
            Transformation("mm", "/entry/instrument/detector/transformations/" + nx_axis,
                           "", "", "translation", slow_axis,
                           {0,0,0}, "");
}

inline void DirectChunkWrite(HDF5File *hdf5_file, const std::string &hdf5_path,
                             const JFJochProtoBuf::WriterMetadataInput &input,
                             int64_t pixel_depth_bytes, const std::string &str,
                             const std::string &units = "") {
    hsize_t xpixel = input.detector_metadata().width_pxl();
    hsize_t ypixel = input.detector_metadata().height_pxl();

    HDF5Dcpl dcpl;
    dcpl.SetCompression(CompressionAlgorithm::BSHUF_LZ4,pixel_depth_bytes,4096);
    dcpl.SetChunking( {ypixel, xpixel});
    HDF5DataType data_type(pixel_depth_bytes, false);
    HDF5DataSpace data_space({ypixel, xpixel},{ypixel, xpixel});
    HDF5DataSet data_set(*hdf5_file, hdf5_path, data_type, data_space, dcpl);
    data_set.WriteDirectChunk(str.data(), str.size(), {0,0});
    if (!units.empty())
        data_set.Units(units);
}

void HDF5Metadata::Calibration(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto &calib = input.calibration_metadata();

    if (calib.has_mask_bshuf_lz4()) {
        DirectChunkWrite(hdf5_file, "/entry/instrument/detector/pixel_mask", input, 2,
                         calib.mask_bshuf_lz4());
        hdf5_file->HardLink("/entry/instrument/detector/pixel_mask",
                            "/entry/instrument/detector/detectorSpecific/pixel_mask");
    }
    for (const auto &i: calib.calibration())
        DirectChunkWrite(hdf5_file,"/entry/instrument/detector/detectorSpecific/" + i.name(),
                         input,2, i.compressed_bshuf_lz4());
}

void HDF5Metadata::LinkToData(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input) {
    const auto& det = input.detector_metadata();

    hsize_t total_images = input.image_number();
    hsize_t width = det.width_pxl();
    hsize_t height = det.height_pxl();
    hsize_t stride = input.file_count();
    hsize_t file_count = std::min<hsize_t>(stride, total_images);

    if (total_images == 0)
        return;

    HDF5Group(*hdf5_file, "/entry/data").NXClass("NXdata");

    HDF5DataType data_type(det.pixel_depth_byte(), det.signed_pxl());
    HDF5DataSpace full_data_space({total_images, height, width});
    HDF5Dcpl dcpl;

    for (hsize_t file_id = 0; file_id < file_count; file_id++) {
        hsize_t images_in_file = total_images / stride;
        if (total_images % stride > file_id)
            images_in_file++;

        HDF5DataSpace src_data_space({images_in_file, height, width});
        HDF5DataSpace virtual_data_space({total_images, height, width});
        virtual_data_space.SelectHyperslabWithStride({file_id, 0, 0},{images_in_file, height, width},{stride,1,1});
        dcpl.SetVirtual(DataFileName(input.file_prefix(), file_id),
                        "/entry/data/data",src_data_space, virtual_data_space);
    }

    if (det.pixel_depth_byte() == 2)
        dcpl.SetFillValue16(INT16_MIN);
    else
        dcpl.SetFillValue32(INT32_MIN);

    HDF5DataSet dataset(*hdf5_file, "/entry/data/data", data_type, full_data_space, dcpl);
    dataset.Attr("image_nr_low", (int32_t) 1).Attr("image_nr_high", (int32_t) total_images);

    /*
    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        dataset.Units("photon");
    else
        dataset.Units("ADU");
        */
}

std::string HDF5Metadata::DataFileName(const std::string &prefix, int64_t file_number) {
    if (file_number < 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "File number cannot be negative");
    else if (file_number >= 1000)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Format doesn't allow for more than 1 thousand files");
    else
        return fmt::format("{:s}_data_{:03d}.h5", prefix, file_number);
}