# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: jfjoch.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0cjfjoch.proto\x12\x0eJFJochProtoBuf\"\x07\n\x05\x45mpty\"W\n\x08UnitCell\x12\t\n\x01\x61\x18\x01 \x01(\x01\x12\t\n\x01\x62\x18\x02 \x01(\x01\x12\t\n\x01\x63\x18\x03 \x01(\x01\x12\r\n\x05\x61lpha\x18\x04 \x01(\x01\x12\x0c\n\x04\x62\x65ta\x18\x05 \x01(\x01\x12\r\n\x05gamma\x18\x06 \x01(\x01\")\n\x06Vector\x12\t\n\x01x\x18\x01 \x01(\x01\x12\t\n\x01y\x18\x02 \x01(\x01\x12\t\n\x01z\x18\x03 \x01(\x01\"B\n\x12\x46rameCountSettings\x12\x1a\n\x12images_per_trigger\x18\x01 \x01(\x03\x12\x10\n\x08ntrigger\x18\x02 \x01(\x03\"\x91\x01\n\x0eTimingSettings\x12\"\n\x1a\x66rame_time_pedestalG1G2_us\x18\x01 \x01(\x03\x12\x15\n\rframe_time_us\x18\x03 \x01(\x03\x12\x15\n\rcount_time_us\x18\x04 \x01(\x03\x12\x11\n\tsummation\x18\x05 \x01(\x03\x12\x1a\n\x12time_resolved_mode\x18\r \x01(\x08\"\xf1\x02\n\x1b\x44iffractionGeometrySettings\x12\x12\n\nbeam_x_pxl\x18\x01 \x01(\x01\x12\x12\n\nbeam_y_pxl\x18\x02 \x01(\x01\x12\x1c\n\x14\x64\x65tector_distance_mm\x18\x03 \x01(\x01\x12\x1b\n\x13\x64\x65tector_size_x_pxl\x18\x06 \x01(\x03\x12\x1b\n\x13\x64\x65tector_size_y_pxl\x18\x07 \x01(\x03\x12\x11\n\tgap_x_pxl\x18\x08 \x01(\x03\x12\x11\n\tgap_y_pxl\x18\t \x01(\x03\x12\x1b\n\x13\x64\x61ta_stream_modules\x18\n \x03(\x03\x12\x1d\n\x15\x66irst_pixel_of_module\x18\x0b \x03(\x03\x12\"\n\x1ahorizontal_module_stacking\x18\x0c \x01(\x03\x12\x31\n\x11scattering_vector\x18\x0e \x01(\x0b\x32\x16.JFJochProtoBuf.Vector\x12\x19\n\x11photon_energy_keV\x18\x10 \x01(\x01\"|\n\x10RotationSettings\x12\x17\n\x0fstart_angle_deg\x18\x01 \x01(\x01\x12 \n\x18\x61ngle_incr_per_image_deg\x18\x02 \x01(\x01\x12-\n\rrotation_axis\x18\x03 \x01(\x0b\x32\x16.JFJochProtoBuf.Vector\"1\n\x18\x42\x65\x61mlineMetadataSettings\x12\x15\n\rdetector_name\x18\t \x01(\t\"h\n\x13\x43ompressionSettings\x12.\n\talgorithm\x18\x01 \x01(\x0e\x32\x1b.JFJochProtoBuf.Compression\x12\r\n\x05level\x18\x02 \x01(\x03\x12\x12\n\nblock_size\x18\x03 \x01(\x03\"C\n\x13ImageSavingSettings\x12\x13\n\x0b\x66ile_prefix\x18\x01 \x01(\t\x12\x17\n\x0fimages_per_file\x18\x02 \x01(\x03\"\xbd\x01\n\x10\x44\x65tectorSettings\x12\x18\n\x10\x66orce_full_speed\x18\x01 \x01(\x08\x12&\n\x1einternal_fpga_packet_generator\x18\x02 \x01(\x08\x12\x15\n\rstorage_cells\x18\x03 \x01(\x03\x12\x1a\n\x12storage_cell_start\x18\x04 \x01(\x03\x12\x1e\n\x16\x64\x65lay_after_trigger_us\x18\x05 \x01(\x03\x12\x14\n\x0csoft_trigger\x18\x06 \x01(\x08\"n\n\x0fPreviewSettings\x12\x19\n\x11preview_period_us\x18\x01 \x01(\x03\x12\x1e\n\x16spot_finding_period_us\x18\x02 \x01(\x03\x12 \n\x18\x62kg_estimation_period_us\x18\x03 \x01(\x03\"K\n\x10PedestalSettings\x12\x11\n\tg0_frames\x18\x01 \x01(\x03\x12\x11\n\tg1_frames\x18\x02 \x01(\x03\x12\x11\n\tg2_frames\x18\x03 \x01(\x03\"\xb5\x01\n\x12\x43onversionSettings\x12*\n\x04type\x18\x05 \x01(\x0e\x32\x1c.JFJochProtoBuf.DetectorType\x12*\n\x04mode\x18\x01 \x01(\x0e\x32\x1c.JFJochProtoBuf.DetectorMode\x12\x13\n\x0bupside_down\x18\x02 \x01(\x08\x12\x19\n\x11mask_module_edges\x18\x03 \x01(\x08\x12\x17\n\x0fmask_chip_edges\x18\x04 \x01(\x08\"t\n\x1cJungfraujochInternalSettings\x12\x19\n\x11\x62\x61se_ipv4_address\x18\x06 \x01(\x03\x12\x15\n\rbase_udp_port\x18\x07 \x01(\x03\x12\x10\n\x08git_sha1\x18\x08 \x01(\t\x12\x10\n\x08git_date\x18\t \x01(\t\"\x80\x01\n\x0eSampleSettings\x12\x30\n\tunit_cell\x18\x02 \x01(\x0b\x32\x18.JFJochProtoBuf.UnitCellH\x00\x88\x01\x01\x12\x1a\n\x12space_group_number\x18\x03 \x01(\x03\x12\x12\n\nrun_number\x18\x04 \x01(\x03\x42\x0c\n\n_unit_cell\"\x86\x01\n\x19RadialIntegrationSettings\x12\r\n\x05low_q\x18\x01 \x01(\x01\x12\x0e\n\x06high_q\x18\x02 \x01(\x01\x12\x11\n\tq_spacing\x18\x03 \x01(\x01\x12\x1a\n\x12\x62kg_estimate_low_q\x18\x04 \x01(\x01\x12\x1b\n\x13\x62kg_estimate_high_q\x18\x05 \x01(\x01\"\xf9\x05\n\x14JungfraujochSettings\x12\x37\n\x0b\x66rame_count\x18\x01 \x01(\x0b\x32\".JFJochProtoBuf.FrameCountSettings\x12.\n\x06timing\x18\x02 \x01(\x0b\x32\x1e.JFJochProtoBuf.TimingSettings\x12I\n\x14\x64iffraction_geometry\x18\x03 \x01(\x0b\x32+.JFJochProtoBuf.DiffractionGeometrySettings\x12\x43\n\x11\x62\x65\x61mline_metadata\x18\x04 \x01(\x0b\x32(.JFJochProtoBuf.BeamlineMetadataSettings\x12\x39\n\x0cimage_saving\x18\x05 \x01(\x0b\x32#.JFJochProtoBuf.ImageSavingSettings\x12\x32\n\x08\x64\x65tector\x18\x06 \x01(\x0b\x32 .JFJochProtoBuf.DetectorSettings\x12\x30\n\x07preview\x18\x07 \x01(\x0b\x32\x1f.JFJochProtoBuf.PreviewSettings\x12\x32\n\x08pedestal\x18\x08 \x01(\x0b\x32 .JFJochProtoBuf.PedestalSettings\x12\x30\n\x04\x63onv\x18\t \x01(\x0b\x32\".JFJochProtoBuf.ConversionSettings\x12\x32\n\x05\x63ompr\x18\n \x01(\x0b\x32#.JFJochProtoBuf.CompressionSettings\x12>\n\x08internal\x18\x0b \x01(\x0b\x32,.JFJochProtoBuf.JungfraujochInternalSettings\x12.\n\x06sample\x18\x0c \x01(\x0b\x32\x1e.JFJochProtoBuf.SampleSettings\x12=\n\nradial_int\x18\r \x01(\x0b\x32).JFJochProtoBuf.RadialIntegrationSettings\"\\\n\nJFPedestal\x12\x10\n\x08pedestal\x18\x01 \x01(\x0c\x12\x0b\n\x03rms\x18\x02 \x01(\x0c\x12\x0c\n\x04mask\x18\x03 \x01(\x0c\x12\x11\n\ttime_unix\x18\x04 \x01(\x03\x12\x0e\n\x06\x66rames\x18\x05 \x01(\x04\"u\n\rJFCalibration\x12\x10\n\x08nmodules\x18\x01 \x01(\x04\x12\x16\n\x0enstorage_cells\x18\x02 \x01(\x04\x12,\n\x08pedestal\x18\x03 \x03(\x0b\x32\x1a.JFJochProtoBuf.JFPedestal\x12\x0c\n\x04mask\x18\x04 \x01(\x0c\"V\n\x17JFCalibrationStatistics\x12;\n\x11module_statistics\x18\x01 \x03(\x0b\x32 .JFJochProtoBuf.ModuleStatistics\"\xa8\x03\n\x1b\x41\x63quisitionDeviceStatistics\x12\x14\n\x0cgood_packets\x18\x01 \x01(\x04\x12#\n\x1bpackets_expected_per_module\x18\x02 \x01(\x04\x12#\n\x1bpackets_received_per_module\x18\x03 \x03(\x04\x12\x12\n\nefficiency\x18\x04 \x01(\x01\x12\x16\n\x0e\x62ytes_received\x18\x05 \x01(\x04\x12\x17\n\x0fstart_timestamp\x18\x06 \x01(\x04\x12\x15\n\rend_timestamp\x18\x07 \x01(\x04\x12/\n\x0b\x66pga_status\x18\x08 \x01(\x0b\x32\x1a.JFJochProtoBuf.FPGAStatus\x12\x10\n\x08nmodules\x18\t \x01(\x04\x12\x13\n\x0bpacket_mask\x18\n \x03(\x04\x12\x1f\n\x17mask_entries_per_module\x18\x0b \x01(\x04\x12\x18\n\x10packets_expected\x18\x0c \x01(\x04\x12\x11\n\ttimestamp\x18\r \x03(\r\x12\x16\n\x0e\x64\x65tector_debug\x18\x0e \x03(\r\x12\x0f\n\x07\x62unchid\x18\x0f \x03(\x04\"\xa4\x01\n\rReceiverInput\x12\x43\n\x15jungfraujoch_settings\x18\x01 \x01(\x0b\x32$.JFJochProtoBuf.JungfraujochSettings\x12\x32\n\x0b\x63\x61libration\x18\x02 \x01(\x0b\x32\x1d.JFJochProtoBuf.JFCalibration\x12\x1a\n\x12writer_zmq_address\x18\x03 \x03(\t\"\x89\x04\n\x0eReceiverOutput\x12\x46\n\x11\x64\x65vice_statistics\x18\x01 \x03(\x0b\x32+.JFJochProtoBuf.AcquisitionDeviceStatistics\x12\x19\n\x11max_receive_delay\x18\x02 \x01(\x04\x12\x17\n\x0f\x63ompressed_size\x18\x03 \x01(\x04\x12\x18\n\x10\x63ompressed_ratio\x18\x04 \x01(\x01\x12\x13\n\x0bimages_sent\x18\x05 \x01(\x04\x12\x15\n\rstart_time_ms\x18\x06 \x01(\x04\x12\x13\n\x0b\x65nd_time_ms\x18\x07 \x01(\x04\x12\x12\n\nefficiency\x18\t \x01(\x01\x12\x1d\n\x15max_image_number_sent\x18\n \x01(\x04\x12\x43\n\x15jungfraujoch_settings\x18\x0f \x01(\x0b\x32$.JFJochProtoBuf.JungfraujochSettings\x12:\n\x0eindexer_output\x18\x10 \x01(\x0b\x32\x1d.JFJochProtoBuf.IndexerOutputH\x00\x88\x01\x01\x12\x33\n\x0fpedestal_result\x18\x11 \x03(\x0b\x32\x1a.JFJochProtoBuf.JFPedestal\x12\x18\n\x10master_file_name\x18\x12 \x01(\tB\x11\n\x0f_indexer_outputJ\x04\x08\x08\x10\tJ\x04\x08\x0e\x10\x0f\".\n\x15ReceiverNetworkConfig\x12\x15\n\rfpga_mac_addr\x18\x01 \x03(\t\"A\n\x08\x44\x61taFile\x12\x10\n\x08\x66ilename\x18\x01 \x01(\t\x12\x13\n\x0bimage_count\x18\x02 \x01(\x03\x12\x0e\n\x06image0\x18\x03 \x01(\x03\"\xc9\x02\n\x0bWriterInput\x12\x1c\n\x14zmq_receiver_address\x18\x01 \x01(\t\x12,\n\ndata_files\x18\x02 \x03(\x0b\x32\x18.JFJochProtoBuf.DataFile\x12\r\n\x05width\x18\x03 \x01(\x03\x12\x0e\n\x06height\x18\x04 \x01(\x03\x12\x17\n\x0fimages_per_file\x18\x05 \x01(\x03\x12\x19\n\x11pixel_depth_bytes\x18\x06 \x01(\x03\x12\x12\n\nsigned_pxl\x18\x07 \x01(\x08\x12:\n\x15\x63ompression_algorithm\x18\x08 \x01(\x0e\x32\x1b.JFJochProtoBuf.Compression\x12\x1e\n\x16\x63ompression_block_size\x18\t \x01(\x03\x12\x13\n\x0bimage_units\x18\n \x01(\t\x12\x16\n\x0emax_spot_count\x18\x0b \x01(\x03\"P\n\x0cWriterOutput\x12\x0f\n\x07nimages\x18\x01 \x01(\x03\x12\x17\n\x0fperformance_MBs\x18\x02 \x01(\x02\x12\x16\n\x0eperformance_Hz\x18\x03 \x01(\x02\"z\n\x10\x46\x61\x63ilityMetadata\x12\x19\n\x11source_name_short\x18\n \x01(\t\x12\x13\n\x0bsource_name\x18\x0b \x01(\t\x12\x17\n\x0finstrument_name\x18\x0c \x01(\t\x12\x1d\n\x15instrument_name_short\x18\r \x01(\t\"\xa7\x01\n\x1a\x45xperimentGeometryMetadata\x12\x12\n\nbeam_x_pxl\x18\x01 \x01(\x01\x12\x12\n\nbeam_y_pxl\x18\x02 \x01(\x01\x12\x13\n\x0b\x64istance_mm\x18\x03 \x01(\x01\x12\x36\n\x11scattering_vector\x18\x04 \x01(\x0b\x32\x16.JFJochProtoBuf.VectorH\x00\x88\x01\x01\x42\x14\n\x12_scattering_vector\"\xeb\x03\n\x10\x44\x65tectorMetadata\x12*\n\x04type\x18\x01 \x01(\x0e\x32\x1c.JFJochProtoBuf.DetectorType\x12\x11\n\twidth_pxl\x18\x02 \x01(\x03\x12\x12\n\nheight_pxl\x18\x03 \x01(\x03\x12\x18\n\x10pixel_depth_byte\x18\x04 \x01(\x03\x12\x12\n\nsigned_pxl\x18\x05 \x01(\x08\x12\x38\n\x0b\x63ompression\x18\x06 \x01(\x0b\x32#.JFJochProtoBuf.CompressionSettings\x12\x15\n\rimage_time_us\x18\x07 \x01(\x03\x12\x15\n\rcount_time_us\x18\x08 \x01(\x03\x12\x10\n\x08overload\x18\t \x01(\x03\x12\x16\n\tunderload\x18\n \x01(\x03H\x00\x88\x01\x01\x12\x1e\n\x16\x64\x65lay_after_trigger_us\x18\x0b \x01(\x01\x12\x10\n\x08ntrigger\x18\x0c \x01(\x03\x12\x1a\n\x12time_resolved_mode\x18\r \x01(\x08\x12\x1b\n\x13storage_cell_number\x18\x0e \x01(\x03\x12\x15\n\rpixel_size_mm\x18\x0f \x01(\x01\x12\x1b\n\x13sensor_thickness_um\x18\x10 \x01(\x01\x12\x17\n\x0fsensor_material\x18\x11 \x01(\tB\x0c\n\n_underload\"\xde\x01\n\x0c\x42\x65\x61mMetadata\x12\x12\n\nenergy_kev\x18\x01 \x01(\x01\x12\x19\n\x0ctransmission\x18\x02 \x01(\x01H\x00\x88\x01\x01\x12\x1b\n\x0etotal_flux_phs\x18\x03 \x01(\x01H\x01\x88\x01\x01\x12\x1b\n\x0e\x62\x65\x61m_size_x_um\x18\x04 \x01(\x01H\x02\x88\x01\x01\x12\x1b\n\x0e\x62\x65\x61m_size_y_um\x18\x05 \x01(\x01H\x03\x88\x01\x01\x42\x0f\n\r_transmissionB\x11\n\x0f_total_flux_phsB\x11\n\x0f_beam_size_x_umB\x11\n\x0f_beam_size_y_um\"\xf6\x01\n\x0eSampleMetadata\x12\x0c\n\x04name\x18\x01 \x01(\t\x12\x30\n\tunit_cell\x18\x02 \x01(\x0b\x32\x18.JFJochProtoBuf.UnitCellH\x00\x88\x01\x01\x12\x18\n\x0bspace_group\x18\x03 \x01(\x03H\x01\x88\x01\x01\x12\x1a\n\rtemperature_K\x18\x04 \x01(\x01H\x02\x88\x01\x01\x12\x34\n\x05omega\x18\x05 \x01(\x0b\x32 .JFJochProtoBuf.RotationSettingsH\x03\x88\x01\x01\x42\x0c\n\n_unit_cellB\x0e\n\x0c_space_groupB\x10\n\x0e_temperature_KB\x08\n\x06_omega\"U\n\x18\x43\x61librationMetadataEntry\x12\x1c\n\x14\x63ompressed_bshuf_lz4\x18\x01 \x01(\x0c\x12\x0c\n\x04name\x18\x02 \x01(\t\x12\r\n\x05units\x18\x03 \x01(\t\"\x84\x01\n\x13\x43\x61librationMetadata\x12\x1b\n\x0emask_bshuf_lz4\x18\x01 \x01(\x0cH\x00\x88\x01\x01\x12=\n\x0b\x63\x61libration\x18\x02 \x03(\x0b\x32(.JFJochProtoBuf.CalibrationMetadataEntryB\x11\n\x0f_mask_bshuf_lz4\"\xdd\x04\n\x13WriterMetadataInput\x12;\n\x11\x66\x61\x63ility_metadata\x18\x01 \x01(\x0b\x32 .JFJochProtoBuf.FacilityMetadata\x12;\n\x11\x64\x65tector_metadata\x18\x02 \x01(\x0b\x32 .JFJochProtoBuf.DetectorMetadata\x12\x33\n\rbeam_metadata\x18\x03 \x01(\x0b\x32\x1c.JFJochProtoBuf.BeamMetadata\x12\x37\n\x0fsample_metadata\x18\x04 \x01(\x0b\x32\x1e.JFJochProtoBuf.SampleMetadata\x12\x41\n\x14\x63\x61libration_metadata\x18\x05 \x01(\x0b\x32#.JFJochProtoBuf.CalibrationMetadata\x12\x45\n\x11geometry_metadata\x18\x06 \x01(\x0b\x32*.JFJochProtoBuf.ExperimentGeometryMetadata\x12\x14\n\x0cimage_number\x18\x07 \x01(\x03\x12\x1a\n\x12images_per_trigger\x18\x08 \x01(\x03\x12\x15\n\rstart_time_ms\x18\t \x01(\x04\x12\x18\n\x0b\x65nd_time_ms\x18\n \x01(\x04H\x00\x88\x01\x01\x12\x1a\n\x12metadata_file_name\x18\x0b \x01(\t\x12,\n\ndata_files\x18\x0c \x03(\x0b\x32\x18.JFJochProtoBuf.DataFile\x12\x17\n\x0fimages_per_file\x18\r \x01(\x03\x42\x0e\n\x0c_end_time_ms\"5\n\nWriterSpot\x12\t\n\x01x\x18\x01 \x01(\x02\x12\t\n\x01y\x18\x02 \x01(\x02\x12\x11\n\tintensity\x18\x03 \x01(\x02\"\xde\x01\n\x0bWriterFrame\x12-\n\x04type\x18\x01 \x01(\x0e\x32\x1f.JFJochProtoBuf.WriterFrameType\x12\x18\n\x0b\x66ile_number\x18\x02 \x01(\x03H\x00\x88\x01\x01\x12\x19\n\x0cimage_number\x18\x03 \x01(\x03H\x01\x88\x01\x01\x12.\n\nspot_coord\x18\x04 \x03(\x0b\x32\x1a.JFJochProtoBuf.WriterSpot\x12\x11\n\x04\x64\x61ta\x18\x05 \x01(\x0cH\x02\x88\x01\x01\x42\x0e\n\x0c_file_numberB\x0f\n\r_image_numberB\x07\n\x05_data\"\xe0\x01\n\x14\x44\x65tectorModuleConfig\x12\x17\n\x0fudp_dest_port_1\x18\x01 \x01(\x04\x12\x17\n\x0fudp_dest_port_2\x18\x02 \x01(\x04\x12\x17\n\x0fipv4_src_addr_1\x18\x03 \x01(\t\x12\x17\n\x0fipv4_src_addr_2\x18\x04 \x01(\t\x12\x18\n\x10ipv4_dest_addr_1\x18\x05 \x01(\t\x12\x18\n\x10ipv4_dest_addr_2\x18\x06 \x01(\t\x12\x17\n\x0fmac_addr_dest_1\x18\x07 \x01(\t\x12\x17\n\x0fmac_addr_dest_2\x18\x08 \x01(\t\"G\n\x0e\x44\x65tectorConfig\x12\x35\n\x07modules\x18\x01 \x03(\x0b\x32$.JFJochProtoBuf.DetectorModuleConfig\"\xa1\x02\n\rDetectorInput\x12\x13\n\x0bmodules_num\x18\x01 \x01(\x03\x12*\n\x04mode\x18\x02 \x01(\x0e\x32\x1c.JFJochProtoBuf.DetectorMode\x12\x12\n\nnum_frames\x18\x03 \x01(\x03\x12\x14\n\x0cnum_triggers\x18\x04 \x01(\x03\x12\x1b\n\x13storage_cell_number\x18\x05 \x01(\x03\x12\x1a\n\x12storage_cell_start\x18\x06 \x01(\x03\x12\x1a\n\x12storage_cell_delay\x18\x07 \x01(\x03\x12\x11\n\tperiod_us\x18\t \x01(\x03\x12\x15\n\rcount_time_us\x18\n \x01(\x03\x12\x10\n\x08\x64\x65lay_us\x18\x0b \x01(\x03\x12\x14\n\x0csoft_trigger\x18\x0c \x01(\x08\"\x10\n\x0e\x44\x65tectorOutput\"p\n\x0e\x44\x65tectorStatus\x12\x32\n\x06status\x18\x01 \x01(\x0e\x32\".JFJochProtoBuf.DetectorStatusEnum\x12\x12\n\nfw_version\x18\x02 \x01(\x03\x12\x16\n\x0eserver_version\x18\x03 \x01(\x03\"\x99\x08\n\nFPGAStatus\x12\x15\n\rpackets_ether\x18\x02 \x01(\x04\x12\x13\n\x0bpackets_udp\x18\x03 \x01(\x04\x12\x16\n\x0epackets_jfjoch\x18\x04 \x01(\x04\x12\x14\n\x0cpackets_icmp\x18\x05 \x01(\x04\x12\x11\n\tfpga_idle\x18\x06 \x01(\x08\x12\x10\n\x08hbm_temp\x18\x07 \x01(\x04\x12\x14\n\x0cmax_hbm_temp\x18\x08 \x01(\x04\x12\x12\n\nstalls_hbm\x18\t \x01(\x04\x12\x13\n\x0bstalls_host\x18\n \x01(\x04\x12\x1b\n\x13\x65thernet_rx_aligned\x18\x0b \x01(\x08\x12\x18\n\x10\x65thernet_bad_fcs\x18\x0c \x01(\x08\x12\x1c\n\x14\x66ull_status_register\x18\r \x01(\r\x12?\n\x0b\x66ifo_status\x18\x0e \x03(\x0b\x32*.JFJochProtoBuf.FPGAStatus.FifoStatusEntry\x12\x13\n\x0bmax_modules\x18\x0f \x01(\x04\x12\x10\n\x08git_sha1\x18\x10 \x01(\r\x12\x17\n\x0fmailbox_err_reg\x18\x11 \x01(\r\x12\x1a\n\x12mailbox_status_reg\x18\x12 \x01(\r\x12\x1c\n\x14\x64\x61tamover_mm2s_error\x18\x13 \x01(\x08\x12\x1c\n\x14\x64\x61tamover_s2mm_error\x18\x14 \x01(\x08\x12&\n\x1e\x66rame_statistics_alignment_err\x18\x15 \x01(\x08\x12\"\n\x1a\x66rame_statistics_tlast_err\x18\x16 \x01(\x08\x12%\n\x1d\x66rame_statistics_work_req_err\x18\x17 \x01(\x08\x12\x14\n\x0cslowest_head\x18\x18 \x01(\x04\x12\x1b\n\x13qsfp_module_present\x18\x19 \x01(\x08\x12\x16\n\x0e\x66pga_temp_degC\x18\x1a \x01(\x01\x12\x1a\n\x12\x63urrent_edge_12V_A\x18\x1b \x01(\x01\x12\x1a\n\x12voltage_edge_12V_V\x18\x1c \x01(\x01\x12\x1b\n\x13\x63urrent_edge_3p3V_A\x18\x1d \x01(\x01\x12\x1b\n\x13voltage_edge_3p3V_V\x18\x1e \x01(\x01\x12\x1c\n\x14pcie_h2c_descriptors\x18\x1f \x01(\x04\x12\x1c\n\x14pcie_c2h_descriptors\x18  \x01(\x04\x12\x16\n\x0epcie_h2c_beats\x18! \x01(\x04\x12\x16\n\x0epcie_c2h_beats\x18\" \x01(\x04\x12\x17\n\x0fpcie_h2c_status\x18# \x01(\x04\x12\x17\n\x0fpcie_c2h_status\x18$ \x01(\x04\x1aQ\n\x0f\x46ifoStatusEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12-\n\x05value\x18\x02 \x01(\x0e\x32\x1e.JFJochProtoBuf.FPGAFIFOStatus:\x02\x38\x01\"\x1c\n\x04Plot\x12\t\n\x01x\x18\x01 \x03(\x02\x12\t\n\x01y\x18\x02 \x03(\x02\"\x83\x02\n\x0eReceiverStatus\x12\x10\n\x08progress\x18\x01 \x01(\x01\x12/\n\x0b\x66pga_status\x18\x02 \x03(\x0b\x32\x1a.JFJochProtoBuf.FPGAStatus\x12\x0c\n\x04idle\x18\x03 \x01(\x08\x12*\n\x0c\x62kg_estimate\x18\x04 \x01(\x0b\x32\x14.JFJochProtoBuf.Plot\x12\x30\n\x12radial_int_profile\x18\x05 \x01(\x0b\x32\x14.JFJochProtoBuf.Plot\x12(\n\nspot_count\x18\x06 \x01(\x0b\x32\x14.JFJochProtoBuf.Plot\x12\x18\n\x10master_file_name\x18\x07 \x01(\t\"\xa1\x02\n\x16\x44\x61taProcessingSettings\x12!\n\x19signal_to_noise_threshold\x18\x01 \x01(\x01\x12\x1e\n\x16photon_count_threshold\x18\x02 \x01(\x03\x12\x18\n\x10min_pix_per_spot\x18\x03 \x01(\x03\x12\x18\n\x10max_pix_per_spot\x18\x04 \x01(\x03\x12\x16\n\x0elocal_bkg_size\x18\x05 \x01(\x03\x12\"\n\x15high_resolution_limit\x18\x06 \x01(\x01H\x00\x88\x01\x01\x12!\n\x14low_resolution_limit\x18\x07 \x01(\x01H\x01\x88\x01\x01\x42\x18\n\x16_high_resolution_limitB\x17\n\x15_low_resolution_limit\"\xc7\x02\n\x0cPreviewFrame\x12\x14\n\x0cimage_number\x18\x01 \x01(\x03\x12\x14\n\x0ctotal_images\x18\x02 \x01(\x03\x12\x14\n\x0cwavelength_A\x18\x03 \x01(\x01\x12\x15\n\rbeam_center_x\x18\x04 \x01(\x01\x12\x15\n\rbeam_center_y\x18\x05 \x01(\x01\x12\x1c\n\x14\x64\x65tector_distance_mm\x18\x06 \x01(\x01\x12\x18\n\x10saturation_value\x18\x07 \x01(\x03\x12\x13\n\x0b\x66ile_prefix\x18\x08 \x01(\t\x12\r\n\x05width\x18\t \x01(\x03\x12\x0e\n\x06height\x18\n \x01(\x03\x12\x13\n\x0bpixel_depth\x18\x0b \x01(\x03\x12\x38\n\x0b\x63ompression\x18\x0c \x01(\x0b\x32#.JFJochProtoBuf.CompressionSettings\x12\x0c\n\x04\x64\x61ta\x18\r \x01(\x0c\"T\n\x15SpotFinderImageOutput\x12\x14\n\x0cimage_number\x18\x01 \x01(\x03\x12%\n\x05\x63oord\x18\x03 \x03(\x0b\x32\x16.JFJochProtoBuf.Vector\"m\n\rIndexerStatus\x12+\n\rindexing_rate\x18\x01 \x01(\x0b\x32\x14.JFJochProtoBuf.Plot\x12\x17\n\x0fimages_analyzed\x18\x03 \x01(\x03\x12\x16\n\x0eimages_indexed\x18\x04 \x01(\x03\"\xc3\x01\n\x0cIndexerInput\x12\x1d\n\x15\x65xpected_image_number\x18\x01 \x01(\x03\x12\x14\n\x0cimage_stride\x18\x02 \x01(\x03\x12\x30\n\tunit_cell\x18\x03 \x01(\x0b\x32\x18.JFJochProtoBuf.UnitCellH\x00\x88\x01\x01\x12\x11\n\tcentering\x18\x04 \x01(\x03\x12\x19\n\x11zmq_recv_pub_addr\x18\x05 \x01(\t\x12\x10\n\x08\x62in_size\x18\x06 \x01(\x03\x42\x0c\n\n_unit_cell\"\x8f\x01\n\x12IndexerImageOutput\x12\x14\n\x0cimage_number\x18\x01 \x01(\x03\x12\x12\n\nspot_count\x18\x02 \x01(\x03\x12\x0f\n\x07indexed\x18\x03 \x01(\x08\x12\x30\n\tunit_cell\x18\x04 \x01(\x0b\x32\x18.JFJochProtoBuf.UnitCellH\x00\x88\x01\x01\x42\x0c\n\n_unit_cell\"w\n\rIndexerOutput\x12\x38\n\x0cimage_output\x18\x01 \x03(\x0b\x32\".JFJochProtoBuf.IndexerImageOutput\x12\x14\n\x0cms_per_image\x18\x02 \x01(\x01\x12\x16\n\x0eindexed_images\x18\x03 \x01(\x03\"\x85\x02\n\x10ModuleStatistics\x12\x15\n\rmodule_number\x18\x01 \x01(\x03\x12\x1b\n\x13storage_cell_number\x18\x02 \x01(\x03\x12\x18\n\x10pedestal_g0_mean\x18\x03 \x01(\x01\x12\x18\n\x10pedestal_g1_mean\x18\x04 \x01(\x01\x12\x18\n\x10pedestal_g2_mean\x18\x05 \x01(\x01\x12\x1c\n\x14pedestal_rms_g0_mean\x18\x06 \x01(\x01\x12\x1c\n\x14pedestal_rms_g1_mean\x18\x07 \x01(\x01\x12\x1c\n\x14pedestal_rms_g2_mean\x18\x08 \x01(\x01\x12\x15\n\rmasked_pixels\x18\t \x01(\x04\"4\n\x05Image\x12\x0c\n\x04\x64\x61ta\x18\x01 \x01(\x0c\x12\r\n\x05width\x18\x02 \x01(\x03\x12\x0e\n\x06height\x18\x03 \x01(\x03\".\n\nMaskToLoad\x12\x0c\n\x04mask\x18\x01 \x03(\r\x12\x12\n\nbit_to_set\x18\x02 \x01(\x05\"\xec\x07\n\x0b\x42rokerSetup\x12\x1a\n\x12images_per_trigger\x18\x01 \x01(\x03\x12\x16\n\tsummation\x18\x02 \x01(\x03H\x00\x88\x01\x01\x12\x19\n\x11\x62\x65\x61m_center_x_pxl\x18\x03 \x01(\x01\x12\x19\n\x11\x62\x65\x61m_center_y_pxl\x18\x04 \x01(\x01\x12\x1c\n\x14\x64\x65tector_distance_mm\x18\x05 \x01(\x01\x12\x19\n\x11photon_energy_kev\x18\x06 \x01(\x01\x12\x19\n\x0cname_pattern\x18\x07 \x01(\tH\x01\x88\x01\x01\x12\x13\n\x0bsample_name\x18\x08 \x01(\t\x12\x19\n\x0csoft_trigger\x18\x0e \x01(\x08H\x02\x88\x01\x01\x12\x15\n\x08ntrigger\x18\x0f \x01(\x03H\x03\x88\x01\x01\x12\x1c\n\x0fimages_per_file\x18\x10 \x01(\x03H\x04\x88\x01\x01\x12\x18\n\x0b\x63ompression\x18\x11 \x01(\tH\x05\x88\x01\x01\x12\x1c\n\x0fomega_start_deg\x18\x12 \x01(\x01H\x06\x88\x01\x01\x12 \n\x13omega_increment_deg\x18\x13 \x01(\x01H\x07\x88\x01\x01\x12\x1f\n\x12time_resolved_mode\x18\x14 \x01(\x08H\x08\x88\x01\x01\x12!\n\x14\x64\x61ta_collection_mode\x18\x17 \x01(\tH\t\x88\x01\x01\x12\x1c\n\x0fpreview_rate_ms\x18\x1b \x01(\x03H\n\x88\x01\x01\x12+\n\tunit_cell\x18\x1d \x01(\x0b\x32\x18.JFJochProtoBuf.UnitCell\x12!\n\x14spot_finding_rate_ms\x18\x1e \x01(\x03H\x0b\x88\x01\x01\x12!\n\x14\x62kg_estimate_rate_ms\x18\x1f \x01(\x03H\x0c\x88\x01\x01\x12\x1f\n\x12space_group_number\x18! \x01(\x03H\r\x88\x01\x01\x12,\n\x1f\x64\x65tector_delay_after_trigger_us\x18# \x01(\x03H\x0e\x88\x01\x01\x42\x0c\n\n_summationB\x0f\n\r_name_patternB\x0f\n\r_soft_triggerB\x0b\n\t_ntriggerB\x12\n\x10_images_per_fileB\x0e\n\x0c_compressionB\x12\n\x10_omega_start_degB\x16\n\x14_omega_increment_degB\x15\n\x13_time_resolved_modeB\x17\n\x15_data_collection_modeB\x12\n\x10_preview_rate_msB\x17\n\x15_spot_finding_rate_msB\x17\n\x15_bkg_estimate_rate_msB\x15\n\x13_space_group_numberB\"\n _detector_delay_after_trigger_us\"e\n\x1e\x42rokerPersistentTimingSettings\x12\x15\n\rframe_time_us\x18\x01 \x01(\x03\x12\x1a\n\rcount_time_us\x18\x02 \x01(\x03H\x00\x88\x01\x01\x42\x10\n\x0e_count_time_us\"\xbe\x03\n\x18\x42rokerPersistentSettings\x12\x1e\n\x11use_storage_cells\x18\x01 \x01(\x08H\x00\x88\x01\x01\x12\x43\n\x06timing\x18\x02 \x01(\x0b\x32..JFJochProtoBuf.BrokerPersistentTimingSettingsH\x01\x88\x01\x01\x12\x1f\n\x12pedestal_g0_frames\x18\x03 \x01(\x03H\x02\x88\x01\x01\x12\x1f\n\x12pedestal_g1_frames\x18\x04 \x01(\x03H\x03\x88\x01\x01\x12\x1f\n\x12pedestal_g2_frames\x18\x05 \x01(\x03H\x04\x88\x01\x01\x12*\n\x1duse_internal_packet_generator\x18\x06 \x01(\x08H\x05\x88\x01\x01\x12\x17\n\nrun_number\x18\x07 \x01(\x03H\x06\x88\x01\x01\x42\x14\n\x12_use_storage_cellsB\t\n\x07_timingB\x15\n\x13_pedestal_g0_framesB\x15\n\x13_pedestal_g1_framesB\x15\n\x13_pedestal_g2_framesB \n\x1e_use_internal_packet_generatorB\r\n\x0b_run_number\"\xde\x01\n\x15MeasurementStatistics\x12\x18\n\x10images_collected\x18\x01 \x01(\x04\x12\x1d\n\x15\x63ollection_efficiency\x18\x02 \x01(\x01\x12\x19\n\x11\x63ompression_ratio\x18\x03 \x01(\x01\x12\x17\n\nrun_number\x18\x04 \x01(\x03H\x00\x88\x01\x01\x12\x11\n\tfile_name\x18\x05 \x01(\t\x12\x1e\n\x16writer_performance_MBs\x18\x06 \x01(\x01\x12\x16\n\x0eimages_written\x18\x07 \x01(\x04\x42\r\n\x0b_run_number\"\xb2\x04\n\x0c\x42rokerStatus\x12\x39\n\rcurrent_state\x18\x01 \x01(\x0e\x32\".JFJochProtoBuf.BrokerStatus.State\x12<\n\x0freceiver_status\x18\x02 \x01(\x0b\x32\x1e.JFJochProtoBuf.ReceiverStatusH\x00\x88\x01\x01\x12:\n\x0eindexer_status\x18\x03 \x01(\x0b\x32\x1d.JFJochProtoBuf.IndexerStatusH\x01\x88\x01\x01\x12J\n\x16measurement_statistics\x18\x04 \x01(\x0b\x32%.JFJochProtoBuf.MeasurementStatisticsH\x02\x88\x01\x01\x12G\n\x16\x63\x61libration_statistics\x18\n \x01(\x0b\x32\'.JFJochProtoBuf.JFCalibrationStatistics\x12\x36\n\x0e\x64\x65tector_state\x18\x0b \x01(\x0b\x32\x1e.JFJochProtoBuf.DetectorStatus\"^\n\x05State\x12\x13\n\x0fNOT_INITIALIZED\x10\x00\x12\x08\n\x04IDLE\x10\x01\x12\x08\n\x04\x42USY\x10\x02\x12\x0c\n\x08PEDESTAL\x10\x03\x12\x13\n\x0f\x44\x41TA_COLLECTION\x10\x04\x12\t\n\x05\x45RROR\x10\x05\x42\x12\n\x10_receiver_statusB\x11\n\x0f_indexer_statusB\x19\n\x17_measurement_statistics\"\xd4\x01\n\x10\x42rokerFullStatus\x12\x30\n\x08receiver\x18\x01 \x01(\x0b\x32\x1e.JFJochProtoBuf.ReceiverOutput\x12\x30\n\x08\x64\x65tector\x18\x02 \x01(\x0b\x32\x1e.JFJochProtoBuf.DetectorOutput\x12,\n\x06writer\x18\x03 \x03(\x0b\x32\x1c.JFJochProtoBuf.WriterOutput\x12.\n\x07indexer\x18\x04 \x01(\x0b\x32\x1d.JFJochProtoBuf.IndexerOutput*@\n\x0b\x43ompression\x12\r\n\tBSHUF_LZ4\x10\x00\x12\x0e\n\nBSHUF_ZSTD\x10\x01\x12\x12\n\x0eNO_COMPRESSION\x10\x02*\'\n\x0c\x44\x65tectorType\x12\x0c\n\x08JUNGFRAU\x10\x00\x12\t\n\x05\x45IGER\x10\x01*Z\n\x0c\x44\x65tectorMode\x12\x0e\n\nCONVERSION\x10\x00\x12\x07\n\x03RAW\x10\x01\x12\x0f\n\x0bPEDESTAL_G0\x10\x02\x12\x0f\n\x0bPEDESTAL_G1\x10\x03\x12\x0f\n\x0bPEDESTAL_G2\x10\x04*2\n\x0e\x46PGAFIFOStatus\x12\t\n\x05\x45MPTY\x10\x00\x12\x08\n\x04\x46ULL\x10\x01\x12\x0b\n\x07PARTIAL\x10\x02*H\n\x0fWriterFrameType\x12\x13\n\x0f\x45ND_ACQUISITION\x10\x00\x12\x15\n\x11START_ACQUISITION\x10\x01\x12\t\n\x05IMAGE\x10\x02*D\n\x12\x44\x65tectorStatusEnum\x12\x08\n\x04IDLE\x10\x00\x12\t\n\x05\x45RROR\x10\x01\x12\x08\n\x04\x42USY\x10\x02\x12\x0f\n\x0bNO_DETECTOR\x10\x03\x32\xb2\x0b\n\x11gRPC_JFJochBroker\x12=\n\x05Start\x12\x1b.JFJochProtoBuf.BrokerSetup\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x36\n\x04Stop\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x37\n\x05\x41\x62ort\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x38\n\x06\x43\x61ncel\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12:\n\x08Pedestal\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12<\n\nInitialize\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12<\n\nDeactivate\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12J\n\x05Setup\x12(.JFJochProtoBuf.BrokerPersistentSettings\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12M\n\x08GetSetup\x12\x15.JFJochProtoBuf.Empty\x1a(.JFJochProtoBuf.BrokerPersistentSettings\"\x00\x12\x42\n\tGetStatus\x12\x15.JFJochProtoBuf.Empty\x1a\x1c.JFJochProtoBuf.BrokerStatus\"\x00\x12H\n\x0eGetCalibration\x12\x15.JFJochProtoBuf.Empty\x1a\x1d.JFJochProtoBuf.JFCalibration\"\x00\x12J\n\rGetFullStatus\x12\x15.JFJochProtoBuf.Empty\x1a .JFJochProtoBuf.BrokerFullStatus\"\x00\x12\\\n\x19GetDataProcessingSettings\x12\x15.JFJochProtoBuf.Empty\x1a&.JFJochProtoBuf.DataProcessingSettings\"\x00\x12\\\n\x19SetDataProcessingSettings\x12&.JFJochProtoBuf.DataProcessingSettings\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12?\n\x08LoadMask\x12\x1a.JFJochProtoBuf.MaskToLoad\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x39\n\x07GetMask\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Image\"\x00\x12\x41\n\x0fGetMaskRawCoord\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Image\"\x00\x12?\n\rGetPedestalG0\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Image\"\x00\x12?\n\rGetPedestalG1\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Image\"\x00\x12?\n\rGetPedestalG2\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Image\"\x00\x12H\n\x0fGetPreviewFrame\x12\x15.JFJochProtoBuf.Empty\x1a\x1c.JFJochProtoBuf.PreviewFrame\"\x00\x32\xcc\x04\n\x13gRPC_JFJochReceiver\x12?\n\x05Start\x12\x1d.JFJochProtoBuf.ReceiverInput\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x37\n\x05\x41\x62ort\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x38\n\x06\x43\x61ncel\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12?\n\x04Stop\x12\x15.JFJochProtoBuf.Empty\x1a\x1e.JFJochProtoBuf.ReceiverOutput\"\x00\x12\x44\n\tGetStatus\x12\x15.JFJochProtoBuf.Empty\x1a\x1e.JFJochProtoBuf.ReceiverStatus\"\x00\x12\\\n\x19SetDataProcessingSettings\x12&.JFJochProtoBuf.DataProcessingSettings\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12H\n\x0fGetPreviewFrame\x12\x15.JFJochProtoBuf.Empty\x1a\x1c.JFJochProtoBuf.PreviewFrame\"\x00\x12R\n\x10GetNetworkConfig\x12\x15.JFJochProtoBuf.Empty\x1a%.JFJochProtoBuf.ReceiverNetworkConfig\"\x00\x32\x9b\x02\n\x11gRPC_JFJochWriter\x12=\n\x05Start\x12\x1b.JFJochProtoBuf.WriterInput\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x37\n\x05\x41\x62ort\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12=\n\x04Stop\x12\x15.JFJochProtoBuf.Empty\x1a\x1c.JFJochProtoBuf.WriterOutput\"\x00\x12O\n\x0fWriteMasterFile\x12#.JFJochProtoBuf.WriterMetadataInput\x1a\x15.JFJochProtoBuf.Empty\"\x00\x32\x82\x03\n\x13gRPC_JFJochDetector\x12?\n\x05Start\x12\x1d.JFJochProtoBuf.DetectorInput\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x36\n\x04Stop\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x41\n\x06Status\x12\x15.JFJochProtoBuf.Empty\x1a\x1e.JFJochProtoBuf.DetectorStatus\"\x00\x12=\n\x02On\x12\x1e.JFJochProtoBuf.DetectorConfig\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x35\n\x03Off\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12\x39\n\x07Trigger\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x32O\n\x12gRPC_JFJochTrigger\x12\x39\n\x07Trigger\x12\x15.JFJochProtoBuf.Empty\x1a\x15.JFJochProtoBuf.Empty\"\x00\x32\xd9\x01\n\x12gRPC_JFJochIndexer\x12>\n\x05Start\x12\x1c.JFJochProtoBuf.IndexerInput\x1a\x15.JFJochProtoBuf.Empty\"\x00\x12>\n\x04Stop\x12\x15.JFJochProtoBuf.Empty\x1a\x1d.JFJochProtoBuf.IndexerOutput\"\x00\x12\x43\n\tGetStatus\x12\x15.JFJochProtoBuf.Empty\x1a\x1d.JFJochProtoBuf.IndexerStatus\"\x00\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'jfjoch_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _FPGASTATUS_FIFOSTATUSENTRY._options = None
  _FPGASTATUS_FIFOSTATUSENTRY._serialized_options = b'8\001'
  _COMPRESSION._serialized_start=13430
  _COMPRESSION._serialized_end=13494
  _DETECTORTYPE._serialized_start=13496
  _DETECTORTYPE._serialized_end=13535
  _DETECTORMODE._serialized_start=13537
  _DETECTORMODE._serialized_end=13627
  _FPGAFIFOSTATUS._serialized_start=13629
  _FPGAFIFOSTATUS._serialized_end=13679
  _WRITERFRAMETYPE._serialized_start=13681
  _WRITERFRAMETYPE._serialized_end=13753
  _DETECTORSTATUSENUM._serialized_start=13755
  _DETECTORSTATUSENUM._serialized_end=13823
  _EMPTY._serialized_start=32
  _EMPTY._serialized_end=39
  _UNITCELL._serialized_start=41
  _UNITCELL._serialized_end=128
  _VECTOR._serialized_start=130
  _VECTOR._serialized_end=171
  _FRAMECOUNTSETTINGS._serialized_start=173
  _FRAMECOUNTSETTINGS._serialized_end=239
  _TIMINGSETTINGS._serialized_start=242
  _TIMINGSETTINGS._serialized_end=387
  _DIFFRACTIONGEOMETRYSETTINGS._serialized_start=390
  _DIFFRACTIONGEOMETRYSETTINGS._serialized_end=759
  _ROTATIONSETTINGS._serialized_start=761
  _ROTATIONSETTINGS._serialized_end=885
  _BEAMLINEMETADATASETTINGS._serialized_start=887
  _BEAMLINEMETADATASETTINGS._serialized_end=936
  _COMPRESSIONSETTINGS._serialized_start=938
  _COMPRESSIONSETTINGS._serialized_end=1042
  _IMAGESAVINGSETTINGS._serialized_start=1044
  _IMAGESAVINGSETTINGS._serialized_end=1111
  _DETECTORSETTINGS._serialized_start=1114
  _DETECTORSETTINGS._serialized_end=1303
  _PREVIEWSETTINGS._serialized_start=1305
  _PREVIEWSETTINGS._serialized_end=1415
  _PEDESTALSETTINGS._serialized_start=1417
  _PEDESTALSETTINGS._serialized_end=1492
  _CONVERSIONSETTINGS._serialized_start=1495
  _CONVERSIONSETTINGS._serialized_end=1676
  _JUNGFRAUJOCHINTERNALSETTINGS._serialized_start=1678
  _JUNGFRAUJOCHINTERNALSETTINGS._serialized_end=1794
  _SAMPLESETTINGS._serialized_start=1797
  _SAMPLESETTINGS._serialized_end=1925
  _RADIALINTEGRATIONSETTINGS._serialized_start=1928
  _RADIALINTEGRATIONSETTINGS._serialized_end=2062
  _JUNGFRAUJOCHSETTINGS._serialized_start=2065
  _JUNGFRAUJOCHSETTINGS._serialized_end=2826
  _JFPEDESTAL._serialized_start=2828
  _JFPEDESTAL._serialized_end=2920
  _JFCALIBRATION._serialized_start=2922
  _JFCALIBRATION._serialized_end=3039
  _JFCALIBRATIONSTATISTICS._serialized_start=3041
  _JFCALIBRATIONSTATISTICS._serialized_end=3127
  _ACQUISITIONDEVICESTATISTICS._serialized_start=3130
  _ACQUISITIONDEVICESTATISTICS._serialized_end=3554
  _RECEIVERINPUT._serialized_start=3557
  _RECEIVERINPUT._serialized_end=3721
  _RECEIVEROUTPUT._serialized_start=3724
  _RECEIVEROUTPUT._serialized_end=4245
  _RECEIVERNETWORKCONFIG._serialized_start=4247
  _RECEIVERNETWORKCONFIG._serialized_end=4293
  _DATAFILE._serialized_start=4295
  _DATAFILE._serialized_end=4360
  _WRITERINPUT._serialized_start=4363
  _WRITERINPUT._serialized_end=4692
  _WRITEROUTPUT._serialized_start=4694
  _WRITEROUTPUT._serialized_end=4774
  _FACILITYMETADATA._serialized_start=4776
  _FACILITYMETADATA._serialized_end=4898
  _EXPERIMENTGEOMETRYMETADATA._serialized_start=4901
  _EXPERIMENTGEOMETRYMETADATA._serialized_end=5068
  _DETECTORMETADATA._serialized_start=5071
  _DETECTORMETADATA._serialized_end=5562
  _BEAMMETADATA._serialized_start=5565
  _BEAMMETADATA._serialized_end=5787
  _SAMPLEMETADATA._serialized_start=5790
  _SAMPLEMETADATA._serialized_end=6036
  _CALIBRATIONMETADATAENTRY._serialized_start=6038
  _CALIBRATIONMETADATAENTRY._serialized_end=6123
  _CALIBRATIONMETADATA._serialized_start=6126
  _CALIBRATIONMETADATA._serialized_end=6258
  _WRITERMETADATAINPUT._serialized_start=6261
  _WRITERMETADATAINPUT._serialized_end=6866
  _WRITERSPOT._serialized_start=6868
  _WRITERSPOT._serialized_end=6921
  _WRITERFRAME._serialized_start=6924
  _WRITERFRAME._serialized_end=7146
  _DETECTORMODULECONFIG._serialized_start=7149
  _DETECTORMODULECONFIG._serialized_end=7373
  _DETECTORCONFIG._serialized_start=7375
  _DETECTORCONFIG._serialized_end=7446
  _DETECTORINPUT._serialized_start=7449
  _DETECTORINPUT._serialized_end=7738
  _DETECTOROUTPUT._serialized_start=7740
  _DETECTOROUTPUT._serialized_end=7756
  _DETECTORSTATUS._serialized_start=7758
  _DETECTORSTATUS._serialized_end=7870
  _FPGASTATUS._serialized_start=7873
  _FPGASTATUS._serialized_end=8922
  _FPGASTATUS_FIFOSTATUSENTRY._serialized_start=8841
  _FPGASTATUS_FIFOSTATUSENTRY._serialized_end=8922
  _PLOT._serialized_start=8924
  _PLOT._serialized_end=8952
  _RECEIVERSTATUS._serialized_start=8955
  _RECEIVERSTATUS._serialized_end=9214
  _DATAPROCESSINGSETTINGS._serialized_start=9217
  _DATAPROCESSINGSETTINGS._serialized_end=9506
  _PREVIEWFRAME._serialized_start=9509
  _PREVIEWFRAME._serialized_end=9836
  _SPOTFINDERIMAGEOUTPUT._serialized_start=9838
  _SPOTFINDERIMAGEOUTPUT._serialized_end=9922
  _INDEXERSTATUS._serialized_start=9924
  _INDEXERSTATUS._serialized_end=10033
  _INDEXERINPUT._serialized_start=10036
  _INDEXERINPUT._serialized_end=10231
  _INDEXERIMAGEOUTPUT._serialized_start=10234
  _INDEXERIMAGEOUTPUT._serialized_end=10377
  _INDEXEROUTPUT._serialized_start=10379
  _INDEXEROUTPUT._serialized_end=10498
  _MODULESTATISTICS._serialized_start=10501
  _MODULESTATISTICS._serialized_end=10762
  _IMAGE._serialized_start=10764
  _IMAGE._serialized_end=10816
  _MASKTOLOAD._serialized_start=10818
  _MASKTOLOAD._serialized_end=10864
  _BROKERSETUP._serialized_start=10867
  _BROKERSETUP._serialized_end=11871
  _BROKERPERSISTENTTIMINGSETTINGS._serialized_start=11873
  _BROKERPERSISTENTTIMINGSETTINGS._serialized_end=11974
  _BROKERPERSISTENTSETTINGS._serialized_start=11977
  _BROKERPERSISTENTSETTINGS._serialized_end=12423
  _MEASUREMENTSTATISTICS._serialized_start=12426
  _MEASUREMENTSTATISTICS._serialized_end=12648
  _BROKERSTATUS._serialized_start=12651
  _BROKERSTATUS._serialized_end=13213
  _BROKERSTATUS_STATE._serialized_start=13053
  _BROKERSTATUS_STATE._serialized_end=13147
  _BROKERFULLSTATUS._serialized_start=13216
  _BROKERFULLSTATUS._serialized_end=13428
  _GRPC_JFJOCHBROKER._serialized_start=13826
  _GRPC_JFJOCHBROKER._serialized_end=15284
  _GRPC_JFJOCHRECEIVER._serialized_start=15287
  _GRPC_JFJOCHRECEIVER._serialized_end=15875
  _GRPC_JFJOCHWRITER._serialized_start=15878
  _GRPC_JFJOCHWRITER._serialized_end=16161
  _GRPC_JFJOCHDETECTOR._serialized_start=16164
  _GRPC_JFJOCHDETECTOR._serialized_end=16550
  _GRPC_JFJOCHTRIGGER._serialized_start=16552
  _GRPC_JFJOCHTRIGGER._serialized_end=16631
  _GRPC_JFJOCHINDEXER._serialized_start=16634
  _GRPC_JFJOCHINDEXER._serialized_end=16851
# @@protoc_insertion_point(module_scope)
