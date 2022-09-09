// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HDF5NXmx.h"
#include "../common/GitInfo.h"

void HDF5Metadata::NXmx(HDF5File *hdf5_file, const DiffractionExperiment& experiment,
                        const JFJochProtoBuf::JFJochWriterMetadataInput &input) {
    auto output = input.receiver_output();

    hdf5_file->Attr("HDF5_Version", hdf5_version());

    HDF5Group(*hdf5_file, "/entry").NXClass("NXentry").SaveScalar("definition", "NXmx");

    if ((output.max_image_number_sent() != 0) && (experiment.GetImageNum() > 0))
        HDF5Metadata::LinkToData(hdf5_file, experiment, output.max_image_number_sent() + 1);

    HDF5Group(*hdf5_file, "/entry/source").NXClass("NXsource");
    SaveScalar(*hdf5_file, "/entry/source/name", experiment.GetSourceName())->Attr("short_name", experiment.GetSourceNameShort());

    HDF5Group(*hdf5_file, "/entry/instrument").NXClass("NXinstrument");
    SaveScalar(*hdf5_file, "/entry/instrument/name", experiment.GetInstrumentName())->Attr("short_name", experiment.GetInstrumentNameShort());

    Time(hdf5_file, experiment, output);
    DetectorGroup(hdf5_file, experiment);
    Detector(hdf5_file, experiment);
    Metrology(hdf5_file, experiment);
    Beam(hdf5_file, experiment);
    Sample(hdf5_file, experiment);
    Attenuator(hdf5_file, experiment);

    if (input.has_calibration()) {
        Mask(hdf5_file, experiment, input.calibration());
        Calibration(hdf5_file, input.calibration());
    }

    if (output.has_indexer_output())
        Processing(hdf5_file, experiment, output.indexer_output());

    FinalSettings(hdf5_file, output);
}

inline std::string time_UTC(int64_t time_ms) {
    char buf1[255], buf2[255];
    time_t time = time_ms / (1000);
    strftime(buf1, sizeof(buf1), "%FT%T", gmtime(&time));
    snprintf(buf2, sizeof(buf2), ".%06ld", time_ms%1000);
    return std::string(buf1) + std::string(buf2) + "Z";
}

void HDF5Metadata::Time(HDF5File *hdf5_file, const DiffractionExperiment& experiment, const JFJochProtoBuf::JFJochReceiverOutput &output) {
    hdf5_file->Attr("file_time", time_UTC(time(nullptr)));

    hdf5_file->SaveScalar("/entry/start_time", time_UTC(output.start_time_ms()));
    auto expected_time_in_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(experiment.GetFrameNum() * experiment.GetFrameTime()).count();
    hdf5_file->SaveScalar("/entry/end_time_estimated", time_UTC( output.start_time_ms() + expected_time_in_ms));

    SaveScalar(*hdf5_file, "/entry/end_time", time_UTC(output.end_time_ms()));

}

void HDF5Metadata::DataVDS(HDF5File *hdf5_file, const DiffractionExperiment &experiment, int64_t actual_image_number) {
    hsize_t total_images = experiment.GetImageNum();
    if (actual_image_number >= 0)
        total_images = actual_image_number;

    HDF5DataType data_type(experiment);
    HDF5DataSpace full_data_space(
            {(hsize_t) total_images, (hsize_t) experiment.GetYPixelsNum(), (hsize_t) experiment.GetXPixelsNum()});

    HDF5Dcpl dcpl;

    if (experiment.GetPixelDepth() == 2)
        dcpl.SetFillValue16(INT16_MIN);
    else
        dcpl.SetFillValue32(INT32_MIN);

    for (uint32_t file_id = 0; file_id < experiment.GetFilesNum(actual_image_number); file_id++) {
        hsize_t image0 = file_id * experiment.GetImagesPerFile();
        hsize_t images = experiment.GetImagesInFile(file_id, actual_image_number);

        HDF5DataSpace src_data_space(
                {images, (hsize_t) experiment.GetYPixelsNum(), (hsize_t) experiment.GetXPixelsNum()});
        HDF5DataSpace virtual_data_space(
                {total_images, (hsize_t) experiment.GetYPixelsNum(),
                 (hsize_t) experiment.GetXPixelsNum()});
        virtual_data_space.SelectHyperslab({image0, 0, 0}, {images, (hsize_t) experiment.GetYPixelsNum(),
                                                            (hsize_t) experiment.GetXPixelsNum()});
        dcpl.SetVirtual(experiment.GenerateDataFilename(file_id), "/entry/data/data",
                        src_data_space, virtual_data_space);
    }
    HDF5DataSet dataset(*hdf5_file, "/entry/data/data", data_type, full_data_space, dcpl);
    dataset.Attr("image_nr_low", (int32_t) 1).Attr("image_nr_high", (int32_t) total_images);

    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        dataset.Units("photon");
    else
        dataset.Units("ADU");
}


void HDF5Metadata::DataVDS_TimeResolved(HDF5File *hdf5_file, const DiffractionExperiment &experiment,
                                        int64_t actual_image_number) {
    hsize_t total_images = experiment.GetImageNum();
    if (actual_image_number >= 0)
        total_images = (actual_image_number / experiment.GetFilesNum()) * experiment.GetFilesNum();

    if (total_images == 0)
        return;

    HDF5DataType data_type(experiment);
    HDF5DataSpace full_data_space(
            {(hsize_t) total_images, (hsize_t) experiment.GetYPixelsNum(), (hsize_t) experiment.GetXPixelsNum()});

    HDF5Dcpl dcpl;

    if (experiment.GetPixelDepth() == 2)
        dcpl.SetFillValue16(INT16_MIN);
    else
        dcpl.SetFillValue32(INT32_MIN);

    for (uint32_t file_id = 0; file_id < experiment.GetFilesNum(); file_id++) {
        hsize_t image0 = file_id;
        hsize_t images = total_images / experiment.GetFilesNum();

        HDF5DataSpace src_data_space(
                {images, (hsize_t) experiment.GetYPixelsNum(), (hsize_t) experiment.GetXPixelsNum()});
        HDF5DataSpace virtual_data_space(
                {total_images, (hsize_t) experiment.GetYPixelsNum(),
                 (hsize_t) experiment.GetXPixelsNum()});
        virtual_data_space.SelectHyperslabWithStride({image0, 0, 0},
                                                     {images, (hsize_t) experiment.GetYPixelsNum(),
                                                                      (hsize_t) experiment.GetXPixelsNum()},
                                                     {(hsize_t) experiment.GetFilesNum(), 1, 1});
        dcpl.SetVirtual(experiment.GenerateDataFilename(file_id), "/entry/data/data",
                        src_data_space, virtual_data_space);
    }
    HDF5DataSet dataset(*hdf5_file, "/entry/data/data", data_type, full_data_space, dcpl);
    dataset.Attr("image_nr_low", (int32_t) 1).Attr("image_nr_high", (int32_t) total_images);

    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        dataset.Units("photon");
    else
        dataset.Units("ADU");
}

void HDF5Metadata::LinkToData(HDF5File *hdf5_file, const DiffractionExperiment &experiment, int64_t actual_image_number) {
    HDF5Group(*hdf5_file, "/entry/data").NXClass("NXdata");

    if (experiment.GetTimeResolvedMode())
        DataVDS_TimeResolved(hdf5_file, experiment, actual_image_number);
    else
        DataVDS(hdf5_file, experiment, actual_image_number);
}

void HDF5Metadata::Attenuator(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    if (experiment.GetTransmission() >= 0.0) {
        HDF5Group(*hdf5_file, "/entry/instrument/filter").NXClass("NXattenuator");
        SaveScalar(*hdf5_file, "/entry/instrument/filter/attenuator_transmission",
                   experiment.GetTransmission());
    }
}

void HDF5Metadata::DetectorGroup(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    HDF5Group group(*hdf5_file, "/entry/instrument/" DETECTOR_NAME);
    group.NXClass("NXdetector_group");
    group.SaveVector("group_names", std::vector<std::string>{DETECTOR_NAME, "detector"});
    group.SaveVector("group_index", std::vector<int32_t>{1,2});
    group.SaveVector("group_parent", std::vector<int32_t>{-1,1});
    group.SaveVector("group_type", std::vector<int32_t>{1,2});
}

void HDF5Metadata::Detector(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    HDF5Group group(*hdf5_file, "/entry/instrument/detector");
    group.NXClass("NXdetector");
    SaveScalar(group, "beam_center_x", experiment.GetBeamX_pxl())->Units("pixel");
    SaveScalar(group, "beam_center_y", experiment.GetBeamY_pxl())->Units("pixel");
    SaveScalar(group, "count_time", (double)experiment.GetImageCountTime().count()/1e6)->Units("s");
    SaveScalar(group, "frame_time", (double)experiment.GetImageTime().count()/1e6)->Units("s");
    SaveScalar(group, "distance", experiment.GetDetectorDistance_mm() / 1e3)->Units("m");
    SaveScalar(group, "detector_distance", experiment.GetDetectorDistance_mm() / 1e3)->Units("m");
    SaveScalar(group, "sensor_thickness", SENSOR_THICKNESS_IN_UM/1e6)->Units("m");
    SaveScalar(group, "x_pixel_size", PIXEL_SIZE_IN_UM/1e6)->Units("m");
    SaveScalar(group, "y_pixel_size", PIXEL_SIZE_IN_UM/1e6)->Units("m");
    SaveScalar(group, "sensor_material", SENSOR_MATERIAL);
    SaveScalar(group, "description", experiment.GetDetectorName());
    SaveScalar(group, "gain_setting", "auto");
    SaveScalar(group, "bit_depth_image", experiment.GetPixelDepth()*8);
    SaveScalar(group, "bit_depth_readout",16);
    SaveScalar(group, "saturation_value", experiment.GetOverflow());
    SaveScalar(group, "underload_value", experiment.GetUnderflow());
    SaveScalar(group, "flatfield_applied", false);
    SaveScalar(group, "pixel_mask_applied", false);
    SaveScalar(group, "type", "adaptive gain switching pixel array detector");
    SaveScalar(group, "acquisition_type", "triggered");
    SaveScalar(group, "countrate_correction_applied", false);
    // H5Lcreate_soft("/entry/data/data_000001", group.GetID(), "data", H5P_DEFAULT, H5P_DEFAULT);

    HDF5Group(group, "geometry").NXClass("NXgeometry");

    HDF5Group det_specific(group, "detectorSpecific");

    if (experiment.GetNumTriggers() > 0) {
        SaveScalar(det_specific, "nimages", experiment.GetImageNum() / experiment.GetNumTriggers());
        SaveScalar(det_specific, "ntrigger", experiment.GetNumTriggers());
    } else {
        SaveScalar(det_specific, "nimages", experiment.GetImageNum());
        SaveScalar(det_specific, "ntrigger", 1);
    }
    SaveScalar(det_specific, "x_pixels_in_detector", (uint32_t)experiment.GetXPixelsNum());
    SaveScalar(det_specific, "y_pixels_in_detector", (uint32_t)experiment.GetYPixelsNum());
    SaveScalar(det_specific, "software_git_commit", jfjoch_git_sha1());
    SaveScalar(det_specific, "software_git_date", jfjoch_git_date());
    SaveScalar(det_specific, "internal_packet_generator", experiment.IsUsingInternalPacketGen());
    SaveScalar(det_specific, "detector_full_speed", experiment.IsDetectorFullSpeed());
    SaveScalar(det_specific, "storage_cell_number", experiment.GetStorageCellNumber());
    SaveScalar(det_specific, "storage_cell_start", experiment.GetStorageCellStart());
    SaveScalar(det_specific, "delay_after_trigger", experiment.GetDetectorDelayAfterTrigger().count())->Units("us");
}

void HDF5Metadata::Beam(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    HDF5Group group(*hdf5_file, "/entry/instrument/beam");
    group.NXClass("NXbeam");
    SaveScalar(group, "incident_wavelength", experiment.GetWavelength_A())->Units("angstrom");
    SaveScalar(group, "total_flux", experiment.GetTotalFlux())->Units("Hz");
    std::vector<double> beam_size = experiment.GetBeamSize();
    if (beam_size[0] > 0.0) SaveVector(group, "incident_beam_size", beam_size);
}

void HDF5Metadata::Mask(HDF5File *hdf5_file, const DiffractionExperiment &experiment, const JFCalibration &calib) {
    for (int i = 0; i < calib.GetStorageCellNum(); i++) {
        std::string suffix = (i == 0) ? "" : ("_sc" + std::to_string(i));

        auto mask = calib.CalculateNexusMask(experiment, i);
        SaveVector(*hdf5_file, "/entry/instrument/detector/pixel_mask"+suffix, mask,
                   {(hsize_t) experiment.GetYPixelsNum(), (hsize_t) experiment.GetXPixelsNum()},
                   experiment.GetCompressionAlgorithm()); // use the same compression algorithm as data
    }

    hdf5_file->HardLink("/entry/instrument/detector/pixel_mask",
                        "/entry/instrument/detector/detectorSpecific/pixel_mask");
}

void HDF5Metadata::Sample(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    HDF5Group group(*hdf5_file, "/entry/sample");
    group.NXClass("NXsample");
    group.SaveScalar("name", experiment.GetSampleName());

    if (experiment.GetSampleTemperature() > 0.0)
        group.SaveScalar("temperature", experiment.GetSampleTemperature())->Units("K");

    if (experiment.GetSpaceGroupNumber() > 0)
        group.SaveScalar("space_group", experiment.GetSpaceGroupNumber());

    if (experiment.HasUnitCell()) {
        auto unit_cell = experiment.GetUnitCell();
        std::vector<double> v = {unit_cell.a(), unit_cell.b(), unit_cell.c(),
                                 unit_cell.alpha(), unit_cell.beta(), unit_cell.gamma()};
        group.SaveVector("unit_cell", v);
    }

    if (experiment.GetOmegaIncrement() != 0) {
        group.SaveScalar("depends_on", "/entry/sample/transformations/omega");
        HDF5Group transformations(group, "transformations");
        transformations.NXClass("NXtransformations");
        std::vector<double> angle_container(experiment.GetImageNum());
        for (int i = 0; i < experiment.GetImageNum(); i++) angle_container[i] = experiment.GetOmega(i);
        if (!angle_container.empty())
            SaveVector(transformations, "omega", angle_container)->
                    Transformation("deg", ".", "", "", "rotation",
                                   experiment.GetRotationAxis(), Coord(), "");
    } else {
        group.SaveScalar("depends_on", ".");
    }
}

void HDF5Metadata::Metrology(HDF5File *hdf5_file, const DiffractionExperiment &experiment) {
    uint16_t nmodules = experiment.GetModulesNum();
    Coord module_origin[nmodules];

    Coord detector_center{0.0, 0.0, 0.0};
    Coord beam_position{experiment.GetBeamX_pxl() * PIXEL_SIZE_IN_MM, experiment.GetBeamY_pxl() * PIXEL_SIZE_IN_MM, 0};

    for (int i = 0; i < nmodules; i ++) {

        module_origin[i] = beam_position - experiment.GetModulePosition(i);

        Coord module_center = module_origin[i] +
                (CONVERTED_MODULE_COLS * experiment.GetModuleFastAxis(i) + CONVERTED_MODULE_LINES * experiment.GetModuleSlowAxis(i))
                * PIXEL_SIZE_IN_MM / 2;

        // 3. Find detector center
        detector_center += module_center;
    }
    detector_center /= nmodules;

    HDF5Group transformations(*hdf5_file, "/entry/instrument/" DETECTOR_NAME "/transformations");
    transformations.NXClass("NXtransformations");


    Coord rail_vector{0,0,1};
    SaveScalar(transformations, "AXIS_RAIL", experiment.GetDetectorDistance_mm())->
    Transformation("mm", ".", "detector", "detector_arm",
                   "translation", rail_vector);

    Coord d0_vector{0,0,1};
    SaveScalar(transformations, "AXIS_D0", 0.0)->
    Transformation("mm", "AXIS_RAIL","detector", "detector_arm",
                   "translation", d0_vector, detector_center, "mm");


    for (int i = 0; i < nmodules; i++)
        SaveScalar(transformations, "AXIS_D0M" + std::to_string(i), 0.0)->
        Transformation("mm", "AXIS_D0", "detector", "detector_module",
                       "translation", d0_vector, module_origin[i] - detector_center, "mm");


    for (uint32_t i = 0; i < nmodules; i++) {
        HDF5Group module_group(*hdf5_file, "/entry/instrument/detector/ARRAY_D0M" + std::to_string(i));

        module_group.NXClass("NXdetector_module");

        std::vector<int32_t> origin = {static_cast<int32_t>(experiment.GetPixel0OfModule(i) / experiment.GetXPixelsNum()),
                                       static_cast<int32_t>(experiment.GetPixel0OfModule(i) % experiment.GetXPixelsNum())};
        std::vector<int32_t> size = {static_cast<int32_t>(CONVERTED_MODULE_LINES),
                                     static_cast<int32_t>(CONVERTED_MODULE_COLS)};
        module_group.SaveVector("data_origin", origin);
        module_group.SaveVector("data_size", size);

        SaveScalar(module_group, "fast_pixel_direction", PIXEL_SIZE_IN_MM)->
        Transformation("mm", "/entry/instrument/" DETECTOR_NAME "/transformations/AXIS_D0M" + std::to_string(i),
                       "", "", "translation", experiment.GetModuleFastAxis(i),
                       Coord{0,0,0}, "");

        SaveScalar(module_group, "slow_pixel_direction", PIXEL_SIZE_IN_MM)->
        Transformation("mm", "/entry/instrument/" DETECTOR_NAME "/transformations/AXIS_D0M" + std::to_string(i),
                       "", "", "translation", experiment.GetModuleSlowAxis(i),
                       Coord{0,0,0}, "");
    }
}

void HDF5Metadata::Calibration(HDF5File *hdf5_file, const JFCalibration &calib) {
    for (int i = 0; i < calib.GetStorageCellNum(); i++) {
        std::vector<hsize_t> detector_size = {(hsize_t) (calib.GetModulesNum() * RAW_MODULE_LINES), RAW_MODULE_COLS};

        std::string suffix = (i == 0) ? "" : ("_sc" + std::to_string(i));

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG0"+suffix,
                   calib.GetPedestal(0, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("ADU")
                .Attr("timestamp", time_UTC(calib.Pedestal(0, 0).collection_time))
                .Attr("timestamp_unix", calib.Pedestal(0, 0).collection_time)
                .Attr("frames", calib.Pedestal(0, 0).frames);

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG1"+suffix,
                   calib.GetPedestal(1, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("ADU")
                .Attr("timestamp", time_UTC(calib.Pedestal(0, 1).collection_time))
                .Attr("timestamp_unix", calib.Pedestal(0, 1).collection_time)
                .Attr("frames", calib.Pedestal(0, 1).frames);

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG2"+suffix,
                   calib.GetPedestal(2, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("ADU")
                .Attr("timestamp", time_UTC(calib.Pedestal(0, 2).collection_time))
                .Attr("timestamp_unix", calib.Pedestal(0, 2).collection_time)
                .Attr("frames", calib.Pedestal(0, 2).frames);

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG0_RMS"+suffix,
                   calib.GetPedestalRMS(0, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("0.5 ADU");

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG1_RMS"+suffix,
                   calib.GetPedestalRMS(1, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("0.5 ADU");

        SaveVector(*hdf5_file, "/entry/instrument/detector/detectorSpecific/pedestalG2_RMS"+suffix,
                   calib.GetPedestalRMS(2, i),
                   detector_size, CompressionAlgorithm::BSHUF_LZ4)
                ->Units("0.5 ADU");
    }
}

void HDF5Metadata::Processing(HDF5File *hdf5_file, const DiffractionExperiment& experiment, const JFJochProtoBuf::JFJochIndexerOutput &output) {
    if (experiment.GetImageNum() > 0) {
        HDF5Group group(*hdf5_file, "/entry/processing");
        SaveScalar(group, "indexing_time_per_image_ms", output.ms_per_image());
        SaveScalar(group, "images_indexed", output.indexed_images());
        SaveScalar(group, "images_analyzed", output.image_output_size());

        std::vector<int16_t> indexed(experiment.GetImageNum(), -1);
        std::vector<int16_t> spot_count(experiment.GetImageNum(), -1);

        for (const auto &i: output.image_output()) {
            indexed.at(i.image_number()) = i.indexed() ? 1 : 0;
            if (i.spot_count() > INT16_MAX)
                indexed.at(i.image_number()) = INT16_MAX;
            if (i.spot_count() < 0) // unlikely...
                indexed.at(i.image_number()) = -1;
            else
                indexed.at(i.image_number()) = static_cast<int16_t>(i.spot_count());
        }

        SaveVector(group, "spot_count", spot_count);
        SaveVector(group, "indexed", indexed);
    }
}

void HDF5Metadata::FinalSettings(HDF5File *hdf5_file, const JFJochProtoBuf::JFJochReceiverOutput &output) {
    SaveScalar(*hdf5_file, "/entry/instrument/detector/detectorSpecific/max_receive_delay", output.max_receive_delay());
    SaveScalar(*hdf5_file, "/entry/instrument/detector/detectorSpecific/efficiency", output.efficiency());

    for (int i = 0; i < output.device_statistics_size(); i++) {
        HDF5Group group(*hdf5_file, "/entry/instrument/detector/detectorSpecific/fpga"+ std::to_string(i));

        std::vector<int64_t> packets_received_per_image = {output.device_statistics(i).packets_received_per_image().begin() ,
                                                           output.device_statistics(i).packets_received_per_image().end() };

        if (!packets_received_per_image.empty())
            group.SaveVector("packets_per_image", packets_received_per_image);

        std::vector<int64_t> trigger_sequence = {output.device_statistics(i).trigger_sequence_frame_numbers().begin(),
                                                 output.device_statistics(i).trigger_sequence_frame_numbers().end()};

        if (!trigger_sequence.empty())
            group.SaveVector("trigger_sequence", trigger_sequence);

        std::vector<uint64_t> packet_mask_half_module = {output.device_statistics(i).packet_mask_half_module().begin(),
                                                         output.device_statistics(i).packet_mask_half_module().end()};
        if (!packet_mask_half_module.empty())
            group.SaveVector("packet_mask_half_module", packet_mask_half_module);

        std::vector<uint32_t> timestamp = {output.device_statistics(i).timestamp().begin(),
                                           output.device_statistics(i).timestamp().end()};

        if (!timestamp.empty())
            group.SaveVector("timestamp", timestamp);

        if (output.device_statistics(i).has_fpga_status()) {
            group.SaveScalar("stalls_hbm", output.device_statistics(i).fpga_status().stalls_hbm());
            group.SaveScalar("stalls_host", output.device_statistics(i).fpga_status().stalls_host());
            group.SaveScalar("fpga_status_register", output.device_statistics(i).fpga_status().full_status_register());
            group.SaveScalar("max_hbm_temperature", output.device_statistics(i).fpga_status().max_hbm_temp());
            group.SaveScalar("fpga_git_sha1", output.device_statistics(i).fpga_status().git_sha1());
        }
    }
}


std::unique_ptr<HDF5DataSet> HDF5Data::Data(HDF5File *hdf5_file, const DiffractionExperiment &experiment, size_t image0, size_t nimages,
                    size_t ypixel) {

    HDF5Dcpl dcpl;
    dcpl.SetCompression(experiment.GetCompressionAlgorithm(), experiment.GetPixelDepth(), experiment.GetCompressionBlockSize());
    dcpl.SetChunking( {1, ypixel, (hsize_t) experiment.GetXPixelsNum()});
    HDF5DataType data_type(experiment);
    HDF5DataSpace data_space({1, ypixel, (hsize_t) experiment.GetXPixelsNum()},
                             {H5S_UNLIMITED, ypixel, (hsize_t) experiment.GetXPixelsNum()});
    auto data_set = std::make_unique<HDF5DataSet>(*hdf5_file, "/entry/data/data", data_type, data_space, dcpl);

    data_set->Attr("image_nr_low", (int32_t) (image0 + 1)).Attr("image_nr_high", (int32_t) (image0 + nimages));

    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        data_set->Units("photon");
    else
        data_set->Units("ADU");

    return data_set;
}