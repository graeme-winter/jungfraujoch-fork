#  Copyright (2019-2022) Paul Scherrer Institute
#  SPDX-License-Identifier: GPL-3.0-or-later

import requests


class DetectorClient:
    def __init__(self, hostname: str, port: int):
        self.__addr = "http://{}:{}/".format(hostname, port)

    def __post_request(self, prefix: str, content: dict) -> None:
        api_url = self.__addr + "/" + prefix
        response = requests.post(api_url, json=content)
        response.raise_for_status()

    def __put_request(self, prefix: str, content: dict) -> None:
        api_url = self.__addr + "/" + prefix
        response = requests.put(api_url, json=content)
        response.raise_for_status()

    def __get_request(self, prefix: str) -> dict:
        api_url = self.__addr + "/" + prefix
        response = requests.get(api_url)
        response.raise_for_status()
        return response.json()

    def initialize(self) -> None:
        """
        Need to be performed before starting measurement.
        It is also used to recover detector from error condition.

        Initialize will execute calibration procedure.
        """
        self.__post_request("/detector/initialize", dict())

    def deactivate(self) -> None:
        """
        Prepare the detector to be turned off (turn off high voltage and ASIC).
        """
        self.__post_request("/detector/deactivate", dict())

    def pedestal(self) -> None:
        """
        Calibration procedure
        """
        self.__post_request("/detector/pedestal", dict())

    def start(self, dataset_settings: dict) -> None:
        """
        Start measurement

        :param dataset_settings: Dictionary containing dataset specific parameters:
        ["images_per_trigger"] - number of images collected per trigger;
        ["ntrigger"] - number of triggers detector should expect;
        ["summation"] or ["image_time_us"] - relation between internal frame time and image time
        (use only one of the two options; if image time is used, it has to be multiple of frame time);
        ["beam_x_pxl"] - beam center location on X-axis in pixels;
        ["beam_y_pxl"] - beam center location on Y-axis in pixels;
        ["detector_distance_mm"] - detector distance from the sample in mm;
        ["photon_energy_keV"] - photon energy;
        ["file_prefix"] - prefix for HDF5 files (empty == no files are written, e.g. for preview);
        ["data_file_count"] - split data frames into N-files on a round-robin basis;
        ["compression"] - algorithm for compression (NO_COMPRESSION, BSHUF_LZ4, BSHUF_ZSTD, and BSHUF_ZSTD_RLE);
        ["sample_name"] - name of sample, can work as merge ID;
        ["unit_cell"] - unit cell (dictionary with a, b, c, alpha, beta, gamma);
        ["scattering vector"] - optional (default is [0, 0, 1], array of 3 floats
        """
        self.__post_request("/detector/start", dataset_settings)

    def trigger(self) -> None:
        """
        Send soft trigger to the detector
        """
        self.__post_request("/detector/trigger", dict())

    def stop(self) -> None:
        """
        Block till measurement is finished (it is not necessary to call stop to start next measurement).
        """
        self.__post_request("/detector/stop", dict())

    def cancel(self) -> None:
        """
        Cancel running data collection.
        """
        self.__post_request("/detector/cancel", dict())

    @property
    def detector_settings(self) -> dict:
        """
        Get settings of the detector that require to redo the calibration

        :return Dictionary with detector settings (see setter for more information)
        """
        return self.__get_request("/detector/settings")

    @detector_settings.setter
    def detector_settings(self, settings: dict):
        """
        Put settings of the detector that require to redo the calibration.
        Can only be done when detector is idle and change will trigger pedestal calibration.

        :param settings: Dictionary with the following entries:
        ["frame_time_us"] - internal frame time (recommended is using only 1000 and 500 us values);
        ["count_time_us"] - count time (must be less by at least 20 us than frame time, zero means the longest possible
        for the given frame time);
        ["use_storage_cells"] - when True enables usage of all 16 storage cells;
        ["use_internal_packet_generator"] - take frames from the internal packet generator of the FPGA
        (to test FPGA if detector is not present);
        ["collect_raw_data"] - when True JUNGFAU images won't be converted to photon counts;
        ["pedestal_g0_frames"] - number of frames used to calculate G0 pedestal
        (optional, if not provided current value won't be changed);
        ["pedestal_g1_frames"] - number of frames used to calculate G1 pedestal
        (optional, if not provided current value won't be changed);
        ["pedestal_g2_frames"] - number of frames used to calculate G2 pedestal
        (optional, if not provided current value won't be changed);
        """
        self.__put_request("/detector/settings", settings)

    @property
    def data_processing_settings(self) -> dict:
        """
        Get settings of the data processing

        :return Dictionary with data processing settings (see setter for more information)
        """

        return self.__get_request("/data_processing/settings")

    @data_processing_settings.setter
    def data_processing_settings(self, settings: dict):
        """
        Configure real-time data processing settings, safe to do anytime, including during measurement

        :param settings: Dictionary with the following entries:

        """

        self.__put_request("/data_processing/settings", settings)

    @property
    def last_measurement_stats(self):
        """
        Get statistics for the last measurement

        :return Dictionary with the following entries:
        ["file_prefix"] - file prefix for data collection;
        ["images_collected"] - number of images successfully collected;
        ["max_image_number_sent"] - the highest image number
        (can differ from image number, if images were lost during collection);
        ["collection_efficiency"] - fraction of expected packets collected (1.0 is all good);
        ["compression_ratio"] - compression ratio of the dataset;
        ["cancelled"] - if cancellation happened during data collection;
        ["max_receive_delay"] - number of frames receiver was late vs. detector - large number means there were problems;
        ["writer_performance_MBs"] - total writing speed (only present if files were written);
        ["images_written"] - number of images written (should be the same as images_collected,
        only present if files were written);
        ["indexing_rate"] - indexing rate from real-time indexing;
        ["indexing_performance_ms"] - average time to index a single image;
        """
        return self.__get_request("/detector/last_measurement")

    @property
    def status(self):
        """
        Get detector status:

        :return Dictionary with the following entries:
        ["broker_state"] - current state: NOT_INITIALIZED, IDLE, BUSY, PEDESTAL, DATA_COLLECTION, ERROR;
        ["progress'] - data collection progress in percent (only if measurement running);
        ["indexing_rate"] - instantaneous indexing rate from real-time indexing (only if measurement running);
        """
        return self.__get_request("/detector/status")
