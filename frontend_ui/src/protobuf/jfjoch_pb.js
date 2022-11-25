// source: jfjoch.proto
/**
 * @fileoverview
 * @enhanceable
 * @suppress {missingRequire} reports error on implicit type usages.
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!
/* eslint-disable */
// @ts-nocheck

var jspb = require('google-protobuf');
var goog = jspb;
var global = (function() {
  if (this) { return this; }
  if (typeof window !== 'undefined') { return window; }
  if (typeof global !== 'undefined') { return global; }
  if (typeof self !== 'undefined') { return self; }
  return Function('return this')();
}.call(null));

goog.exportSymbol('proto.JFJochProtoBuf.AcquisitionDeviceStatistics', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BeamMetadata', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BeamlineMetadataSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerFullStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerPersistentSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerPersistentTimingSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerSetup', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.BrokerStatus.State', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.CalibrationMetadata', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.Compression', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.CompressionSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ConversionSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DataFile', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DataProcessingSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorConfig', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorInput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorMetadata', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorMode', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorModuleConfig', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorStatusEnum', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DetectorType', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.DiffractionGeometrySettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.Empty', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ExperimentGeometry', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.FPGAFIFOStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.FPGAStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.FacilityMetadata', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.FrameCountSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.Image', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ImageSavingSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.IndexerImageOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.IndexerInput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.IndexerOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.IndexerStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.JFCalibration', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.JFCalibrationStatistics', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.JFPedestal', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.JungfraujochInternalSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.JungfraujochSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.MaskToLoad', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.MeasurementStatistics', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ModuleStatistics', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.PedestalSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.Plot', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.PreviewFrame', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.PreviewSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.RadialIntegrationSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ReceiverInput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ReceiverNetworkConfig', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ReceiverOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.ReceiverStatus', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.RotationSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.SampleMetadata', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.SampleSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.SpotFinderImageOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.TimingSettings', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.UnitCell', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.Vector', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterFrame', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterFrameType', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterInput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterMetadataInput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterOutput', null, global);
goog.exportSymbol('proto.JFJochProtoBuf.WriterSpot', null, global);
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.Empty = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.Empty, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.Empty.displayName = 'proto.JFJochProtoBuf.Empty';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.UnitCell = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.UnitCell, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.UnitCell.displayName = 'proto.JFJochProtoBuf.UnitCell';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.Vector = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.Vector, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.Vector.displayName = 'proto.JFJochProtoBuf.Vector';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.FrameCountSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.FrameCountSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.FrameCountSettings.displayName = 'proto.JFJochProtoBuf.FrameCountSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.TimingSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.TimingSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.TimingSettings.displayName = 'proto.JFJochProtoBuf.TimingSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.DiffractionGeometrySettings.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.DiffractionGeometrySettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DiffractionGeometrySettings.displayName = 'proto.JFJochProtoBuf.DiffractionGeometrySettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.RotationSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.RotationSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.RotationSettings.displayName = 'proto.JFJochProtoBuf.RotationSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BeamlineMetadataSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BeamlineMetadataSettings.displayName = 'proto.JFJochProtoBuf.BeamlineMetadataSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.CompressionSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.CompressionSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.CompressionSettings.displayName = 'proto.JFJochProtoBuf.CompressionSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ImageSavingSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.ImageSavingSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ImageSavingSettings.displayName = 'proto.JFJochProtoBuf.ImageSavingSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorSettings.displayName = 'proto.JFJochProtoBuf.DetectorSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.PreviewSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.PreviewSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.PreviewSettings.displayName = 'proto.JFJochProtoBuf.PreviewSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.PedestalSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.PedestalSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.PedestalSettings.displayName = 'proto.JFJochProtoBuf.PedestalSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ConversionSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.ConversionSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ConversionSettings.displayName = 'proto.JFJochProtoBuf.ConversionSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.JungfraujochInternalSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.JungfraujochInternalSettings.displayName = 'proto.JFJochProtoBuf.JungfraujochInternalSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.SampleSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.SampleSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.SampleSettings.displayName = 'proto.JFJochProtoBuf.SampleSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.RadialIntegrationSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.RadialIntegrationSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.RadialIntegrationSettings.displayName = 'proto.JFJochProtoBuf.RadialIntegrationSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.JungfraujochSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.JungfraujochSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.JungfraujochSettings.displayName = 'proto.JFJochProtoBuf.JungfraujochSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.JFPedestal = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.JFPedestal, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.JFPedestal.displayName = 'proto.JFJochProtoBuf.JFPedestal';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.JFCalibration = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.JFCalibration.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.JFCalibration, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.JFCalibration.displayName = 'proto.JFJochProtoBuf.JFCalibration';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.JFCalibrationStatistics = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.JFCalibrationStatistics.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.JFCalibrationStatistics, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.JFCalibrationStatistics.displayName = 'proto.JFJochProtoBuf.JFCalibrationStatistics';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.AcquisitionDeviceStatistics.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.AcquisitionDeviceStatistics, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.AcquisitionDeviceStatistics.displayName = 'proto.JFJochProtoBuf.AcquisitionDeviceStatistics';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ReceiverInput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.ReceiverInput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.ReceiverInput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ReceiverInput.displayName = 'proto.JFJochProtoBuf.ReceiverInput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ReceiverOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.ReceiverOutput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.ReceiverOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ReceiverOutput.displayName = 'proto.JFJochProtoBuf.ReceiverOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.ReceiverNetworkConfig.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.ReceiverNetworkConfig, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ReceiverNetworkConfig.displayName = 'proto.JFJochProtoBuf.ReceiverNetworkConfig';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DataFile = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DataFile, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DataFile.displayName = 'proto.JFJochProtoBuf.DataFile';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.WriterInput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.WriterInput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.WriterInput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.WriterInput.displayName = 'proto.JFJochProtoBuf.WriterInput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.WriterOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.WriterOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.WriterOutput.displayName = 'proto.JFJochProtoBuf.WriterOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.FacilityMetadata = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.FacilityMetadata, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.FacilityMetadata.displayName = 'proto.JFJochProtoBuf.FacilityMetadata';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ExperimentGeometry = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.ExperimentGeometry, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ExperimentGeometry.displayName = 'proto.JFJochProtoBuf.ExperimentGeometry';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorMetadata = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorMetadata, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorMetadata.displayName = 'proto.JFJochProtoBuf.DetectorMetadata';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BeamMetadata = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BeamMetadata, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BeamMetadata.displayName = 'proto.JFJochProtoBuf.BeamMetadata';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.SampleMetadata = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.SampleMetadata, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.SampleMetadata.displayName = 'proto.JFJochProtoBuf.SampleMetadata';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.CalibrationMetadata = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.CalibrationMetadata, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.CalibrationMetadata.displayName = 'proto.JFJochProtoBuf.CalibrationMetadata';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.WriterMetadataInput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.WriterMetadataInput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.WriterMetadataInput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.WriterMetadataInput.displayName = 'proto.JFJochProtoBuf.WriterMetadataInput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.WriterSpot = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.WriterSpot, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.WriterSpot.displayName = 'proto.JFJochProtoBuf.WriterSpot';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.WriterFrame = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.WriterFrame.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.WriterFrame, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.WriterFrame.displayName = 'proto.JFJochProtoBuf.WriterFrame';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorModuleConfig = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorModuleConfig, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorModuleConfig.displayName = 'proto.JFJochProtoBuf.DetectorModuleConfig';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorConfig = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.DetectorConfig.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorConfig, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorConfig.displayName = 'proto.JFJochProtoBuf.DetectorConfig';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorInput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorInput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorInput.displayName = 'proto.JFJochProtoBuf.DetectorInput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorOutput.displayName = 'proto.JFJochProtoBuf.DetectorOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DetectorStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DetectorStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DetectorStatus.displayName = 'proto.JFJochProtoBuf.DetectorStatus';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.FPGAStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.FPGAStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.FPGAStatus.displayName = 'proto.JFJochProtoBuf.FPGAStatus';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.Plot = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.Plot.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.Plot, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.Plot.displayName = 'proto.JFJochProtoBuf.Plot';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ReceiverStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.ReceiverStatus.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.ReceiverStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ReceiverStatus.displayName = 'proto.JFJochProtoBuf.ReceiverStatus';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.DataProcessingSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.DataProcessingSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.DataProcessingSettings.displayName = 'proto.JFJochProtoBuf.DataProcessingSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.PreviewFrame = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.PreviewFrame, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.PreviewFrame.displayName = 'proto.JFJochProtoBuf.PreviewFrame';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.SpotFinderImageOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.SpotFinderImageOutput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.SpotFinderImageOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.SpotFinderImageOutput.displayName = 'proto.JFJochProtoBuf.SpotFinderImageOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.IndexerStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.IndexerStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.IndexerStatus.displayName = 'proto.JFJochProtoBuf.IndexerStatus';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.IndexerInput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.IndexerInput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.IndexerInput.displayName = 'proto.JFJochProtoBuf.IndexerInput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.IndexerImageOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.IndexerImageOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.IndexerImageOutput.displayName = 'proto.JFJochProtoBuf.IndexerImageOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.IndexerOutput = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.IndexerOutput.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.IndexerOutput, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.IndexerOutput.displayName = 'proto.JFJochProtoBuf.IndexerOutput';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.ModuleStatistics = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.ModuleStatistics, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.ModuleStatistics.displayName = 'proto.JFJochProtoBuf.ModuleStatistics';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.Image = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.Image, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.Image.displayName = 'proto.JFJochProtoBuf.Image';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.MaskToLoad = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.MaskToLoad.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.MaskToLoad, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.MaskToLoad.displayName = 'proto.JFJochProtoBuf.MaskToLoad';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BrokerSetup = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BrokerSetup, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BrokerSetup.displayName = 'proto.JFJochProtoBuf.BrokerSetup';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BrokerPersistentTimingSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BrokerPersistentTimingSettings.displayName = 'proto.JFJochProtoBuf.BrokerPersistentTimingSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BrokerPersistentSettings = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BrokerPersistentSettings, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BrokerPersistentSettings.displayName = 'proto.JFJochProtoBuf.BrokerPersistentSettings';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.MeasurementStatistics = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.MeasurementStatistics, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.MeasurementStatistics.displayName = 'proto.JFJochProtoBuf.MeasurementStatistics';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BrokerStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.JFJochProtoBuf.BrokerStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BrokerStatus.displayName = 'proto.JFJochProtoBuf.BrokerStatus';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.JFJochProtoBuf.BrokerFullStatus = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.JFJochProtoBuf.BrokerFullStatus.repeatedFields_, null);
};
goog.inherits(proto.JFJochProtoBuf.BrokerFullStatus, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.JFJochProtoBuf.BrokerFullStatus.displayName = 'proto.JFJochProtoBuf.BrokerFullStatus';
}



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.Empty.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.Empty.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.Empty} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Empty.toObject = function(includeInstance, msg) {
  var f, obj = {

  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.Empty}
 */
proto.JFJochProtoBuf.Empty.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.Empty;
  return proto.JFJochProtoBuf.Empty.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.Empty} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.Empty}
 */
proto.JFJochProtoBuf.Empty.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.Empty.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.Empty.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.Empty} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Empty.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.UnitCell.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.UnitCell.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.UnitCell} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.UnitCell.toObject = function(includeInstance, msg) {
  var f, obj = {
    a: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    b: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    c: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    alpha: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    beta: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0),
    gamma: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.UnitCell.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.UnitCell;
  return proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.UnitCell} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setA(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setB(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setC(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setAlpha(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeta(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setGamma(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.UnitCell.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.UnitCell} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getA();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getB();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getC();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getAlpha();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getBeta();
  if (f !== 0.0) {
    writer.writeDouble(
      5,
      f
    );
  }
  f = message.getGamma();
  if (f !== 0.0) {
    writer.writeDouble(
      6,
      f
    );
  }
};


/**
 * optional double a = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getA = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setA = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double b = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getB = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setB = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double c = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getC = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setC = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional double alpha = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getAlpha = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setAlpha = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional double beta = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getBeta = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setBeta = function(value) {
  return jspb.Message.setProto3FloatField(this, 5, value);
};


/**
 * optional double gamma = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.UnitCell.prototype.getGamma = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.UnitCell} returns this
 */
proto.JFJochProtoBuf.UnitCell.prototype.setGamma = function(value) {
  return jspb.Message.setProto3FloatField(this, 6, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.Vector.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.Vector.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.Vector} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Vector.toObject = function(includeInstance, msg) {
  var f, obj = {
    x: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    y: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    z: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.Vector}
 */
proto.JFJochProtoBuf.Vector.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.Vector;
  return proto.JFJochProtoBuf.Vector.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.Vector} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.Vector}
 */
proto.JFJochProtoBuf.Vector.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setX(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setY(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setZ(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.Vector.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.Vector.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.Vector} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Vector.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getX();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getY();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getZ();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
};


/**
 * optional double x = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.Vector.prototype.getX = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.Vector} returns this
 */
proto.JFJochProtoBuf.Vector.prototype.setX = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double y = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.Vector.prototype.getY = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.Vector} returns this
 */
proto.JFJochProtoBuf.Vector.prototype.setY = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double z = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.Vector.prototype.getZ = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.Vector} returns this
 */
proto.JFJochProtoBuf.Vector.prototype.setZ = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.FrameCountSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.FrameCountSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FrameCountSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    imagesPerTrigger: jspb.Message.getFieldWithDefault(msg, 1, 0),
    ntrigger: jspb.Message.getFieldWithDefault(msg, 2, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.FrameCountSettings}
 */
proto.JFJochProtoBuf.FrameCountSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.FrameCountSettings;
  return proto.JFJochProtoBuf.FrameCountSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.FrameCountSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.FrameCountSettings}
 */
proto.JFJochProtoBuf.FrameCountSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesPerTrigger(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setNtrigger(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.FrameCountSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.FrameCountSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FrameCountSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImagesPerTrigger();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getNtrigger();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
};


/**
 * optional int64 images_per_trigger = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.getImagesPerTrigger = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FrameCountSettings} returns this
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.setImagesPerTrigger = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 ntrigger = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.getNtrigger = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FrameCountSettings} returns this
 */
proto.JFJochProtoBuf.FrameCountSettings.prototype.setNtrigger = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.TimingSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.TimingSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.TimingSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    frameTimePedestalg1g2Us: jspb.Message.getFieldWithDefault(msg, 1, 0),
    frameTimeUs: jspb.Message.getFieldWithDefault(msg, 3, 0),
    countTimeUs: jspb.Message.getFieldWithDefault(msg, 4, 0),
    summation: jspb.Message.getFieldWithDefault(msg, 5, 0),
    timeResolvedMode: jspb.Message.getBooleanFieldWithDefault(msg, 13, false)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.TimingSettings}
 */
proto.JFJochProtoBuf.TimingSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.TimingSettings;
  return proto.JFJochProtoBuf.TimingSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.TimingSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.TimingSettings}
 */
proto.JFJochProtoBuf.TimingSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setFrameTimePedestalg1g2Us(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setFrameTimeUs(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setCountTimeUs(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSummation(value);
      break;
    case 13:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setTimeResolvedMode(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.TimingSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.TimingSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.TimingSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFrameTimePedestalg1g2Us();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getFrameTimeUs();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getCountTimeUs();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getSummation();
  if (f !== 0) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = message.getTimeResolvedMode();
  if (f) {
    writer.writeBool(
      13,
      f
    );
  }
};


/**
 * optional int64 frame_time_pedestalG1G2_us = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.getFrameTimePedestalg1g2Us = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.TimingSettings} returns this
 */
proto.JFJochProtoBuf.TimingSettings.prototype.setFrameTimePedestalg1g2Us = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 frame_time_us = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.getFrameTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.TimingSettings} returns this
 */
proto.JFJochProtoBuf.TimingSettings.prototype.setFrameTimeUs = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 count_time_us = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.getCountTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.TimingSettings} returns this
 */
proto.JFJochProtoBuf.TimingSettings.prototype.setCountTimeUs = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional int64 summation = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.getSummation = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.TimingSettings} returns this
 */
proto.JFJochProtoBuf.TimingSettings.prototype.setSummation = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional bool time_resolved_mode = 13;
 * @return {boolean}
 */
proto.JFJochProtoBuf.TimingSettings.prototype.getTimeResolvedMode = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 13, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.TimingSettings} returns this
 */
proto.JFJochProtoBuf.TimingSettings.prototype.setTimeResolvedMode = function(value) {
  return jspb.Message.setProto3BooleanField(this, 13, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.repeatedFields_ = [10,11];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DiffractionGeometrySettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DiffractionGeometrySettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    beamXPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    beamYPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    detectorDistanceMm: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    detectorSizeXPxl: jspb.Message.getFieldWithDefault(msg, 6, 0),
    detectorSizeYPxl: jspb.Message.getFieldWithDefault(msg, 7, 0),
    gapXPxl: jspb.Message.getFieldWithDefault(msg, 8, 0),
    gapYPxl: jspb.Message.getFieldWithDefault(msg, 9, 0),
    dataStreamModulesList: (f = jspb.Message.getRepeatedField(msg, 10)) == null ? undefined : f,
    firstPixelOfModuleList: (f = jspb.Message.getRepeatedField(msg, 11)) == null ? undefined : f,
    horizontalModuleStacking: jspb.Message.getFieldWithDefault(msg, 12, 0),
    scatteringVector: (f = msg.getScatteringVector()) && proto.JFJochProtoBuf.Vector.toObject(includeInstance, f),
    photonEnergyKev: jspb.Message.getFloatingPointFieldWithDefault(msg, 16, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DiffractionGeometrySettings;
  return proto.JFJochProtoBuf.DiffractionGeometrySettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DiffractionGeometrySettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamXPxl(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamYPxl(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setDetectorDistanceMm(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setDetectorSizeXPxl(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setDetectorSizeYPxl(value);
      break;
    case 8:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setGapXPxl(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setGapYPxl(value);
      break;
    case 10:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedInt64() : [reader.readInt64()]);
      for (var i = 0; i < values.length; i++) {
        msg.addDataStreamModules(values[i]);
      }
      break;
    case 11:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedInt64() : [reader.readInt64()]);
      for (var i = 0; i < values.length; i++) {
        msg.addFirstPixelOfModule(values[i]);
      }
      break;
    case 12:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setHorizontalModuleStacking(value);
      break;
    case 14:
      var value = new proto.JFJochProtoBuf.Vector;
      reader.readMessage(value,proto.JFJochProtoBuf.Vector.deserializeBinaryFromReader);
      msg.setScatteringVector(value);
      break;
    case 16:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPhotonEnergyKev(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DiffractionGeometrySettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DiffractionGeometrySettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getBeamXPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getBeamYPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getDetectorDistanceMm();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getDetectorSizeXPxl();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
  f = message.getDetectorSizeYPxl();
  if (f !== 0) {
    writer.writeInt64(
      7,
      f
    );
  }
  f = message.getGapXPxl();
  if (f !== 0) {
    writer.writeInt64(
      8,
      f
    );
  }
  f = message.getGapYPxl();
  if (f !== 0) {
    writer.writeInt64(
      9,
      f
    );
  }
  f = message.getDataStreamModulesList();
  if (f.length > 0) {
    writer.writePackedInt64(
      10,
      f
    );
  }
  f = message.getFirstPixelOfModuleList();
  if (f.length > 0) {
    writer.writePackedInt64(
      11,
      f
    );
  }
  f = message.getHorizontalModuleStacking();
  if (f !== 0) {
    writer.writeInt64(
      12,
      f
    );
  }
  f = message.getScatteringVector();
  if (f != null) {
    writer.writeMessage(
      14,
      f,
      proto.JFJochProtoBuf.Vector.serializeBinaryToWriter
    );
  }
  f = message.getPhotonEnergyKev();
  if (f !== 0.0) {
    writer.writeDouble(
      16,
      f
    );
  }
};


/**
 * optional double beam_x_pxl = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getBeamXPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setBeamXPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double beam_y_pxl = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getBeamYPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setBeamYPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double detector_distance_mm = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getDetectorDistanceMm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setDetectorDistanceMm = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional int64 detector_size_x_pxl = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getDetectorSizeXPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setDetectorSizeXPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional int64 detector_size_y_pxl = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getDetectorSizeYPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setDetectorSizeYPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional int64 gap_x_pxl = 8;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getGapXPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 8, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setGapXPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 8, value);
};


/**
 * optional int64 gap_y_pxl = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getGapYPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setGapYPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};


/**
 * repeated int64 data_stream_modules = 10;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getDataStreamModulesList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 10));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setDataStreamModulesList = function(value) {
  return jspb.Message.setField(this, 10, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.addDataStreamModules = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 10, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.clearDataStreamModulesList = function() {
  return this.setDataStreamModulesList([]);
};


/**
 * repeated int64 first_pixel_of_module = 11;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getFirstPixelOfModuleList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 11));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setFirstPixelOfModuleList = function(value) {
  return jspb.Message.setField(this, 11, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.addFirstPixelOfModule = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 11, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.clearFirstPixelOfModuleList = function() {
  return this.setFirstPixelOfModuleList([]);
};


/**
 * optional int64 horizontal_module_stacking = 12;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getHorizontalModuleStacking = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 12, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setHorizontalModuleStacking = function(value) {
  return jspb.Message.setProto3IntField(this, 12, value);
};


/**
 * optional Vector scattering_vector = 14;
 * @return {?proto.JFJochProtoBuf.Vector}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getScatteringVector = function() {
  return /** @type{?proto.JFJochProtoBuf.Vector} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Vector, 14));
};


/**
 * @param {?proto.JFJochProtoBuf.Vector|undefined} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
*/
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setScatteringVector = function(value) {
  return jspb.Message.setWrapperField(this, 14, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.clearScatteringVector = function() {
  return this.setScatteringVector(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.hasScatteringVector = function() {
  return jspb.Message.getField(this, 14) != null;
};


/**
 * optional double photon_energy_keV = 16;
 * @return {number}
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.getPhotonEnergyKev = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 16, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DiffractionGeometrySettings} returns this
 */
proto.JFJochProtoBuf.DiffractionGeometrySettings.prototype.setPhotonEnergyKev = function(value) {
  return jspb.Message.setProto3FloatField(this, 16, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.RotationSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.RotationSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.RotationSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    startAngleDeg: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    angleIncrPerImageDeg: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    rotationAxis: (f = msg.getRotationAxis()) && proto.JFJochProtoBuf.Vector.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.RotationSettings}
 */
proto.JFJochProtoBuf.RotationSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.RotationSettings;
  return proto.JFJochProtoBuf.RotationSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.RotationSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.RotationSettings}
 */
proto.JFJochProtoBuf.RotationSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setStartAngleDeg(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setAngleIncrPerImageDeg(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.Vector;
      reader.readMessage(value,proto.JFJochProtoBuf.Vector.deserializeBinaryFromReader);
      msg.setRotationAxis(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.RotationSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.RotationSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.RotationSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getStartAngleDeg();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getAngleIncrPerImageDeg();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getRotationAxis();
  if (f != null) {
    writer.writeMessage(
      3,
      f,
      proto.JFJochProtoBuf.Vector.serializeBinaryToWriter
    );
  }
};


/**
 * optional double start_angle_deg = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.getStartAngleDeg = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RotationSettings} returns this
 */
proto.JFJochProtoBuf.RotationSettings.prototype.setStartAngleDeg = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double angle_incr_per_image_deg = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.getAngleIncrPerImageDeg = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RotationSettings} returns this
 */
proto.JFJochProtoBuf.RotationSettings.prototype.setAngleIncrPerImageDeg = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional Vector rotation_axis = 3;
 * @return {?proto.JFJochProtoBuf.Vector}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.getRotationAxis = function() {
  return /** @type{?proto.JFJochProtoBuf.Vector} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Vector, 3));
};


/**
 * @param {?proto.JFJochProtoBuf.Vector|undefined} value
 * @return {!proto.JFJochProtoBuf.RotationSettings} returns this
*/
proto.JFJochProtoBuf.RotationSettings.prototype.setRotationAxis = function(value) {
  return jspb.Message.setWrapperField(this, 3, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.RotationSettings} returns this
 */
proto.JFJochProtoBuf.RotationSettings.prototype.clearRotationAxis = function() {
  return this.setRotationAxis(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.RotationSettings.prototype.hasRotationAxis = function() {
  return jspb.Message.getField(this, 3) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BeamlineMetadataSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BeamlineMetadataSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    detectorName: jspb.Message.getFieldWithDefault(msg, 9, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BeamlineMetadataSettings}
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BeamlineMetadataSettings;
  return proto.JFJochProtoBuf.BeamlineMetadataSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BeamlineMetadataSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BeamlineMetadataSettings}
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 9:
      var value = /** @type {string} */ (reader.readString());
      msg.setDetectorName(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BeamlineMetadataSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BeamlineMetadataSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getDetectorName();
  if (f.length > 0) {
    writer.writeString(
      9,
      f
    );
  }
};


/**
 * optional string detector_name = 9;
 * @return {string}
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.prototype.getDetectorName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 9, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.BeamlineMetadataSettings} returns this
 */
proto.JFJochProtoBuf.BeamlineMetadataSettings.prototype.setDetectorName = function(value) {
  return jspb.Message.setProto3StringField(this, 9, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.CompressionSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.CompressionSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.CompressionSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    algorithm: jspb.Message.getFieldWithDefault(msg, 1, 0),
    level: jspb.Message.getFieldWithDefault(msg, 2, 0),
    blockSize: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.CompressionSettings}
 */
proto.JFJochProtoBuf.CompressionSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.CompressionSettings;
  return proto.JFJochProtoBuf.CompressionSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.CompressionSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.CompressionSettings}
 */
proto.JFJochProtoBuf.CompressionSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.Compression} */ (reader.readEnum());
      msg.setAlgorithm(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setLevel(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBlockSize(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.CompressionSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.CompressionSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.CompressionSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getAlgorithm();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getLevel();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getBlockSize();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional Compression algorithm = 1;
 * @return {!proto.JFJochProtoBuf.Compression}
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.getAlgorithm = function() {
  return /** @type {!proto.JFJochProtoBuf.Compression} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.Compression} value
 * @return {!proto.JFJochProtoBuf.CompressionSettings} returns this
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.setAlgorithm = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional int64 level = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.getLevel = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.CompressionSettings} returns this
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.setLevel = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 block_size = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.getBlockSize = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.CompressionSettings} returns this
 */
proto.JFJochProtoBuf.CompressionSettings.prototype.setBlockSize = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ImageSavingSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ImageSavingSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ImageSavingSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    filePrefix: jspb.Message.getFieldWithDefault(msg, 1, ""),
    imagesPerFile: jspb.Message.getFieldWithDefault(msg, 2, 0),
    errorWhenOverwritting: jspb.Message.getBooleanFieldWithDefault(msg, 3, false)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ImageSavingSettings}
 */
proto.JFJochProtoBuf.ImageSavingSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ImageSavingSettings;
  return proto.JFJochProtoBuf.ImageSavingSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ImageSavingSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ImageSavingSettings}
 */
proto.JFJochProtoBuf.ImageSavingSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setFilePrefix(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesPerFile(value);
      break;
    case 3:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setErrorWhenOverwritting(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ImageSavingSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ImageSavingSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ImageSavingSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFilePrefix();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getImagesPerFile();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getErrorWhenOverwritting();
  if (f) {
    writer.writeBool(
      3,
      f
    );
  }
};


/**
 * optional string file_prefix = 1;
 * @return {string}
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.getFilePrefix = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.ImageSavingSettings} returns this
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.setFilePrefix = function(value) {
  return jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * optional int64 images_per_file = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.getImagesPerFile = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ImageSavingSettings} returns this
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.setImagesPerFile = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional bool error_when_overwritting = 3;
 * @return {boolean}
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.getErrorWhenOverwritting = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 3, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.ImageSavingSettings} returns this
 */
proto.JFJochProtoBuf.ImageSavingSettings.prototype.setErrorWhenOverwritting = function(value) {
  return jspb.Message.setProto3BooleanField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    forceFullSpeed: jspb.Message.getBooleanFieldWithDefault(msg, 1, false),
    internalFpgaPacketGenerator: jspb.Message.getBooleanFieldWithDefault(msg, 2, false),
    storageCells: jspb.Message.getFieldWithDefault(msg, 3, 0),
    storageCellStart: jspb.Message.getFieldWithDefault(msg, 4, 0),
    delayAfterTriggerUs: jspb.Message.getFieldWithDefault(msg, 5, 0),
    softTrigger: jspb.Message.getBooleanFieldWithDefault(msg, 6, false)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorSettings}
 */
proto.JFJochProtoBuf.DetectorSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorSettings;
  return proto.JFJochProtoBuf.DetectorSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorSettings}
 */
proto.JFJochProtoBuf.DetectorSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setForceFullSpeed(value);
      break;
    case 2:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setInternalFpgaPacketGenerator(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCells(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCellStart(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setDelayAfterTriggerUs(value);
      break;
    case 6:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setSoftTrigger(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getForceFullSpeed();
  if (f) {
    writer.writeBool(
      1,
      f
    );
  }
  f = message.getInternalFpgaPacketGenerator();
  if (f) {
    writer.writeBool(
      2,
      f
    );
  }
  f = message.getStorageCells();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getStorageCellStart();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getDelayAfterTriggerUs();
  if (f !== 0) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = message.getSoftTrigger();
  if (f) {
    writer.writeBool(
      6,
      f
    );
  }
};


/**
 * optional bool force_full_speed = 1;
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getForceFullSpeed = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 1, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setForceFullSpeed = function(value) {
  return jspb.Message.setProto3BooleanField(this, 1, value);
};


/**
 * optional bool internal_fpga_packet_generator = 2;
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getInternalFpgaPacketGenerator = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 2, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setInternalFpgaPacketGenerator = function(value) {
  return jspb.Message.setProto3BooleanField(this, 2, value);
};


/**
 * optional int64 storage_cells = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getStorageCells = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setStorageCells = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 storage_cell_start = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getStorageCellStart = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setStorageCellStart = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional int64 delay_after_trigger_us = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getDelayAfterTriggerUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setDelayAfterTriggerUs = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional bool soft_trigger = 6;
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.getSoftTrigger = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 6, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.DetectorSettings} returns this
 */
proto.JFJochProtoBuf.DetectorSettings.prototype.setSoftTrigger = function(value) {
  return jspb.Message.setProto3BooleanField(this, 6, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.PreviewSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.PreviewSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PreviewSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    previewPeriodUs: jspb.Message.getFieldWithDefault(msg, 1, 0),
    spotFindingPeriodUs: jspb.Message.getFieldWithDefault(msg, 2, 0),
    bkgEstimationPeriodUs: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.PreviewSettings}
 */
proto.JFJochProtoBuf.PreviewSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.PreviewSettings;
  return proto.JFJochProtoBuf.PreviewSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.PreviewSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.PreviewSettings}
 */
proto.JFJochProtoBuf.PreviewSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPreviewPeriodUs(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpotFindingPeriodUs(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBkgEstimationPeriodUs(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.PreviewSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.PreviewSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PreviewSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getPreviewPeriodUs();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getSpotFindingPeriodUs();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getBkgEstimationPeriodUs();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional int64 preview_period_us = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.getPreviewPeriodUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewSettings} returns this
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.setPreviewPeriodUs = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 spot_finding_period_us = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.getSpotFindingPeriodUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewSettings} returns this
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.setSpotFindingPeriodUs = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 bkg_estimation_period_us = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.getBkgEstimationPeriodUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewSettings} returns this
 */
proto.JFJochProtoBuf.PreviewSettings.prototype.setBkgEstimationPeriodUs = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.PedestalSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.PedestalSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PedestalSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    g0Frames: jspb.Message.getFieldWithDefault(msg, 1, 0),
    g1Frames: jspb.Message.getFieldWithDefault(msg, 2, 0),
    g2Frames: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.PedestalSettings}
 */
proto.JFJochProtoBuf.PedestalSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.PedestalSettings;
  return proto.JFJochProtoBuf.PedestalSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.PedestalSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.PedestalSettings}
 */
proto.JFJochProtoBuf.PedestalSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setG0Frames(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setG1Frames(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setG2Frames(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.PedestalSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.PedestalSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PedestalSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getG0Frames();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getG1Frames();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getG2Frames();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional int64 g0_frames = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.getG0Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PedestalSettings} returns this
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.setG0Frames = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 g1_frames = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.getG1Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PedestalSettings} returns this
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.setG1Frames = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 g2_frames = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.getG2Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PedestalSettings} returns this
 */
proto.JFJochProtoBuf.PedestalSettings.prototype.setG2Frames = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ConversionSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ConversionSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ConversionSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    type: jspb.Message.getFieldWithDefault(msg, 5, 0),
    mode: jspb.Message.getFieldWithDefault(msg, 1, 0),
    upsideDown: jspb.Message.getBooleanFieldWithDefault(msg, 2, false),
    maskModuleEdges: jspb.Message.getBooleanFieldWithDefault(msg, 3, false),
    maskChipEdges: jspb.Message.getBooleanFieldWithDefault(msg, 4, false)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ConversionSettings}
 */
proto.JFJochProtoBuf.ConversionSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ConversionSettings;
  return proto.JFJochProtoBuf.ConversionSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ConversionSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ConversionSettings}
 */
proto.JFJochProtoBuf.ConversionSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 5:
      var value = /** @type {!proto.JFJochProtoBuf.DetectorType} */ (reader.readEnum());
      msg.setType(value);
      break;
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.DetectorMode} */ (reader.readEnum());
      msg.setMode(value);
      break;
    case 2:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setUpsideDown(value);
      break;
    case 3:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setMaskModuleEdges(value);
      break;
    case 4:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setMaskChipEdges(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ConversionSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ConversionSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ConversionSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getType();
  if (f !== 0.0) {
    writer.writeEnum(
      5,
      f
    );
  }
  f = message.getMode();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getUpsideDown();
  if (f) {
    writer.writeBool(
      2,
      f
    );
  }
  f = message.getMaskModuleEdges();
  if (f) {
    writer.writeBool(
      3,
      f
    );
  }
  f = message.getMaskChipEdges();
  if (f) {
    writer.writeBool(
      4,
      f
    );
  }
};


/**
 * optional DetectorType type = 5;
 * @return {!proto.JFJochProtoBuf.DetectorType}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.getType = function() {
  return /** @type {!proto.JFJochProtoBuf.DetectorType} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorType} value
 * @return {!proto.JFJochProtoBuf.ConversionSettings} returns this
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.setType = function(value) {
  return jspb.Message.setProto3EnumField(this, 5, value);
};


/**
 * optional DetectorMode mode = 1;
 * @return {!proto.JFJochProtoBuf.DetectorMode}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.getMode = function() {
  return /** @type {!proto.JFJochProtoBuf.DetectorMode} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorMode} value
 * @return {!proto.JFJochProtoBuf.ConversionSettings} returns this
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.setMode = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional bool upside_down = 2;
 * @return {boolean}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.getUpsideDown = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 2, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.ConversionSettings} returns this
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.setUpsideDown = function(value) {
  return jspb.Message.setProto3BooleanField(this, 2, value);
};


/**
 * optional bool mask_module_edges = 3;
 * @return {boolean}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.getMaskModuleEdges = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 3, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.ConversionSettings} returns this
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.setMaskModuleEdges = function(value) {
  return jspb.Message.setProto3BooleanField(this, 3, value);
};


/**
 * optional bool mask_chip_edges = 4;
 * @return {boolean}
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.getMaskChipEdges = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 4, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.ConversionSettings} returns this
 */
proto.JFJochProtoBuf.ConversionSettings.prototype.setMaskChipEdges = function(value) {
  return jspb.Message.setProto3BooleanField(this, 4, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.JungfraujochInternalSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.JungfraujochInternalSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    baseIpv4Address: jspb.Message.getFieldWithDefault(msg, 6, 0),
    baseUdpPort: jspb.Message.getFieldWithDefault(msg, 7, 0),
    gitSha1: jspb.Message.getFieldWithDefault(msg, 8, ""),
    gitDate: jspb.Message.getFieldWithDefault(msg, 9, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.JungfraujochInternalSettings;
  return proto.JFJochProtoBuf.JungfraujochInternalSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.JungfraujochInternalSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBaseIpv4Address(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBaseUdpPort(value);
      break;
    case 8:
      var value = /** @type {string} */ (reader.readString());
      msg.setGitSha1(value);
      break;
    case 9:
      var value = /** @type {string} */ (reader.readString());
      msg.setGitDate(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.JungfraujochInternalSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.JungfraujochInternalSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getBaseIpv4Address();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
  f = message.getBaseUdpPort();
  if (f !== 0) {
    writer.writeInt64(
      7,
      f
    );
  }
  f = message.getGitSha1();
  if (f.length > 0) {
    writer.writeString(
      8,
      f
    );
  }
  f = message.getGitDate();
  if (f.length > 0) {
    writer.writeString(
      9,
      f
    );
  }
};


/**
 * optional int64 base_ipv4_address = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.getBaseIpv4Address = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.setBaseIpv4Address = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional int64 base_udp_port = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.getBaseUdpPort = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.setBaseUdpPort = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional string git_sha1 = 8;
 * @return {string}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.getGitSha1 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 8, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.setGitSha1 = function(value) {
  return jspb.Message.setProto3StringField(this, 8, value);
};


/**
 * optional string git_date = 9;
 * @return {string}
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.getGitDate = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 9, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.JungfraujochInternalSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochInternalSettings.prototype.setGitDate = function(value) {
  return jspb.Message.setProto3StringField(this, 9, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.SampleSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.SampleSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SampleSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    unitCell: (f = msg.getUnitCell()) && proto.JFJochProtoBuf.UnitCell.toObject(includeInstance, f),
    spaceGroupNumber: jspb.Message.getFieldWithDefault(msg, 3, 0),
    runNumber: jspb.Message.getFieldWithDefault(msg, 4, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.SampleSettings}
 */
proto.JFJochProtoBuf.SampleSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.SampleSettings;
  return proto.JFJochProtoBuf.SampleSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.SampleSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.SampleSettings}
 */
proto.JFJochProtoBuf.SampleSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 2:
      var value = new proto.JFJochProtoBuf.UnitCell;
      reader.readMessage(value,proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader);
      msg.setUnitCell(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpaceGroupNumber(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setRunNumber(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.SampleSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.SampleSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SampleSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getUnitCell();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter
    );
  }
  f = message.getSpaceGroupNumber();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getRunNumber();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
};


/**
 * optional UnitCell unit_cell = 2;
 * @return {?proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.getUnitCell = function() {
  return /** @type{?proto.JFJochProtoBuf.UnitCell} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.UnitCell, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.UnitCell|undefined} value
 * @return {!proto.JFJochProtoBuf.SampleSettings} returns this
*/
proto.JFJochProtoBuf.SampleSettings.prototype.setUnitCell = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.SampleSettings} returns this
 */
proto.JFJochProtoBuf.SampleSettings.prototype.clearUnitCell = function() {
  return this.setUnitCell(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.hasUnitCell = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional int64 space_group_number = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.getSpaceGroupNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.SampleSettings} returns this
 */
proto.JFJochProtoBuf.SampleSettings.prototype.setSpaceGroupNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 run_number = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.SampleSettings.prototype.getRunNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.SampleSettings} returns this
 */
proto.JFJochProtoBuf.SampleSettings.prototype.setRunNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.RadialIntegrationSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.RadialIntegrationSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    lowQ: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    highQ: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    qSpacing: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    bkgEstimateLowQ: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    bkgEstimateHighQ: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.RadialIntegrationSettings;
  return proto.JFJochProtoBuf.RadialIntegrationSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.RadialIntegrationSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setLowQ(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setHighQ(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setQSpacing(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBkgEstimateLowQ(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBkgEstimateHighQ(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.RadialIntegrationSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.RadialIntegrationSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getLowQ();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getHighQ();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getQSpacing();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getBkgEstimateLowQ();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getBkgEstimateHighQ();
  if (f !== 0.0) {
    writer.writeDouble(
      5,
      f
    );
  }
};


/**
 * optional double low_q = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.getLowQ = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings} returns this
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.setLowQ = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double high_q = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.getHighQ = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings} returns this
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.setHighQ = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double q_spacing = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.getQSpacing = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings} returns this
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.setQSpacing = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional double bkg_estimate_low_q = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.getBkgEstimateLowQ = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings} returns this
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.setBkgEstimateLowQ = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional double bkg_estimate_high_q = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.getBkgEstimateHighQ = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.RadialIntegrationSettings} returns this
 */
proto.JFJochProtoBuf.RadialIntegrationSettings.prototype.setBkgEstimateHighQ = function(value) {
  return jspb.Message.setProto3FloatField(this, 5, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.JungfraujochSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.JungfraujochSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JungfraujochSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    frameCount: (f = msg.getFrameCount()) && proto.JFJochProtoBuf.FrameCountSettings.toObject(includeInstance, f),
    timing: (f = msg.getTiming()) && proto.JFJochProtoBuf.TimingSettings.toObject(includeInstance, f),
    diffractionGeometry: (f = msg.getDiffractionGeometry()) && proto.JFJochProtoBuf.DiffractionGeometrySettings.toObject(includeInstance, f),
    beamlineMetadata: (f = msg.getBeamlineMetadata()) && proto.JFJochProtoBuf.BeamlineMetadataSettings.toObject(includeInstance, f),
    imageSaving: (f = msg.getImageSaving()) && proto.JFJochProtoBuf.ImageSavingSettings.toObject(includeInstance, f),
    detector: (f = msg.getDetector()) && proto.JFJochProtoBuf.DetectorSettings.toObject(includeInstance, f),
    preview: (f = msg.getPreview()) && proto.JFJochProtoBuf.PreviewSettings.toObject(includeInstance, f),
    pedestal: (f = msg.getPedestal()) && proto.JFJochProtoBuf.PedestalSettings.toObject(includeInstance, f),
    conv: (f = msg.getConv()) && proto.JFJochProtoBuf.ConversionSettings.toObject(includeInstance, f),
    compr: (f = msg.getCompr()) && proto.JFJochProtoBuf.CompressionSettings.toObject(includeInstance, f),
    internal: (f = msg.getInternal()) && proto.JFJochProtoBuf.JungfraujochInternalSettings.toObject(includeInstance, f),
    sample: (f = msg.getSample()) && proto.JFJochProtoBuf.SampleSettings.toObject(includeInstance, f),
    radialInt: (f = msg.getRadialInt()) && proto.JFJochProtoBuf.RadialIntegrationSettings.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.JungfraujochSettings;
  return proto.JFJochProtoBuf.JungfraujochSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.JungfraujochSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.FrameCountSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.FrameCountSettings.deserializeBinaryFromReader);
      msg.setFrameCount(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.TimingSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.TimingSettings.deserializeBinaryFromReader);
      msg.setTiming(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.DiffractionGeometrySettings;
      reader.readMessage(value,proto.JFJochProtoBuf.DiffractionGeometrySettings.deserializeBinaryFromReader);
      msg.setDiffractionGeometry(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.BeamlineMetadataSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.BeamlineMetadataSettings.deserializeBinaryFromReader);
      msg.setBeamlineMetadata(value);
      break;
    case 5:
      var value = new proto.JFJochProtoBuf.ImageSavingSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.ImageSavingSettings.deserializeBinaryFromReader);
      msg.setImageSaving(value);
      break;
    case 6:
      var value = new proto.JFJochProtoBuf.DetectorSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.DetectorSettings.deserializeBinaryFromReader);
      msg.setDetector(value);
      break;
    case 7:
      var value = new proto.JFJochProtoBuf.PreviewSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.PreviewSettings.deserializeBinaryFromReader);
      msg.setPreview(value);
      break;
    case 8:
      var value = new proto.JFJochProtoBuf.PedestalSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.PedestalSettings.deserializeBinaryFromReader);
      msg.setPedestal(value);
      break;
    case 9:
      var value = new proto.JFJochProtoBuf.ConversionSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.ConversionSettings.deserializeBinaryFromReader);
      msg.setConv(value);
      break;
    case 10:
      var value = new proto.JFJochProtoBuf.CompressionSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.CompressionSettings.deserializeBinaryFromReader);
      msg.setCompr(value);
      break;
    case 11:
      var value = new proto.JFJochProtoBuf.JungfraujochInternalSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.JungfraujochInternalSettings.deserializeBinaryFromReader);
      msg.setInternal(value);
      break;
    case 12:
      var value = new proto.JFJochProtoBuf.SampleSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.SampleSettings.deserializeBinaryFromReader);
      msg.setSample(value);
      break;
    case 13:
      var value = new proto.JFJochProtoBuf.RadialIntegrationSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.RadialIntegrationSettings.deserializeBinaryFromReader);
      msg.setRadialInt(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.JungfraujochSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.JungfraujochSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JungfraujochSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFrameCount();
  if (f != null) {
    writer.writeMessage(
      1,
      f,
      proto.JFJochProtoBuf.FrameCountSettings.serializeBinaryToWriter
    );
  }
  f = message.getTiming();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.TimingSettings.serializeBinaryToWriter
    );
  }
  f = message.getDiffractionGeometry();
  if (f != null) {
    writer.writeMessage(
      3,
      f,
      proto.JFJochProtoBuf.DiffractionGeometrySettings.serializeBinaryToWriter
    );
  }
  f = message.getBeamlineMetadata();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.BeamlineMetadataSettings.serializeBinaryToWriter
    );
  }
  f = message.getImageSaving();
  if (f != null) {
    writer.writeMessage(
      5,
      f,
      proto.JFJochProtoBuf.ImageSavingSettings.serializeBinaryToWriter
    );
  }
  f = message.getDetector();
  if (f != null) {
    writer.writeMessage(
      6,
      f,
      proto.JFJochProtoBuf.DetectorSettings.serializeBinaryToWriter
    );
  }
  f = message.getPreview();
  if (f != null) {
    writer.writeMessage(
      7,
      f,
      proto.JFJochProtoBuf.PreviewSettings.serializeBinaryToWriter
    );
  }
  f = message.getPedestal();
  if (f != null) {
    writer.writeMessage(
      8,
      f,
      proto.JFJochProtoBuf.PedestalSettings.serializeBinaryToWriter
    );
  }
  f = message.getConv();
  if (f != null) {
    writer.writeMessage(
      9,
      f,
      proto.JFJochProtoBuf.ConversionSettings.serializeBinaryToWriter
    );
  }
  f = message.getCompr();
  if (f != null) {
    writer.writeMessage(
      10,
      f,
      proto.JFJochProtoBuf.CompressionSettings.serializeBinaryToWriter
    );
  }
  f = message.getInternal();
  if (f != null) {
    writer.writeMessage(
      11,
      f,
      proto.JFJochProtoBuf.JungfraujochInternalSettings.serializeBinaryToWriter
    );
  }
  f = message.getSample();
  if (f != null) {
    writer.writeMessage(
      12,
      f,
      proto.JFJochProtoBuf.SampleSettings.serializeBinaryToWriter
    );
  }
  f = message.getRadialInt();
  if (f != null) {
    writer.writeMessage(
      13,
      f,
      proto.JFJochProtoBuf.RadialIntegrationSettings.serializeBinaryToWriter
    );
  }
};


/**
 * optional FrameCountSettings frame_count = 1;
 * @return {?proto.JFJochProtoBuf.FrameCountSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getFrameCount = function() {
  return /** @type{?proto.JFJochProtoBuf.FrameCountSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.FrameCountSettings, 1));
};


/**
 * @param {?proto.JFJochProtoBuf.FrameCountSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setFrameCount = function(value) {
  return jspb.Message.setWrapperField(this, 1, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearFrameCount = function() {
  return this.setFrameCount(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasFrameCount = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional TimingSettings timing = 2;
 * @return {?proto.JFJochProtoBuf.TimingSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getTiming = function() {
  return /** @type{?proto.JFJochProtoBuf.TimingSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.TimingSettings, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.TimingSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setTiming = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearTiming = function() {
  return this.setTiming(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasTiming = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional DiffractionGeometrySettings diffraction_geometry = 3;
 * @return {?proto.JFJochProtoBuf.DiffractionGeometrySettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getDiffractionGeometry = function() {
  return /** @type{?proto.JFJochProtoBuf.DiffractionGeometrySettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.DiffractionGeometrySettings, 3));
};


/**
 * @param {?proto.JFJochProtoBuf.DiffractionGeometrySettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setDiffractionGeometry = function(value) {
  return jspb.Message.setWrapperField(this, 3, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearDiffractionGeometry = function() {
  return this.setDiffractionGeometry(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasDiffractionGeometry = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional BeamlineMetadataSettings beamline_metadata = 4;
 * @return {?proto.JFJochProtoBuf.BeamlineMetadataSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getBeamlineMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.BeamlineMetadataSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.BeamlineMetadataSettings, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.BeamlineMetadataSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setBeamlineMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearBeamlineMetadata = function() {
  return this.setBeamlineMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasBeamlineMetadata = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional ImageSavingSettings image_saving = 5;
 * @return {?proto.JFJochProtoBuf.ImageSavingSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getImageSaving = function() {
  return /** @type{?proto.JFJochProtoBuf.ImageSavingSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.ImageSavingSettings, 5));
};


/**
 * @param {?proto.JFJochProtoBuf.ImageSavingSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setImageSaving = function(value) {
  return jspb.Message.setWrapperField(this, 5, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearImageSaving = function() {
  return this.setImageSaving(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasImageSaving = function() {
  return jspb.Message.getField(this, 5) != null;
};


/**
 * optional DetectorSettings detector = 6;
 * @return {?proto.JFJochProtoBuf.DetectorSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getDetector = function() {
  return /** @type{?proto.JFJochProtoBuf.DetectorSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.DetectorSettings, 6));
};


/**
 * @param {?proto.JFJochProtoBuf.DetectorSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setDetector = function(value) {
  return jspb.Message.setWrapperField(this, 6, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearDetector = function() {
  return this.setDetector(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasDetector = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * optional PreviewSettings preview = 7;
 * @return {?proto.JFJochProtoBuf.PreviewSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getPreview = function() {
  return /** @type{?proto.JFJochProtoBuf.PreviewSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.PreviewSettings, 7));
};


/**
 * @param {?proto.JFJochProtoBuf.PreviewSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setPreview = function(value) {
  return jspb.Message.setWrapperField(this, 7, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearPreview = function() {
  return this.setPreview(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasPreview = function() {
  return jspb.Message.getField(this, 7) != null;
};


/**
 * optional PedestalSettings pedestal = 8;
 * @return {?proto.JFJochProtoBuf.PedestalSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getPedestal = function() {
  return /** @type{?proto.JFJochProtoBuf.PedestalSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.PedestalSettings, 8));
};


/**
 * @param {?proto.JFJochProtoBuf.PedestalSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setPedestal = function(value) {
  return jspb.Message.setWrapperField(this, 8, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearPedestal = function() {
  return this.setPedestal(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasPedestal = function() {
  return jspb.Message.getField(this, 8) != null;
};


/**
 * optional ConversionSettings conv = 9;
 * @return {?proto.JFJochProtoBuf.ConversionSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getConv = function() {
  return /** @type{?proto.JFJochProtoBuf.ConversionSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.ConversionSettings, 9));
};


/**
 * @param {?proto.JFJochProtoBuf.ConversionSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setConv = function(value) {
  return jspb.Message.setWrapperField(this, 9, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearConv = function() {
  return this.setConv(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasConv = function() {
  return jspb.Message.getField(this, 9) != null;
};


/**
 * optional CompressionSettings compr = 10;
 * @return {?proto.JFJochProtoBuf.CompressionSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getCompr = function() {
  return /** @type{?proto.JFJochProtoBuf.CompressionSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.CompressionSettings, 10));
};


/**
 * @param {?proto.JFJochProtoBuf.CompressionSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setCompr = function(value) {
  return jspb.Message.setWrapperField(this, 10, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearCompr = function() {
  return this.setCompr(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasCompr = function() {
  return jspb.Message.getField(this, 10) != null;
};


/**
 * optional JungfraujochInternalSettings internal = 11;
 * @return {?proto.JFJochProtoBuf.JungfraujochInternalSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getInternal = function() {
  return /** @type{?proto.JFJochProtoBuf.JungfraujochInternalSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.JungfraujochInternalSettings, 11));
};


/**
 * @param {?proto.JFJochProtoBuf.JungfraujochInternalSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setInternal = function(value) {
  return jspb.Message.setWrapperField(this, 11, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearInternal = function() {
  return this.setInternal(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasInternal = function() {
  return jspb.Message.getField(this, 11) != null;
};


/**
 * optional SampleSettings sample = 12;
 * @return {?proto.JFJochProtoBuf.SampleSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getSample = function() {
  return /** @type{?proto.JFJochProtoBuf.SampleSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.SampleSettings, 12));
};


/**
 * @param {?proto.JFJochProtoBuf.SampleSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setSample = function(value) {
  return jspb.Message.setWrapperField(this, 12, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearSample = function() {
  return this.setSample(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasSample = function() {
  return jspb.Message.getField(this, 12) != null;
};


/**
 * optional RadialIntegrationSettings radial_int = 13;
 * @return {?proto.JFJochProtoBuf.RadialIntegrationSettings}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.getRadialInt = function() {
  return /** @type{?proto.JFJochProtoBuf.RadialIntegrationSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.RadialIntegrationSettings, 13));
};


/**
 * @param {?proto.JFJochProtoBuf.RadialIntegrationSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
*/
proto.JFJochProtoBuf.JungfraujochSettings.prototype.setRadialInt = function(value) {
  return jspb.Message.setWrapperField(this, 13, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.JungfraujochSettings} returns this
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.clearRadialInt = function() {
  return this.setRadialInt(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.JungfraujochSettings.prototype.hasRadialInt = function() {
  return jspb.Message.getField(this, 13) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.JFPedestal.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.JFPedestal} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFPedestal.toObject = function(includeInstance, msg) {
  var f, obj = {
    pedestal: msg.getPedestal_asB64(),
    rms: msg.getRms_asB64(),
    mask: msg.getMask_asB64(),
    timeUnix: jspb.Message.getFieldWithDefault(msg, 4, 0),
    frames: jspb.Message.getFieldWithDefault(msg, 5, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.JFPedestal}
 */
proto.JFJochProtoBuf.JFPedestal.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.JFPedestal;
  return proto.JFJochProtoBuf.JFPedestal.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.JFPedestal} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.JFPedestal}
 */
proto.JFJochProtoBuf.JFPedestal.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setPedestal(value);
      break;
    case 2:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setRms(value);
      break;
    case 3:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setMask(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setTimeUnix(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setFrames(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.JFPedestal.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.JFPedestal} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFPedestal.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getPedestal_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      1,
      f
    );
  }
  f = message.getRms_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      2,
      f
    );
  }
  f = message.getMask_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      3,
      f
    );
  }
  f = message.getTimeUnix();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getFrames();
  if (f !== 0) {
    writer.writeUint64(
      5,
      f
    );
  }
};


/**
 * optional bytes pedestal = 1;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getPedestal = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * optional bytes pedestal = 1;
 * This is a type-conversion wrapper around `getPedestal()`
 * @return {string}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getPedestal_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getPedestal()));
};


/**
 * optional bytes pedestal = 1;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getPedestal()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getPedestal_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getPedestal()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.JFPedestal} returns this
 */
proto.JFJochProtoBuf.JFPedestal.prototype.setPedestal = function(value) {
  return jspb.Message.setProto3BytesField(this, 1, value);
};


/**
 * optional bytes rms = 2;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getRms = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 2, ""));
};


/**
 * optional bytes rms = 2;
 * This is a type-conversion wrapper around `getRms()`
 * @return {string}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getRms_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getRms()));
};


/**
 * optional bytes rms = 2;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getRms()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getRms_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getRms()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.JFPedestal} returns this
 */
proto.JFJochProtoBuf.JFPedestal.prototype.setRms = function(value) {
  return jspb.Message.setProto3BytesField(this, 2, value);
};


/**
 * optional bytes mask = 3;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getMask = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 3, ""));
};


/**
 * optional bytes mask = 3;
 * This is a type-conversion wrapper around `getMask()`
 * @return {string}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getMask_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getMask()));
};


/**
 * optional bytes mask = 3;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getMask()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getMask_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getMask()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.JFPedestal} returns this
 */
proto.JFJochProtoBuf.JFPedestal.prototype.setMask = function(value) {
  return jspb.Message.setProto3BytesField(this, 3, value);
};


/**
 * optional int64 time_unix = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getTimeUnix = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JFPedestal} returns this
 */
proto.JFJochProtoBuf.JFPedestal.prototype.setTimeUnix = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional uint64 frames = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.JFPedestal.prototype.getFrames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JFPedestal} returns this
 */
proto.JFJochProtoBuf.JFPedestal.prototype.setFrames = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.JFCalibration.repeatedFields_ = [3];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.JFCalibration.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.JFCalibration} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFCalibration.toObject = function(includeInstance, msg) {
  var f, obj = {
    nmodules: jspb.Message.getFieldWithDefault(msg, 1, 0),
    nstorageCells: jspb.Message.getFieldWithDefault(msg, 2, 0),
    pedestalList: jspb.Message.toObjectList(msg.getPedestalList(),
    proto.JFJochProtoBuf.JFPedestal.toObject, includeInstance),
    mask: msg.getMask_asB64()
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.JFCalibration}
 */
proto.JFJochProtoBuf.JFCalibration.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.JFCalibration;
  return proto.JFJochProtoBuf.JFCalibration.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.JFCalibration} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.JFCalibration}
 */
proto.JFJochProtoBuf.JFCalibration.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setNmodules(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setNstorageCells(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.JFPedestal;
      reader.readMessage(value,proto.JFJochProtoBuf.JFPedestal.deserializeBinaryFromReader);
      msg.addPedestal(value);
      break;
    case 4:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setMask(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.JFCalibration.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.JFCalibration} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFCalibration.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getNmodules();
  if (f !== 0) {
    writer.writeUint64(
      1,
      f
    );
  }
  f = message.getNstorageCells();
  if (f !== 0) {
    writer.writeUint64(
      2,
      f
    );
  }
  f = message.getPedestalList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      3,
      f,
      proto.JFJochProtoBuf.JFPedestal.serializeBinaryToWriter
    );
  }
  f = message.getMask_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      4,
      f
    );
  }
};


/**
 * optional uint64 nmodules = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getNmodules = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JFCalibration} returns this
 */
proto.JFJochProtoBuf.JFCalibration.prototype.setNmodules = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional uint64 nstorage_cells = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getNstorageCells = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.JFCalibration} returns this
 */
proto.JFJochProtoBuf.JFCalibration.prototype.setNstorageCells = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * repeated JFPedestal pedestal = 3;
 * @return {!Array<!proto.JFJochProtoBuf.JFPedestal>}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getPedestalList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.JFPedestal>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.JFPedestal, 3));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.JFPedestal>} value
 * @return {!proto.JFJochProtoBuf.JFCalibration} returns this
*/
proto.JFJochProtoBuf.JFCalibration.prototype.setPedestalList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 3, value);
};


/**
 * @param {!proto.JFJochProtoBuf.JFPedestal=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.JFPedestal}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.addPedestal = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 3, opt_value, proto.JFJochProtoBuf.JFPedestal, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.JFCalibration} returns this
 */
proto.JFJochProtoBuf.JFCalibration.prototype.clearPedestalList = function() {
  return this.setPedestalList([]);
};


/**
 * optional bytes mask = 4;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getMask = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 4, ""));
};


/**
 * optional bytes mask = 4;
 * This is a type-conversion wrapper around `getMask()`
 * @return {string}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getMask_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getMask()));
};


/**
 * optional bytes mask = 4;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getMask()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFCalibration.prototype.getMask_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getMask()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.JFCalibration} returns this
 */
proto.JFJochProtoBuf.JFCalibration.prototype.setMask = function(value) {
  return jspb.Message.setProto3BytesField(this, 4, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.repeatedFields_ = [1];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.JFCalibrationStatistics.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.JFCalibrationStatistics} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.toObject = function(includeInstance, msg) {
  var f, obj = {
    moduleStatisticsList: jspb.Message.toObjectList(msg.getModuleStatisticsList(),
    proto.JFJochProtoBuf.ModuleStatistics.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.JFCalibrationStatistics}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.JFCalibrationStatistics;
  return proto.JFJochProtoBuf.JFCalibrationStatistics.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.JFCalibrationStatistics} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.JFCalibrationStatistics}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.ModuleStatistics;
      reader.readMessage(value,proto.JFJochProtoBuf.ModuleStatistics.deserializeBinaryFromReader);
      msg.addModuleStatistics(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.JFCalibrationStatistics.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.JFCalibrationStatistics} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getModuleStatisticsList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      1,
      f,
      proto.JFJochProtoBuf.ModuleStatistics.serializeBinaryToWriter
    );
  }
};


/**
 * repeated ModuleStatistics module_statistics = 1;
 * @return {!Array<!proto.JFJochProtoBuf.ModuleStatistics>}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.getModuleStatisticsList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.ModuleStatistics>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.ModuleStatistics, 1));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.ModuleStatistics>} value
 * @return {!proto.JFJochProtoBuf.JFCalibrationStatistics} returns this
*/
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.setModuleStatisticsList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 1, value);
};


/**
 * @param {!proto.JFJochProtoBuf.ModuleStatistics=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.ModuleStatistics}
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.addModuleStatistics = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 1, opt_value, proto.JFJochProtoBuf.ModuleStatistics, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.JFCalibrationStatistics} returns this
 */
proto.JFJochProtoBuf.JFCalibrationStatistics.prototype.clearModuleStatisticsList = function() {
  return this.setModuleStatisticsList([]);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.repeatedFields_ = [3,10,13,14,15];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.AcquisitionDeviceStatistics.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.toObject = function(includeInstance, msg) {
  var f, obj = {
    goodPackets: jspb.Message.getFieldWithDefault(msg, 1, 0),
    packetsExpectedPerModule: jspb.Message.getFieldWithDefault(msg, 2, 0),
    packetsReceivedPerModuleList: (f = jspb.Message.getRepeatedField(msg, 3)) == null ? undefined : f,
    efficiency: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    bytesReceived: jspb.Message.getFieldWithDefault(msg, 5, 0),
    startTimestamp: jspb.Message.getFieldWithDefault(msg, 6, 0),
    endTimestamp: jspb.Message.getFieldWithDefault(msg, 7, 0),
    fpgaStatus: (f = msg.getFpgaStatus()) && proto.JFJochProtoBuf.FPGAStatus.toObject(includeInstance, f),
    nmodules: jspb.Message.getFieldWithDefault(msg, 9, 0),
    packetMaskList: (f = jspb.Message.getRepeatedField(msg, 10)) == null ? undefined : f,
    maskEntriesPerModule: jspb.Message.getFieldWithDefault(msg, 11, 0),
    packetsExpected: jspb.Message.getFieldWithDefault(msg, 12, 0),
    timestampList: (f = jspb.Message.getRepeatedField(msg, 13)) == null ? undefined : f,
    detectorDebugList: (f = jspb.Message.getRepeatedField(msg, 14)) == null ? undefined : f,
    bunchidList: (f = jspb.Message.getRepeatedField(msg, 15)) == null ? undefined : f
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.AcquisitionDeviceStatistics;
  return proto.JFJochProtoBuf.AcquisitionDeviceStatistics.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setGoodPackets(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsExpectedPerModule(value);
      break;
    case 3:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint64() : [reader.readUint64()]);
      for (var i = 0; i < values.length; i++) {
        msg.addPacketsReceivedPerModule(values[i]);
      }
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setEfficiency(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setBytesReceived(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setStartTimestamp(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setEndTimestamp(value);
      break;
    case 8:
      var value = new proto.JFJochProtoBuf.FPGAStatus;
      reader.readMessage(value,proto.JFJochProtoBuf.FPGAStatus.deserializeBinaryFromReader);
      msg.setFpgaStatus(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setNmodules(value);
      break;
    case 10:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint64() : [reader.readUint64()]);
      for (var i = 0; i < values.length; i++) {
        msg.addPacketMask(values[i]);
      }
      break;
    case 11:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaskEntriesPerModule(value);
      break;
    case 12:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsExpected(value);
      break;
    case 13:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint32() : [reader.readUint32()]);
      for (var i = 0; i < values.length; i++) {
        msg.addTimestamp(values[i]);
      }
      break;
    case 14:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint32() : [reader.readUint32()]);
      for (var i = 0; i < values.length; i++) {
        msg.addDetectorDebug(values[i]);
      }
      break;
    case 15:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint64() : [reader.readUint64()]);
      for (var i = 0; i < values.length; i++) {
        msg.addBunchid(values[i]);
      }
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.AcquisitionDeviceStatistics.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getGoodPackets();
  if (f !== 0) {
    writer.writeUint64(
      1,
      f
    );
  }
  f = message.getPacketsExpectedPerModule();
  if (f !== 0) {
    writer.writeUint64(
      2,
      f
    );
  }
  f = message.getPacketsReceivedPerModuleList();
  if (f.length > 0) {
    writer.writePackedUint64(
      3,
      f
    );
  }
  f = message.getEfficiency();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getBytesReceived();
  if (f !== 0) {
    writer.writeUint64(
      5,
      f
    );
  }
  f = message.getStartTimestamp();
  if (f !== 0) {
    writer.writeUint64(
      6,
      f
    );
  }
  f = message.getEndTimestamp();
  if (f !== 0) {
    writer.writeUint64(
      7,
      f
    );
  }
  f = message.getFpgaStatus();
  if (f != null) {
    writer.writeMessage(
      8,
      f,
      proto.JFJochProtoBuf.FPGAStatus.serializeBinaryToWriter
    );
  }
  f = message.getNmodules();
  if (f !== 0) {
    writer.writeUint64(
      9,
      f
    );
  }
  f = message.getPacketMaskList();
  if (f.length > 0) {
    writer.writePackedUint64(
      10,
      f
    );
  }
  f = message.getMaskEntriesPerModule();
  if (f !== 0) {
    writer.writeUint64(
      11,
      f
    );
  }
  f = message.getPacketsExpected();
  if (f !== 0) {
    writer.writeUint64(
      12,
      f
    );
  }
  f = message.getTimestampList();
  if (f.length > 0) {
    writer.writePackedUint32(
      13,
      f
    );
  }
  f = message.getDetectorDebugList();
  if (f.length > 0) {
    writer.writePackedUint32(
      14,
      f
    );
  }
  f = message.getBunchidList();
  if (f.length > 0) {
    writer.writePackedUint64(
      15,
      f
    );
  }
};


/**
 * optional uint64 good_packets = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getGoodPackets = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setGoodPackets = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional uint64 packets_expected_per_module = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getPacketsExpectedPerModule = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setPacketsExpectedPerModule = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * repeated uint64 packets_received_per_module = 3;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getPacketsReceivedPerModuleList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 3));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setPacketsReceivedPerModuleList = function(value) {
  return jspb.Message.setField(this, 3, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.addPacketsReceivedPerModule = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 3, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearPacketsReceivedPerModuleList = function() {
  return this.setPacketsReceivedPerModuleList([]);
};


/**
 * optional double efficiency = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getEfficiency = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setEfficiency = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional uint64 bytes_received = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getBytesReceived = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setBytesReceived = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional uint64 start_timestamp = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getStartTimestamp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setStartTimestamp = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional uint64 end_timestamp = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getEndTimestamp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setEndTimestamp = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional FPGAStatus fpga_status = 8;
 * @return {?proto.JFJochProtoBuf.FPGAStatus}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getFpgaStatus = function() {
  return /** @type{?proto.JFJochProtoBuf.FPGAStatus} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.FPGAStatus, 8));
};


/**
 * @param {?proto.JFJochProtoBuf.FPGAStatus|undefined} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
*/
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setFpgaStatus = function(value) {
  return jspb.Message.setWrapperField(this, 8, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearFpgaStatus = function() {
  return this.setFpgaStatus(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.hasFpgaStatus = function() {
  return jspb.Message.getField(this, 8) != null;
};


/**
 * optional uint64 nmodules = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getNmodules = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setNmodules = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};


/**
 * repeated uint64 packet_mask = 10;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getPacketMaskList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 10));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setPacketMaskList = function(value) {
  return jspb.Message.setField(this, 10, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.addPacketMask = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 10, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearPacketMaskList = function() {
  return this.setPacketMaskList([]);
};


/**
 * optional uint64 mask_entries_per_module = 11;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getMaskEntriesPerModule = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 11, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setMaskEntriesPerModule = function(value) {
  return jspb.Message.setProto3IntField(this, 11, value);
};


/**
 * optional uint64 packets_expected = 12;
 * @return {number}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getPacketsExpected = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 12, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setPacketsExpected = function(value) {
  return jspb.Message.setProto3IntField(this, 12, value);
};


/**
 * repeated uint32 timestamp = 13;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getTimestampList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 13));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setTimestampList = function(value) {
  return jspb.Message.setField(this, 13, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.addTimestamp = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 13, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearTimestampList = function() {
  return this.setTimestampList([]);
};


/**
 * repeated uint32 detector_debug = 14;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getDetectorDebugList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 14));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setDetectorDebugList = function(value) {
  return jspb.Message.setField(this, 14, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.addDetectorDebug = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 14, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearDetectorDebugList = function() {
  return this.setDetectorDebugList([]);
};


/**
 * repeated uint64 bunchid = 15;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.getBunchidList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 15));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.setBunchidList = function(value) {
  return jspb.Message.setField(this, 15, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.addBunchid = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 15, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics} returns this
 */
proto.JFJochProtoBuf.AcquisitionDeviceStatistics.prototype.clearBunchidList = function() {
  return this.setBunchidList([]);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.ReceiverInput.repeatedFields_ = [3];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ReceiverInput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ReceiverInput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverInput.toObject = function(includeInstance, msg) {
  var f, obj = {
    jungfraujochSettings: (f = msg.getJungfraujochSettings()) && proto.JFJochProtoBuf.JungfraujochSettings.toObject(includeInstance, f),
    calibration: (f = msg.getCalibration()) && proto.JFJochProtoBuf.JFCalibration.toObject(includeInstance, f),
    writerZmqAddressList: (f = jspb.Message.getRepeatedField(msg, 3)) == null ? undefined : f
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ReceiverInput}
 */
proto.JFJochProtoBuf.ReceiverInput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ReceiverInput;
  return proto.JFJochProtoBuf.ReceiverInput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ReceiverInput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ReceiverInput}
 */
proto.JFJochProtoBuf.ReceiverInput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.JungfraujochSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.JungfraujochSettings.deserializeBinaryFromReader);
      msg.setJungfraujochSettings(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.JFCalibration;
      reader.readMessage(value,proto.JFJochProtoBuf.JFCalibration.deserializeBinaryFromReader);
      msg.setCalibration(value);
      break;
    case 3:
      var value = /** @type {string} */ (reader.readString());
      msg.addWriterZmqAddress(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ReceiverInput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ReceiverInput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverInput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getJungfraujochSettings();
  if (f != null) {
    writer.writeMessage(
      1,
      f,
      proto.JFJochProtoBuf.JungfraujochSettings.serializeBinaryToWriter
    );
  }
  f = message.getCalibration();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.JFCalibration.serializeBinaryToWriter
    );
  }
  f = message.getWriterZmqAddressList();
  if (f.length > 0) {
    writer.writeRepeatedString(
      3,
      f
    );
  }
};


/**
 * optional JungfraujochSettings jungfraujoch_settings = 1;
 * @return {?proto.JFJochProtoBuf.JungfraujochSettings}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.getJungfraujochSettings = function() {
  return /** @type{?proto.JFJochProtoBuf.JungfraujochSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.JungfraujochSettings, 1));
};


/**
 * @param {?proto.JFJochProtoBuf.JungfraujochSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
*/
proto.JFJochProtoBuf.ReceiverInput.prototype.setJungfraujochSettings = function(value) {
  return jspb.Message.setWrapperField(this, 1, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.clearJungfraujochSettings = function() {
  return this.setJungfraujochSettings(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.hasJungfraujochSettings = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional JFCalibration calibration = 2;
 * @return {?proto.JFJochProtoBuf.JFCalibration}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.getCalibration = function() {
  return /** @type{?proto.JFJochProtoBuf.JFCalibration} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.JFCalibration, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.JFCalibration|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
*/
proto.JFJochProtoBuf.ReceiverInput.prototype.setCalibration = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.clearCalibration = function() {
  return this.setCalibration(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.hasCalibration = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * repeated string writer_zmq_address = 3;
 * @return {!Array<string>}
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.getWriterZmqAddressList = function() {
  return /** @type {!Array<string>} */ (jspb.Message.getRepeatedField(this, 3));
};


/**
 * @param {!Array<string>} value
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.setWriterZmqAddressList = function(value) {
  return jspb.Message.setField(this, 3, value || []);
};


/**
 * @param {string} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.addWriterZmqAddress = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 3, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.ReceiverInput} returns this
 */
proto.JFJochProtoBuf.ReceiverInput.prototype.clearWriterZmqAddressList = function() {
  return this.setWriterZmqAddressList([]);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.ReceiverOutput.repeatedFields_ = [1,17];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ReceiverOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ReceiverOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverOutput.toObject = function(includeInstance, msg) {
  var f, obj = {
    deviceStatisticsList: jspb.Message.toObjectList(msg.getDeviceStatisticsList(),
    proto.JFJochProtoBuf.AcquisitionDeviceStatistics.toObject, includeInstance),
    maxReceiveDelay: jspb.Message.getFieldWithDefault(msg, 2, 0),
    compressedSize: jspb.Message.getFieldWithDefault(msg, 3, 0),
    compressedRatio: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    imagesSent: jspb.Message.getFieldWithDefault(msg, 5, 0),
    startTimeMs: jspb.Message.getFieldWithDefault(msg, 6, 0),
    endTimeMs: jspb.Message.getFieldWithDefault(msg, 7, 0),
    efficiency: jspb.Message.getFloatingPointFieldWithDefault(msg, 9, 0.0),
    maxImageNumberSent: jspb.Message.getFieldWithDefault(msg, 10, 0),
    jungfraujochSettings: (f = msg.getJungfraujochSettings()) && proto.JFJochProtoBuf.JungfraujochSettings.toObject(includeInstance, f),
    indexerOutput: (f = msg.getIndexerOutput()) && proto.JFJochProtoBuf.IndexerOutput.toObject(includeInstance, f),
    pedestalResultList: jspb.Message.toObjectList(msg.getPedestalResultList(),
    proto.JFJochProtoBuf.JFPedestal.toObject, includeInstance),
    masterFileName: jspb.Message.getFieldWithDefault(msg, 18, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput}
 */
proto.JFJochProtoBuf.ReceiverOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ReceiverOutput;
  return proto.JFJochProtoBuf.ReceiverOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ReceiverOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput}
 */
proto.JFJochProtoBuf.ReceiverOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.AcquisitionDeviceStatistics;
      reader.readMessage(value,proto.JFJochProtoBuf.AcquisitionDeviceStatistics.deserializeBinaryFromReader);
      msg.addDeviceStatistics(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaxReceiveDelay(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setCompressedSize(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCompressedRatio(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setImagesSent(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setStartTimeMs(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setEndTimeMs(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setEfficiency(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaxImageNumberSent(value);
      break;
    case 15:
      var value = new proto.JFJochProtoBuf.JungfraujochSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.JungfraujochSettings.deserializeBinaryFromReader);
      msg.setJungfraujochSettings(value);
      break;
    case 16:
      var value = new proto.JFJochProtoBuf.IndexerOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.IndexerOutput.deserializeBinaryFromReader);
      msg.setIndexerOutput(value);
      break;
    case 17:
      var value = new proto.JFJochProtoBuf.JFPedestal;
      reader.readMessage(value,proto.JFJochProtoBuf.JFPedestal.deserializeBinaryFromReader);
      msg.addPedestalResult(value);
      break;
    case 18:
      var value = /** @type {string} */ (reader.readString());
      msg.setMasterFileName(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ReceiverOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ReceiverOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getDeviceStatisticsList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      1,
      f,
      proto.JFJochProtoBuf.AcquisitionDeviceStatistics.serializeBinaryToWriter
    );
  }
  f = message.getMaxReceiveDelay();
  if (f !== 0) {
    writer.writeUint64(
      2,
      f
    );
  }
  f = message.getCompressedSize();
  if (f !== 0) {
    writer.writeUint64(
      3,
      f
    );
  }
  f = message.getCompressedRatio();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getImagesSent();
  if (f !== 0) {
    writer.writeUint64(
      5,
      f
    );
  }
  f = message.getStartTimeMs();
  if (f !== 0) {
    writer.writeUint64(
      6,
      f
    );
  }
  f = message.getEndTimeMs();
  if (f !== 0) {
    writer.writeUint64(
      7,
      f
    );
  }
  f = message.getEfficiency();
  if (f !== 0.0) {
    writer.writeDouble(
      9,
      f
    );
  }
  f = message.getMaxImageNumberSent();
  if (f !== 0) {
    writer.writeUint64(
      10,
      f
    );
  }
  f = message.getJungfraujochSettings();
  if (f != null) {
    writer.writeMessage(
      15,
      f,
      proto.JFJochProtoBuf.JungfraujochSettings.serializeBinaryToWriter
    );
  }
  f = message.getIndexerOutput();
  if (f != null) {
    writer.writeMessage(
      16,
      f,
      proto.JFJochProtoBuf.IndexerOutput.serializeBinaryToWriter
    );
  }
  f = message.getPedestalResultList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      17,
      f,
      proto.JFJochProtoBuf.JFPedestal.serializeBinaryToWriter
    );
  }
  f = message.getMasterFileName();
  if (f.length > 0) {
    writer.writeString(
      18,
      f
    );
  }
};


/**
 * repeated AcquisitionDeviceStatistics device_statistics = 1;
 * @return {!Array<!proto.JFJochProtoBuf.AcquisitionDeviceStatistics>}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getDeviceStatisticsList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.AcquisitionDeviceStatistics>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.AcquisitionDeviceStatistics, 1));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.AcquisitionDeviceStatistics>} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
*/
proto.JFJochProtoBuf.ReceiverOutput.prototype.setDeviceStatisticsList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 1, value);
};


/**
 * @param {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.AcquisitionDeviceStatistics}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.addDeviceStatistics = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 1, opt_value, proto.JFJochProtoBuf.AcquisitionDeviceStatistics, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.clearDeviceStatisticsList = function() {
  return this.setDeviceStatisticsList([]);
};


/**
 * optional uint64 max_receive_delay = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getMaxReceiveDelay = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setMaxReceiveDelay = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional uint64 compressed_size = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getCompressedSize = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setCompressedSize = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional double compressed_ratio = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getCompressedRatio = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setCompressedRatio = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional uint64 images_sent = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getImagesSent = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setImagesSent = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional uint64 start_time_ms = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getStartTimeMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setStartTimeMs = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional uint64 end_time_ms = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getEndTimeMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setEndTimeMs = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional double efficiency = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getEfficiency = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 9, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setEfficiency = function(value) {
  return jspb.Message.setProto3FloatField(this, 9, value);
};


/**
 * optional uint64 max_image_number_sent = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getMaxImageNumberSent = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setMaxImageNumberSent = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional JungfraujochSettings jungfraujoch_settings = 15;
 * @return {?proto.JFJochProtoBuf.JungfraujochSettings}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getJungfraujochSettings = function() {
  return /** @type{?proto.JFJochProtoBuf.JungfraujochSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.JungfraujochSettings, 15));
};


/**
 * @param {?proto.JFJochProtoBuf.JungfraujochSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
*/
proto.JFJochProtoBuf.ReceiverOutput.prototype.setJungfraujochSettings = function(value) {
  return jspb.Message.setWrapperField(this, 15, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.clearJungfraujochSettings = function() {
  return this.setJungfraujochSettings(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.hasJungfraujochSettings = function() {
  return jspb.Message.getField(this, 15) != null;
};


/**
 * optional IndexerOutput indexer_output = 16;
 * @return {?proto.JFJochProtoBuf.IndexerOutput}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getIndexerOutput = function() {
  return /** @type{?proto.JFJochProtoBuf.IndexerOutput} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.IndexerOutput, 16));
};


/**
 * @param {?proto.JFJochProtoBuf.IndexerOutput|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
*/
proto.JFJochProtoBuf.ReceiverOutput.prototype.setIndexerOutput = function(value) {
  return jspb.Message.setWrapperField(this, 16, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.clearIndexerOutput = function() {
  return this.setIndexerOutput(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.hasIndexerOutput = function() {
  return jspb.Message.getField(this, 16) != null;
};


/**
 * repeated JFPedestal pedestal_result = 17;
 * @return {!Array<!proto.JFJochProtoBuf.JFPedestal>}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getPedestalResultList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.JFPedestal>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.JFPedestal, 17));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.JFPedestal>} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
*/
proto.JFJochProtoBuf.ReceiverOutput.prototype.setPedestalResultList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 17, value);
};


/**
 * @param {!proto.JFJochProtoBuf.JFPedestal=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.JFPedestal}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.addPedestalResult = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 17, opt_value, proto.JFJochProtoBuf.JFPedestal, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.clearPedestalResultList = function() {
  return this.setPedestalResultList([]);
};


/**
 * optional string master_file_name = 18;
 * @return {string}
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.getMasterFileName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 18, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.ReceiverOutput} returns this
 */
proto.JFJochProtoBuf.ReceiverOutput.prototype.setMasterFileName = function(value) {
  return jspb.Message.setProto3StringField(this, 18, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.repeatedFields_ = [1];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ReceiverNetworkConfig.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ReceiverNetworkConfig} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.toObject = function(includeInstance, msg) {
  var f, obj = {
    fpgaMacAddrList: (f = jspb.Message.getRepeatedField(msg, 1)) == null ? undefined : f
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ReceiverNetworkConfig}
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ReceiverNetworkConfig;
  return proto.JFJochProtoBuf.ReceiverNetworkConfig.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ReceiverNetworkConfig} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ReceiverNetworkConfig}
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.addFpgaMacAddr(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ReceiverNetworkConfig.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ReceiverNetworkConfig} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFpgaMacAddrList();
  if (f.length > 0) {
    writer.writeRepeatedString(
      1,
      f
    );
  }
};


/**
 * repeated string fpga_mac_addr = 1;
 * @return {!Array<string>}
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.getFpgaMacAddrList = function() {
  return /** @type {!Array<string>} */ (jspb.Message.getRepeatedField(this, 1));
};


/**
 * @param {!Array<string>} value
 * @return {!proto.JFJochProtoBuf.ReceiverNetworkConfig} returns this
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.setFpgaMacAddrList = function(value) {
  return jspb.Message.setField(this, 1, value || []);
};


/**
 * @param {string} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.ReceiverNetworkConfig} returns this
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.addFpgaMacAddr = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 1, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.ReceiverNetworkConfig} returns this
 */
proto.JFJochProtoBuf.ReceiverNetworkConfig.prototype.clearFpgaMacAddrList = function() {
  return this.setFpgaMacAddrList([]);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DataFile.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DataFile.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DataFile} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DataFile.toObject = function(includeInstance, msg) {
  var f, obj = {
    filename: jspb.Message.getFieldWithDefault(msg, 1, ""),
    imageCount: jspb.Message.getFieldWithDefault(msg, 2, 0),
    image0: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DataFile}
 */
proto.JFJochProtoBuf.DataFile.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DataFile;
  return proto.JFJochProtoBuf.DataFile.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DataFile} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DataFile}
 */
proto.JFJochProtoBuf.DataFile.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setFilename(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageCount(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImage0(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DataFile.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DataFile.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DataFile} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DataFile.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFilename();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getImageCount();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getImage0();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional string filename = 1;
 * @return {string}
 */
proto.JFJochProtoBuf.DataFile.prototype.getFilename = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DataFile} returns this
 */
proto.JFJochProtoBuf.DataFile.prototype.setFilename = function(value) {
  return jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * optional int64 image_count = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DataFile.prototype.getImageCount = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataFile} returns this
 */
proto.JFJochProtoBuf.DataFile.prototype.setImageCount = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 image0 = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DataFile.prototype.getImage0 = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataFile} returns this
 */
proto.JFJochProtoBuf.DataFile.prototype.setImage0 = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.WriterInput.repeatedFields_ = [2];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.WriterInput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.WriterInput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.WriterInput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterInput.toObject = function(includeInstance, msg) {
  var f, obj = {
    zmqReceiverAddress: jspb.Message.getFieldWithDefault(msg, 1, ""),
    dataFilesList: jspb.Message.toObjectList(msg.getDataFilesList(),
    proto.JFJochProtoBuf.DataFile.toObject, includeInstance),
    width: jspb.Message.getFieldWithDefault(msg, 3, 0),
    height: jspb.Message.getFieldWithDefault(msg, 4, 0),
    imagesPerFile: jspb.Message.getFieldWithDefault(msg, 5, 0),
    pixelDepthBytes: jspb.Message.getFieldWithDefault(msg, 6, 0),
    signedPxl: jspb.Message.getBooleanFieldWithDefault(msg, 7, false),
    errorWhenOverwriting: jspb.Message.getBooleanFieldWithDefault(msg, 8, false),
    compressionAlgorithm: jspb.Message.getFieldWithDefault(msg, 9, 0),
    compressionBlockSize: jspb.Message.getFieldWithDefault(msg, 10, 0),
    imageUnits: jspb.Message.getFieldWithDefault(msg, 11, ""),
    maxSpotCount: jspb.Message.getFieldWithDefault(msg, 12, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.WriterInput}
 */
proto.JFJochProtoBuf.WriterInput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.WriterInput;
  return proto.JFJochProtoBuf.WriterInput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.WriterInput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.WriterInput}
 */
proto.JFJochProtoBuf.WriterInput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setZmqReceiverAddress(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.DataFile;
      reader.readMessage(value,proto.JFJochProtoBuf.DataFile.deserializeBinaryFromReader);
      msg.addDataFiles(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setWidth(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setHeight(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesPerFile(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPixelDepthBytes(value);
      break;
    case 7:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setSignedPxl(value);
      break;
    case 8:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setErrorWhenOverwriting(value);
      break;
    case 9:
      var value = /** @type {!proto.JFJochProtoBuf.Compression} */ (reader.readEnum());
      msg.setCompressionAlgorithm(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setCompressionBlockSize(value);
      break;
    case 11:
      var value = /** @type {string} */ (reader.readString());
      msg.setImageUnits(value);
      break;
    case 12:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setMaxSpotCount(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterInput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.WriterInput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.WriterInput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterInput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getZmqReceiverAddress();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getDataFilesList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      2,
      f,
      proto.JFJochProtoBuf.DataFile.serializeBinaryToWriter
    );
  }
  f = message.getWidth();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getHeight();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getImagesPerFile();
  if (f !== 0) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = message.getPixelDepthBytes();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
  f = message.getSignedPxl();
  if (f) {
    writer.writeBool(
      7,
      f
    );
  }
  f = message.getErrorWhenOverwriting();
  if (f) {
    writer.writeBool(
      8,
      f
    );
  }
  f = message.getCompressionAlgorithm();
  if (f !== 0.0) {
    writer.writeEnum(
      9,
      f
    );
  }
  f = message.getCompressionBlockSize();
  if (f !== 0) {
    writer.writeInt64(
      10,
      f
    );
  }
  f = message.getImageUnits();
  if (f.length > 0) {
    writer.writeString(
      11,
      f
    );
  }
  f = message.getMaxSpotCount();
  if (f !== 0) {
    writer.writeInt64(
      12,
      f
    );
  }
};


/**
 * optional string zmq_receiver_address = 1;
 * @return {string}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getZmqReceiverAddress = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setZmqReceiverAddress = function(value) {
  return jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * repeated DataFile data_files = 2;
 * @return {!Array<!proto.JFJochProtoBuf.DataFile>}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getDataFilesList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.DataFile>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.DataFile, 2));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.DataFile>} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
*/
proto.JFJochProtoBuf.WriterInput.prototype.setDataFilesList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 2, value);
};


/**
 * @param {!proto.JFJochProtoBuf.DataFile=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.DataFile}
 */
proto.JFJochProtoBuf.WriterInput.prototype.addDataFiles = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 2, opt_value, proto.JFJochProtoBuf.DataFile, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.clearDataFilesList = function() {
  return this.setDataFilesList([]);
};


/**
 * optional int64 width = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getWidth = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setWidth = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 height = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getHeight = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setHeight = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional int64 images_per_file = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getImagesPerFile = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setImagesPerFile = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional int64 pixel_depth_bytes = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getPixelDepthBytes = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setPixelDepthBytes = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional bool signed_pxl = 7;
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getSignedPxl = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 7, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setSignedPxl = function(value) {
  return jspb.Message.setProto3BooleanField(this, 7, value);
};


/**
 * optional bool error_when_overwriting = 8;
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getErrorWhenOverwriting = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 8, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setErrorWhenOverwriting = function(value) {
  return jspb.Message.setProto3BooleanField(this, 8, value);
};


/**
 * optional Compression compression_algorithm = 9;
 * @return {!proto.JFJochProtoBuf.Compression}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getCompressionAlgorithm = function() {
  return /** @type {!proto.JFJochProtoBuf.Compression} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.Compression} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setCompressionAlgorithm = function(value) {
  return jspb.Message.setProto3EnumField(this, 9, value);
};


/**
 * optional int64 compression_block_size = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getCompressionBlockSize = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setCompressionBlockSize = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional string image_units = 11;
 * @return {string}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getImageUnits = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 11, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setImageUnits = function(value) {
  return jspb.Message.setProto3StringField(this, 11, value);
};


/**
 * optional int64 max_spot_count = 12;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterInput.prototype.getMaxSpotCount = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 12, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterInput} returns this
 */
proto.JFJochProtoBuf.WriterInput.prototype.setMaxSpotCount = function(value) {
  return jspb.Message.setProto3IntField(this, 12, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.WriterOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.WriterOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.WriterOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterOutput.toObject = function(includeInstance, msg) {
  var f, obj = {
    nimages: jspb.Message.getFieldWithDefault(msg, 1, 0),
    performanceMbs: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    performanceHz: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.WriterOutput}
 */
proto.JFJochProtoBuf.WriterOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.WriterOutput;
  return proto.JFJochProtoBuf.WriterOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.WriterOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.WriterOutput}
 */
proto.JFJochProtoBuf.WriterOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setNimages(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setPerformanceMbs(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setPerformanceHz(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.WriterOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.WriterOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getNimages();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getPerformanceMbs();
  if (f !== 0.0) {
    writer.writeFloat(
      2,
      f
    );
  }
  f = message.getPerformanceHz();
  if (f !== 0.0) {
    writer.writeFloat(
      3,
      f
    );
  }
};


/**
 * optional int64 nimages = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterOutput.prototype.getNimages = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterOutput} returns this
 */
proto.JFJochProtoBuf.WriterOutput.prototype.setNimages = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional float performance_MBs = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterOutput.prototype.getPerformanceMbs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterOutput} returns this
 */
proto.JFJochProtoBuf.WriterOutput.prototype.setPerformanceMbs = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional float performance_Hz = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterOutput.prototype.getPerformanceHz = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterOutput} returns this
 */
proto.JFJochProtoBuf.WriterOutput.prototype.setPerformanceHz = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.FacilityMetadata.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.FacilityMetadata} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FacilityMetadata.toObject = function(includeInstance, msg) {
  var f, obj = {
    sourceNameShort: jspb.Message.getFieldWithDefault(msg, 10, ""),
    sourceName: jspb.Message.getFieldWithDefault(msg, 11, ""),
    instrumentName: jspb.Message.getFieldWithDefault(msg, 12, ""),
    instrumentNameShort: jspb.Message.getFieldWithDefault(msg, 13, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.FacilityMetadata}
 */
proto.JFJochProtoBuf.FacilityMetadata.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.FacilityMetadata;
  return proto.JFJochProtoBuf.FacilityMetadata.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.FacilityMetadata} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.FacilityMetadata}
 */
proto.JFJochProtoBuf.FacilityMetadata.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 10:
      var value = /** @type {string} */ (reader.readString());
      msg.setSourceNameShort(value);
      break;
    case 11:
      var value = /** @type {string} */ (reader.readString());
      msg.setSourceName(value);
      break;
    case 12:
      var value = /** @type {string} */ (reader.readString());
      msg.setInstrumentName(value);
      break;
    case 13:
      var value = /** @type {string} */ (reader.readString());
      msg.setInstrumentNameShort(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.FacilityMetadata.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.FacilityMetadata} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FacilityMetadata.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getSourceNameShort();
  if (f.length > 0) {
    writer.writeString(
      10,
      f
    );
  }
  f = message.getSourceName();
  if (f.length > 0) {
    writer.writeString(
      11,
      f
    );
  }
  f = message.getInstrumentName();
  if (f.length > 0) {
    writer.writeString(
      12,
      f
    );
  }
  f = message.getInstrumentNameShort();
  if (f.length > 0) {
    writer.writeString(
      13,
      f
    );
  }
};


/**
 * optional string source_name_short = 10;
 * @return {string}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.getSourceNameShort = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 10, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.FacilityMetadata} returns this
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.setSourceNameShort = function(value) {
  return jspb.Message.setProto3StringField(this, 10, value);
};


/**
 * optional string source_name = 11;
 * @return {string}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.getSourceName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 11, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.FacilityMetadata} returns this
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.setSourceName = function(value) {
  return jspb.Message.setProto3StringField(this, 11, value);
};


/**
 * optional string instrument_name = 12;
 * @return {string}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.getInstrumentName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 12, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.FacilityMetadata} returns this
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.setInstrumentName = function(value) {
  return jspb.Message.setProto3StringField(this, 12, value);
};


/**
 * optional string instrument_name_short = 13;
 * @return {string}
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.getInstrumentNameShort = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 13, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.FacilityMetadata} returns this
 */
proto.JFJochProtoBuf.FacilityMetadata.prototype.setInstrumentNameShort = function(value) {
  return jspb.Message.setProto3StringField(this, 13, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ExperimentGeometry.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ExperimentGeometry} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ExperimentGeometry.toObject = function(includeInstance, msg) {
  var f, obj = {
    beamXPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    beamYPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    distanceMm: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ExperimentGeometry}
 */
proto.JFJochProtoBuf.ExperimentGeometry.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ExperimentGeometry;
  return proto.JFJochProtoBuf.ExperimentGeometry.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ExperimentGeometry} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ExperimentGeometry}
 */
proto.JFJochProtoBuf.ExperimentGeometry.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamXPxl(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamYPxl(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setDistanceMm(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ExperimentGeometry.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ExperimentGeometry} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ExperimentGeometry.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getBeamXPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getBeamYPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getDistanceMm();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
};


/**
 * optional double beam_x_pxl = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.getBeamXPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ExperimentGeometry} returns this
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.setBeamXPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double beam_y_pxl = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.getBeamYPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ExperimentGeometry} returns this
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.setBeamYPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double distance_mm = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.getDistanceMm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ExperimentGeometry} returns this
 */
proto.JFJochProtoBuf.ExperimentGeometry.prototype.setDistanceMm = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorMetadata.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorMetadata} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorMetadata.toObject = function(includeInstance, msg) {
  var f, obj = {
    type: jspb.Message.getFieldWithDefault(msg, 1, 0),
    widthPxl: jspb.Message.getFieldWithDefault(msg, 2, 0),
    heightPxl: jspb.Message.getFieldWithDefault(msg, 3, 0),
    pixelDepthByte: jspb.Message.getFieldWithDefault(msg, 4, 0),
    signedPxl: jspb.Message.getBooleanFieldWithDefault(msg, 5, false),
    compression: (f = msg.getCompression()) && proto.JFJochProtoBuf.CompressionSettings.toObject(includeInstance, f),
    imageTimeUs: jspb.Message.getFloatingPointFieldWithDefault(msg, 7, 0.0),
    countTimeUs: jspb.Message.getFloatingPointFieldWithDefault(msg, 8, 0.0),
    geometry: (f = msg.getGeometry()) && proto.JFJochProtoBuf.ExperimentGeometry.toObject(includeInstance, f),
    overload: jspb.Message.getFieldWithDefault(msg, 10, 0),
    underload: jspb.Message.getFieldWithDefault(msg, 11, 0),
    delayAfterTriggerUs: jspb.Message.getFloatingPointFieldWithDefault(msg, 12, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorMetadata}
 */
proto.JFJochProtoBuf.DetectorMetadata.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorMetadata;
  return proto.JFJochProtoBuf.DetectorMetadata.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorMetadata} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorMetadata}
 */
proto.JFJochProtoBuf.DetectorMetadata.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.DetectorType} */ (reader.readEnum());
      msg.setType(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setWidthPxl(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setHeightPxl(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPixelDepthByte(value);
      break;
    case 5:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setSignedPxl(value);
      break;
    case 6:
      var value = new proto.JFJochProtoBuf.CompressionSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.CompressionSettings.deserializeBinaryFromReader);
      msg.setCompression(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setImageTimeUs(value);
      break;
    case 8:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCountTimeUs(value);
      break;
    case 9:
      var value = new proto.JFJochProtoBuf.ExperimentGeometry;
      reader.readMessage(value,proto.JFJochProtoBuf.ExperimentGeometry.deserializeBinaryFromReader);
      msg.setGeometry(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setOverload(value);
      break;
    case 11:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setUnderload(value);
      break;
    case 12:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setDelayAfterTriggerUs(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorMetadata.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorMetadata} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorMetadata.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getType();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getWidthPxl();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getHeightPxl();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getPixelDepthByte();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getSignedPxl();
  if (f) {
    writer.writeBool(
      5,
      f
    );
  }
  f = message.getCompression();
  if (f != null) {
    writer.writeMessage(
      6,
      f,
      proto.JFJochProtoBuf.CompressionSettings.serializeBinaryToWriter
    );
  }
  f = message.getImageTimeUs();
  if (f !== 0.0) {
    writer.writeDouble(
      7,
      f
    );
  }
  f = message.getCountTimeUs();
  if (f !== 0.0) {
    writer.writeDouble(
      8,
      f
    );
  }
  f = message.getGeometry();
  if (f != null) {
    writer.writeMessage(
      9,
      f,
      proto.JFJochProtoBuf.ExperimentGeometry.serializeBinaryToWriter
    );
  }
  f = message.getOverload();
  if (f !== 0) {
    writer.writeInt64(
      10,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 11));
  if (f != null) {
    writer.writeInt64(
      11,
      f
    );
  }
  f = message.getDelayAfterTriggerUs();
  if (f !== 0.0) {
    writer.writeDouble(
      12,
      f
    );
  }
};


/**
 * optional DetectorType type = 1;
 * @return {!proto.JFJochProtoBuf.DetectorType}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getType = function() {
  return /** @type {!proto.JFJochProtoBuf.DetectorType} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorType} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setType = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional int64 width_pxl = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getWidthPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setWidthPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 height_pxl = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getHeightPxl = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setHeightPxl = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 pixel_depth_byte = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getPixelDepthByte = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setPixelDepthByte = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional bool signed_pxl = 5;
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getSignedPxl = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 5, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setSignedPxl = function(value) {
  return jspb.Message.setProto3BooleanField(this, 5, value);
};


/**
 * optional CompressionSettings compression = 6;
 * @return {?proto.JFJochProtoBuf.CompressionSettings}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getCompression = function() {
  return /** @type{?proto.JFJochProtoBuf.CompressionSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.CompressionSettings, 6));
};


/**
 * @param {?proto.JFJochProtoBuf.CompressionSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
*/
proto.JFJochProtoBuf.DetectorMetadata.prototype.setCompression = function(value) {
  return jspb.Message.setWrapperField(this, 6, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.clearCompression = function() {
  return this.setCompression(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.hasCompression = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * optional double image_time_us = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getImageTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 7, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setImageTimeUs = function(value) {
  return jspb.Message.setProto3FloatField(this, 7, value);
};


/**
 * optional double count_time_us = 8;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getCountTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 8, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setCountTimeUs = function(value) {
  return jspb.Message.setProto3FloatField(this, 8, value);
};


/**
 * optional ExperimentGeometry geometry = 9;
 * @return {?proto.JFJochProtoBuf.ExperimentGeometry}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getGeometry = function() {
  return /** @type{?proto.JFJochProtoBuf.ExperimentGeometry} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.ExperimentGeometry, 9));
};


/**
 * @param {?proto.JFJochProtoBuf.ExperimentGeometry|undefined} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
*/
proto.JFJochProtoBuf.DetectorMetadata.prototype.setGeometry = function(value) {
  return jspb.Message.setWrapperField(this, 9, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.clearGeometry = function() {
  return this.setGeometry(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.hasGeometry = function() {
  return jspb.Message.getField(this, 9) != null;
};


/**
 * optional int64 overload = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getOverload = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setOverload = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional int64 underload = 11;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getUnderload = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 11, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setUnderload = function(value) {
  return jspb.Message.setField(this, 11, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.clearUnderload = function() {
  return jspb.Message.setField(this, 11, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.hasUnderload = function() {
  return jspb.Message.getField(this, 11) != null;
};


/**
 * optional double delay_after_trigger_us = 12;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.getDelayAfterTriggerUs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 12, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorMetadata} returns this
 */
proto.JFJochProtoBuf.DetectorMetadata.prototype.setDelayAfterTriggerUs = function(value) {
  return jspb.Message.setProto3FloatField(this, 12, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BeamMetadata.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BeamMetadata} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BeamMetadata.toObject = function(includeInstance, msg) {
  var f, obj = {
    energyKev: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    transmission: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    totalFluxPhs: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    beamSizeXUm: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    beamSizeYUm: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BeamMetadata}
 */
proto.JFJochProtoBuf.BeamMetadata.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BeamMetadata;
  return proto.JFJochProtoBuf.BeamMetadata.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BeamMetadata} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BeamMetadata}
 */
proto.JFJochProtoBuf.BeamMetadata.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setEnergyKev(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setTransmission(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setTotalFluxPhs(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamSizeXUm(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamSizeYUm(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BeamMetadata.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BeamMetadata} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BeamMetadata.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getEnergyKev();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 2));
  if (f != null) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 3));
  if (f != null) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 4));
  if (f != null) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 5));
  if (f != null) {
    writer.writeDouble(
      5,
      f
    );
  }
};


/**
 * optional double energy_kev = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.getEnergyKev = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.setEnergyKev = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional double transmission = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.getTransmission = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.setTransmission = function(value) {
  return jspb.Message.setField(this, 2, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.clearTransmission = function() {
  return jspb.Message.setField(this, 2, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.hasTransmission = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional double total_flux_phs = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.getTotalFluxPhs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.setTotalFluxPhs = function(value) {
  return jspb.Message.setField(this, 3, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.clearTotalFluxPhs = function() {
  return jspb.Message.setField(this, 3, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.hasTotalFluxPhs = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional double beam_size_x_um = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.getBeamSizeXUm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.setBeamSizeXUm = function(value) {
  return jspb.Message.setField(this, 4, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.clearBeamSizeXUm = function() {
  return jspb.Message.setField(this, 4, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.hasBeamSizeXUm = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional double beam_size_y_um = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.getBeamSizeYUm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.setBeamSizeYUm = function(value) {
  return jspb.Message.setField(this, 5, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BeamMetadata} returns this
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.clearBeamSizeYUm = function() {
  return jspb.Message.setField(this, 5, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BeamMetadata.prototype.hasBeamSizeYUm = function() {
  return jspb.Message.getField(this, 5) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.SampleMetadata.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.SampleMetadata} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SampleMetadata.toObject = function(includeInstance, msg) {
  var f, obj = {
    name: jspb.Message.getFieldWithDefault(msg, 1, ""),
    unitCell: (f = msg.getUnitCell()) && proto.JFJochProtoBuf.UnitCell.toObject(includeInstance, f),
    spaceGroup: jspb.Message.getFieldWithDefault(msg, 3, 0),
    temperatureK: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    omega: (f = msg.getOmega()) && proto.JFJochProtoBuf.RotationSettings.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.SampleMetadata}
 */
proto.JFJochProtoBuf.SampleMetadata.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.SampleMetadata;
  return proto.JFJochProtoBuf.SampleMetadata.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.SampleMetadata} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.SampleMetadata}
 */
proto.JFJochProtoBuf.SampleMetadata.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setName(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.UnitCell;
      reader.readMessage(value,proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader);
      msg.setUnitCell(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpaceGroup(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setTemperatureK(value);
      break;
    case 5:
      var value = new proto.JFJochProtoBuf.RotationSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.RotationSettings.deserializeBinaryFromReader);
      msg.setOmega(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.SampleMetadata.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.SampleMetadata} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SampleMetadata.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getName();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getUnitCell();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 3));
  if (f != null) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 4));
  if (f != null) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getOmega();
  if (f != null) {
    writer.writeMessage(
      5,
      f,
      proto.JFJochProtoBuf.RotationSettings.serializeBinaryToWriter
    );
  }
};


/**
 * optional string name = 1;
 * @return {string}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.getName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.setName = function(value) {
  return jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * optional UnitCell unit_cell = 2;
 * @return {?proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.getUnitCell = function() {
  return /** @type{?proto.JFJochProtoBuf.UnitCell} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.UnitCell, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.UnitCell|undefined} value
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
*/
proto.JFJochProtoBuf.SampleMetadata.prototype.setUnitCell = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.clearUnitCell = function() {
  return this.setUnitCell(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.hasUnitCell = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional int64 space_group = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.getSpaceGroup = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.setSpaceGroup = function(value) {
  return jspb.Message.setField(this, 3, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.clearSpaceGroup = function() {
  return jspb.Message.setField(this, 3, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.hasSpaceGroup = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional double temperature_K = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.getTemperatureK = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.setTemperatureK = function(value) {
  return jspb.Message.setField(this, 4, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.clearTemperatureK = function() {
  return jspb.Message.setField(this, 4, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.hasTemperatureK = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional RotationSettings omega = 5;
 * @return {?proto.JFJochProtoBuf.RotationSettings}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.getOmega = function() {
  return /** @type{?proto.JFJochProtoBuf.RotationSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.RotationSettings, 5));
};


/**
 * @param {?proto.JFJochProtoBuf.RotationSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
*/
proto.JFJochProtoBuf.SampleMetadata.prototype.setOmega = function(value) {
  return jspb.Message.setWrapperField(this, 5, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.SampleMetadata} returns this
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.clearOmega = function() {
  return this.setOmega(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.SampleMetadata.prototype.hasOmega = function() {
  return jspb.Message.getField(this, 5) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.CalibrationMetadata.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.CalibrationMetadata} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.CalibrationMetadata.toObject = function(includeInstance, msg) {
  var f, obj = {
    maskBshufLz4: msg.getMaskBshufLz4_asB64(),
    calibrationMap: (f = msg.getCalibrationMap()) ? f.toObject(includeInstance, undefined) : []
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.CalibrationMetadata}
 */
proto.JFJochProtoBuf.CalibrationMetadata.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.CalibrationMetadata;
  return proto.JFJochProtoBuf.CalibrationMetadata.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.CalibrationMetadata} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.CalibrationMetadata}
 */
proto.JFJochProtoBuf.CalibrationMetadata.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setMaskBshufLz4(value);
      break;
    case 2:
      var value = msg.getCalibrationMap();
      reader.readMessage(value, function(message, reader) {
        jspb.Map.deserializeBinary(message, reader, jspb.BinaryReader.prototype.readString, jspb.BinaryReader.prototype.readBytes, null, "", "");
         });
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.CalibrationMetadata.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.CalibrationMetadata} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.CalibrationMetadata.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getMaskBshufLz4_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      1,
      f
    );
  }
  f = message.getCalibrationMap(true);
  if (f && f.getLength() > 0) {
    f.serializeBinary(2, writer, jspb.BinaryWriter.prototype.writeString, jspb.BinaryWriter.prototype.writeBytes);
  }
};


/**
 * optional bytes mask_bshuf_lz4 = 1;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.getMaskBshufLz4 = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * optional bytes mask_bshuf_lz4 = 1;
 * This is a type-conversion wrapper around `getMaskBshufLz4()`
 * @return {string}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.getMaskBshufLz4_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getMaskBshufLz4()));
};


/**
 * optional bytes mask_bshuf_lz4 = 1;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getMaskBshufLz4()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.getMaskBshufLz4_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getMaskBshufLz4()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.CalibrationMetadata} returns this
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.setMaskBshufLz4 = function(value) {
  return jspb.Message.setProto3BytesField(this, 1, value);
};


/**
 * map<string, bytes> calibration = 2;
 * @param {boolean=} opt_noLazyCreate Do not create the map if
 * empty, instead returning `undefined`
 * @return {!jspb.Map<string,!(string|Uint8Array)>}
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.getCalibrationMap = function(opt_noLazyCreate) {
  return /** @type {!jspb.Map<string,!(string|Uint8Array)>} */ (
      jspb.Message.getMapField(this, 2, opt_noLazyCreate,
      null));
};


/**
 * Clears values from the map. The map will be non-null.
 * @return {!proto.JFJochProtoBuf.CalibrationMetadata} returns this
 */
proto.JFJochProtoBuf.CalibrationMetadata.prototype.clearCalibrationMap = function() {
  this.getCalibrationMap().clear();
  return this;};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.WriterMetadataInput.repeatedFields_ = [10];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.WriterMetadataInput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.WriterMetadataInput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterMetadataInput.toObject = function(includeInstance, msg) {
  var f, obj = {
    facilityMetadata: (f = msg.getFacilityMetadata()) && proto.JFJochProtoBuf.FacilityMetadata.toObject(includeInstance, f),
    detectorMetadata: (f = msg.getDetectorMetadata()) && proto.JFJochProtoBuf.DetectorMetadata.toObject(includeInstance, f),
    beamMetadata: (f = msg.getBeamMetadata()) && proto.JFJochProtoBuf.BeamMetadata.toObject(includeInstance, f),
    sampleMetadata: (f = msg.getSampleMetadata()) && proto.JFJochProtoBuf.SampleMetadata.toObject(includeInstance, f),
    calibrationMetadata: (f = msg.getCalibrationMetadata()) && proto.JFJochProtoBuf.CalibrationMetadata.toObject(includeInstance, f),
    imageNumber: jspb.Message.getFieldWithDefault(msg, 6, 0),
    startTimeMs: jspb.Message.getFieldWithDefault(msg, 7, 0),
    endTimeMs: jspb.Message.getFieldWithDefault(msg, 8, 0),
    fileName: jspb.Message.getFieldWithDefault(msg, 9, ""),
    dataFilesList: jspb.Message.toObjectList(msg.getDataFilesList(),
    proto.JFJochProtoBuf.DataFile.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput}
 */
proto.JFJochProtoBuf.WriterMetadataInput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.WriterMetadataInput;
  return proto.JFJochProtoBuf.WriterMetadataInput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.WriterMetadataInput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput}
 */
proto.JFJochProtoBuf.WriterMetadataInput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.FacilityMetadata;
      reader.readMessage(value,proto.JFJochProtoBuf.FacilityMetadata.deserializeBinaryFromReader);
      msg.setFacilityMetadata(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.DetectorMetadata;
      reader.readMessage(value,proto.JFJochProtoBuf.DetectorMetadata.deserializeBinaryFromReader);
      msg.setDetectorMetadata(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.BeamMetadata;
      reader.readMessage(value,proto.JFJochProtoBuf.BeamMetadata.deserializeBinaryFromReader);
      msg.setBeamMetadata(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.SampleMetadata;
      reader.readMessage(value,proto.JFJochProtoBuf.SampleMetadata.deserializeBinaryFromReader);
      msg.setSampleMetadata(value);
      break;
    case 5:
      var value = new proto.JFJochProtoBuf.CalibrationMetadata;
      reader.readMessage(value,proto.JFJochProtoBuf.CalibrationMetadata.deserializeBinaryFromReader);
      msg.setCalibrationMetadata(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageNumber(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setStartTimeMs(value);
      break;
    case 8:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setEndTimeMs(value);
      break;
    case 9:
      var value = /** @type {string} */ (reader.readString());
      msg.setFileName(value);
      break;
    case 10:
      var value = new proto.JFJochProtoBuf.DataFile;
      reader.readMessage(value,proto.JFJochProtoBuf.DataFile.deserializeBinaryFromReader);
      msg.addDataFiles(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.WriterMetadataInput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.WriterMetadataInput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterMetadataInput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFacilityMetadata();
  if (f != null) {
    writer.writeMessage(
      1,
      f,
      proto.JFJochProtoBuf.FacilityMetadata.serializeBinaryToWriter
    );
  }
  f = message.getDetectorMetadata();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.DetectorMetadata.serializeBinaryToWriter
    );
  }
  f = message.getBeamMetadata();
  if (f != null) {
    writer.writeMessage(
      3,
      f,
      proto.JFJochProtoBuf.BeamMetadata.serializeBinaryToWriter
    );
  }
  f = message.getSampleMetadata();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.SampleMetadata.serializeBinaryToWriter
    );
  }
  f = message.getCalibrationMetadata();
  if (f != null) {
    writer.writeMessage(
      5,
      f,
      proto.JFJochProtoBuf.CalibrationMetadata.serializeBinaryToWriter
    );
  }
  f = message.getImageNumber();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
  f = message.getStartTimeMs();
  if (f !== 0) {
    writer.writeUint64(
      7,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 8));
  if (f != null) {
    writer.writeUint64(
      8,
      f
    );
  }
  f = message.getFileName();
  if (f.length > 0) {
    writer.writeString(
      9,
      f
    );
  }
  f = message.getDataFilesList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      10,
      f,
      proto.JFJochProtoBuf.DataFile.serializeBinaryToWriter
    );
  }
};


/**
 * optional FacilityMetadata facility_metadata = 1;
 * @return {?proto.JFJochProtoBuf.FacilityMetadata}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getFacilityMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.FacilityMetadata} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.FacilityMetadata, 1));
};


/**
 * @param {?proto.JFJochProtoBuf.FacilityMetadata|undefined} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setFacilityMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 1, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearFacilityMetadata = function() {
  return this.setFacilityMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasFacilityMetadata = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional DetectorMetadata detector_metadata = 2;
 * @return {?proto.JFJochProtoBuf.DetectorMetadata}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getDetectorMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.DetectorMetadata} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.DetectorMetadata, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.DetectorMetadata|undefined} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setDetectorMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearDetectorMetadata = function() {
  return this.setDetectorMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasDetectorMetadata = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional BeamMetadata beam_metadata = 3;
 * @return {?proto.JFJochProtoBuf.BeamMetadata}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getBeamMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.BeamMetadata} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.BeamMetadata, 3));
};


/**
 * @param {?proto.JFJochProtoBuf.BeamMetadata|undefined} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setBeamMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 3, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearBeamMetadata = function() {
  return this.setBeamMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasBeamMetadata = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional SampleMetadata sample_metadata = 4;
 * @return {?proto.JFJochProtoBuf.SampleMetadata}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getSampleMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.SampleMetadata} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.SampleMetadata, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.SampleMetadata|undefined} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setSampleMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearSampleMetadata = function() {
  return this.setSampleMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasSampleMetadata = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional CalibrationMetadata calibration_metadata = 5;
 * @return {?proto.JFJochProtoBuf.CalibrationMetadata}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getCalibrationMetadata = function() {
  return /** @type{?proto.JFJochProtoBuf.CalibrationMetadata} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.CalibrationMetadata, 5));
};


/**
 * @param {?proto.JFJochProtoBuf.CalibrationMetadata|undefined} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setCalibrationMetadata = function(value) {
  return jspb.Message.setWrapperField(this, 5, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearCalibrationMetadata = function() {
  return this.setCalibrationMetadata(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasCalibrationMetadata = function() {
  return jspb.Message.getField(this, 5) != null;
};


/**
 * optional int64 image_number = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setImageNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional uint64 start_time_ms = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getStartTimeMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setStartTimeMs = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional uint64 end_time_ms = 8;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getEndTimeMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 8, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setEndTimeMs = function(value) {
  return jspb.Message.setField(this, 8, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearEndTimeMs = function() {
  return jspb.Message.setField(this, 8, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.hasEndTimeMs = function() {
  return jspb.Message.getField(this, 8) != null;
};


/**
 * optional string file_name = 9;
 * @return {string}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getFileName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 9, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setFileName = function(value) {
  return jspb.Message.setProto3StringField(this, 9, value);
};


/**
 * repeated DataFile data_files = 10;
 * @return {!Array<!proto.JFJochProtoBuf.DataFile>}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.getDataFilesList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.DataFile>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.DataFile, 10));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.DataFile>} value
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
*/
proto.JFJochProtoBuf.WriterMetadataInput.prototype.setDataFilesList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 10, value);
};


/**
 * @param {!proto.JFJochProtoBuf.DataFile=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.DataFile}
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.addDataFiles = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 10, opt_value, proto.JFJochProtoBuf.DataFile, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.WriterMetadataInput} returns this
 */
proto.JFJochProtoBuf.WriterMetadataInput.prototype.clearDataFilesList = function() {
  return this.setDataFilesList([]);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.WriterSpot.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.WriterSpot.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.WriterSpot} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterSpot.toObject = function(includeInstance, msg) {
  var f, obj = {
    x: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    y: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    intensity: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.WriterSpot}
 */
proto.JFJochProtoBuf.WriterSpot.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.WriterSpot;
  return proto.JFJochProtoBuf.WriterSpot.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.WriterSpot} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.WriterSpot}
 */
proto.JFJochProtoBuf.WriterSpot.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setX(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setY(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setIntensity(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterSpot.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.WriterSpot.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.WriterSpot} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterSpot.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getX();
  if (f !== 0.0) {
    writer.writeFloat(
      1,
      f
    );
  }
  f = message.getY();
  if (f !== 0.0) {
    writer.writeFloat(
      2,
      f
    );
  }
  f = message.getIntensity();
  if (f !== 0.0) {
    writer.writeFloat(
      3,
      f
    );
  }
};


/**
 * optional float x = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterSpot.prototype.getX = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterSpot} returns this
 */
proto.JFJochProtoBuf.WriterSpot.prototype.setX = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional float y = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterSpot.prototype.getY = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterSpot} returns this
 */
proto.JFJochProtoBuf.WriterSpot.prototype.setY = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional float intensity = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterSpot.prototype.getIntensity = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterSpot} returns this
 */
proto.JFJochProtoBuf.WriterSpot.prototype.setIntensity = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.WriterFrame.repeatedFields_ = [4];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.WriterFrame.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.WriterFrame} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterFrame.toObject = function(includeInstance, msg) {
  var f, obj = {
    type: jspb.Message.getFieldWithDefault(msg, 1, 0),
    fileNumber: jspb.Message.getFieldWithDefault(msg, 2, 0),
    imageNumber: jspb.Message.getFieldWithDefault(msg, 3, 0),
    spotCoordList: jspb.Message.toObjectList(msg.getSpotCoordList(),
    proto.JFJochProtoBuf.WriterSpot.toObject, includeInstance),
    data: msg.getData_asB64()
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.WriterFrame}
 */
proto.JFJochProtoBuf.WriterFrame.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.WriterFrame;
  return proto.JFJochProtoBuf.WriterFrame.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.WriterFrame} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.WriterFrame}
 */
proto.JFJochProtoBuf.WriterFrame.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.WriterFrameType} */ (reader.readEnum());
      msg.setType(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setFileNumber(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageNumber(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.WriterSpot;
      reader.readMessage(value,proto.JFJochProtoBuf.WriterSpot.deserializeBinaryFromReader);
      msg.addSpotCoord(value);
      break;
    case 5:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setData(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.WriterFrame.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.WriterFrame} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.WriterFrame.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getType();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 2));
  if (f != null) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 3));
  if (f != null) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getSpotCoordList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      4,
      f,
      proto.JFJochProtoBuf.WriterSpot.serializeBinaryToWriter
    );
  }
  f = /** @type {!(string|Uint8Array)} */ (jspb.Message.getField(message, 5));
  if (f != null) {
    writer.writeBytes(
      5,
      f
    );
  }
};


/**
 * optional WriterFrameType type = 1;
 * @return {!proto.JFJochProtoBuf.WriterFrameType}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getType = function() {
  return /** @type {!proto.JFJochProtoBuf.WriterFrameType} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.WriterFrameType} value
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.setType = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional int64 file_number = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getFileNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.setFileNumber = function(value) {
  return jspb.Message.setField(this, 2, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.clearFileNumber = function() {
  return jspb.Message.setField(this, 2, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.hasFileNumber = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional int64 image_number = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.setImageNumber = function(value) {
  return jspb.Message.setField(this, 3, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.clearImageNumber = function() {
  return jspb.Message.setField(this, 3, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.hasImageNumber = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * repeated WriterSpot spot_coord = 4;
 * @return {!Array<!proto.JFJochProtoBuf.WriterSpot>}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getSpotCoordList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.WriterSpot>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.WriterSpot, 4));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.WriterSpot>} value
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
*/
proto.JFJochProtoBuf.WriterFrame.prototype.setSpotCoordList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 4, value);
};


/**
 * @param {!proto.JFJochProtoBuf.WriterSpot=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.WriterSpot}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.addSpotCoord = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 4, opt_value, proto.JFJochProtoBuf.WriterSpot, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.clearSpotCoordList = function() {
  return this.setSpotCoordList([]);
};


/**
 * optional bytes data = 5;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getData = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 5, ""));
};


/**
 * optional bytes data = 5;
 * This is a type-conversion wrapper around `getData()`
 * @return {string}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getData_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getData()));
};


/**
 * optional bytes data = 5;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getData()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.getData_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getData()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.setData = function(value) {
  return jspb.Message.setField(this, 5, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.WriterFrame} returns this
 */
proto.JFJochProtoBuf.WriterFrame.prototype.clearData = function() {
  return jspb.Message.setField(this, 5, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.WriterFrame.prototype.hasData = function() {
  return jspb.Message.getField(this, 5) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorModuleConfig.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorModuleConfig} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorModuleConfig.toObject = function(includeInstance, msg) {
  var f, obj = {
    udpDestPort1: jspb.Message.getFieldWithDefault(msg, 1, 0),
    udpDestPort2: jspb.Message.getFieldWithDefault(msg, 2, 0),
    ipv4SrcAddr1: jspb.Message.getFieldWithDefault(msg, 3, ""),
    ipv4SrcAddr2: jspb.Message.getFieldWithDefault(msg, 4, ""),
    ipv4DestAddr1: jspb.Message.getFieldWithDefault(msg, 5, ""),
    ipv4DestAddr2: jspb.Message.getFieldWithDefault(msg, 6, ""),
    macAddrDest1: jspb.Message.getFieldWithDefault(msg, 7, ""),
    macAddrDest2: jspb.Message.getFieldWithDefault(msg, 8, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorModuleConfig;
  return proto.JFJochProtoBuf.DetectorModuleConfig.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorModuleConfig} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setUdpDestPort1(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setUdpDestPort2(value);
      break;
    case 3:
      var value = /** @type {string} */ (reader.readString());
      msg.setIpv4SrcAddr1(value);
      break;
    case 4:
      var value = /** @type {string} */ (reader.readString());
      msg.setIpv4SrcAddr2(value);
      break;
    case 5:
      var value = /** @type {string} */ (reader.readString());
      msg.setIpv4DestAddr1(value);
      break;
    case 6:
      var value = /** @type {string} */ (reader.readString());
      msg.setIpv4DestAddr2(value);
      break;
    case 7:
      var value = /** @type {string} */ (reader.readString());
      msg.setMacAddrDest1(value);
      break;
    case 8:
      var value = /** @type {string} */ (reader.readString());
      msg.setMacAddrDest2(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorModuleConfig.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorModuleConfig} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorModuleConfig.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getUdpDestPort1();
  if (f !== 0) {
    writer.writeUint64(
      1,
      f
    );
  }
  f = message.getUdpDestPort2();
  if (f !== 0) {
    writer.writeUint64(
      2,
      f
    );
  }
  f = message.getIpv4SrcAddr1();
  if (f.length > 0) {
    writer.writeString(
      3,
      f
    );
  }
  f = message.getIpv4SrcAddr2();
  if (f.length > 0) {
    writer.writeString(
      4,
      f
    );
  }
  f = message.getIpv4DestAddr1();
  if (f.length > 0) {
    writer.writeString(
      5,
      f
    );
  }
  f = message.getIpv4DestAddr2();
  if (f.length > 0) {
    writer.writeString(
      6,
      f
    );
  }
  f = message.getMacAddrDest1();
  if (f.length > 0) {
    writer.writeString(
      7,
      f
    );
  }
  f = message.getMacAddrDest2();
  if (f.length > 0) {
    writer.writeString(
      8,
      f
    );
  }
};


/**
 * optional uint64 udp_dest_port_1 = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getUdpDestPort1 = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setUdpDestPort1 = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional uint64 udp_dest_port_2 = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getUdpDestPort2 = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setUdpDestPort2 = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional string ipv4_src_addr_1 = 3;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getIpv4SrcAddr1 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 3, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setIpv4SrcAddr1 = function(value) {
  return jspb.Message.setProto3StringField(this, 3, value);
};


/**
 * optional string ipv4_src_addr_2 = 4;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getIpv4SrcAddr2 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 4, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setIpv4SrcAddr2 = function(value) {
  return jspb.Message.setProto3StringField(this, 4, value);
};


/**
 * optional string ipv4_dest_addr_1 = 5;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getIpv4DestAddr1 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 5, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setIpv4DestAddr1 = function(value) {
  return jspb.Message.setProto3StringField(this, 5, value);
};


/**
 * optional string ipv4_dest_addr_2 = 6;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getIpv4DestAddr2 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 6, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setIpv4DestAddr2 = function(value) {
  return jspb.Message.setProto3StringField(this, 6, value);
};


/**
 * optional string mac_addr_dest_1 = 7;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getMacAddrDest1 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 7, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setMacAddrDest1 = function(value) {
  return jspb.Message.setProto3StringField(this, 7, value);
};


/**
 * optional string mac_addr_dest_2 = 8;
 * @return {string}
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.getMacAddrDest2 = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 8, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig} returns this
 */
proto.JFJochProtoBuf.DetectorModuleConfig.prototype.setMacAddrDest2 = function(value) {
  return jspb.Message.setProto3StringField(this, 8, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.DetectorConfig.repeatedFields_ = [1];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorConfig.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorConfig.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorConfig} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorConfig.toObject = function(includeInstance, msg) {
  var f, obj = {
    modulesList: jspb.Message.toObjectList(msg.getModulesList(),
    proto.JFJochProtoBuf.DetectorModuleConfig.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorConfig}
 */
proto.JFJochProtoBuf.DetectorConfig.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorConfig;
  return proto.JFJochProtoBuf.DetectorConfig.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorConfig} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorConfig}
 */
proto.JFJochProtoBuf.DetectorConfig.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.DetectorModuleConfig;
      reader.readMessage(value,proto.JFJochProtoBuf.DetectorModuleConfig.deserializeBinaryFromReader);
      msg.addModules(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorConfig.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorConfig.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorConfig} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorConfig.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getModulesList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      1,
      f,
      proto.JFJochProtoBuf.DetectorModuleConfig.serializeBinaryToWriter
    );
  }
};


/**
 * repeated DetectorModuleConfig modules = 1;
 * @return {!Array<!proto.JFJochProtoBuf.DetectorModuleConfig>}
 */
proto.JFJochProtoBuf.DetectorConfig.prototype.getModulesList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.DetectorModuleConfig>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.DetectorModuleConfig, 1));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.DetectorModuleConfig>} value
 * @return {!proto.JFJochProtoBuf.DetectorConfig} returns this
*/
proto.JFJochProtoBuf.DetectorConfig.prototype.setModulesList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 1, value);
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorModuleConfig=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.DetectorModuleConfig}
 */
proto.JFJochProtoBuf.DetectorConfig.prototype.addModules = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 1, opt_value, proto.JFJochProtoBuf.DetectorModuleConfig, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.DetectorConfig} returns this
 */
proto.JFJochProtoBuf.DetectorConfig.prototype.clearModulesList = function() {
  return this.setModulesList([]);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorInput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorInput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorInput.toObject = function(includeInstance, msg) {
  var f, obj = {
    modulesNum: jspb.Message.getFieldWithDefault(msg, 1, 0),
    mode: jspb.Message.getFieldWithDefault(msg, 2, 0),
    numFrames: jspb.Message.getFieldWithDefault(msg, 3, 0),
    numTriggers: jspb.Message.getFieldWithDefault(msg, 4, 0),
    storageCellNumber: jspb.Message.getFieldWithDefault(msg, 5, 0),
    storageCellStart: jspb.Message.getFieldWithDefault(msg, 6, 0),
    storageCellDelay: jspb.Message.getFieldWithDefault(msg, 7, 0),
    periodUs: jspb.Message.getFieldWithDefault(msg, 9, 0),
    countTimeUs: jspb.Message.getFieldWithDefault(msg, 10, 0),
    delayUs: jspb.Message.getFieldWithDefault(msg, 11, 0),
    softTrigger: jspb.Message.getBooleanFieldWithDefault(msg, 12, false)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorInput}
 */
proto.JFJochProtoBuf.DetectorInput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorInput;
  return proto.JFJochProtoBuf.DetectorInput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorInput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorInput}
 */
proto.JFJochProtoBuf.DetectorInput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setModulesNum(value);
      break;
    case 2:
      var value = /** @type {!proto.JFJochProtoBuf.DetectorMode} */ (reader.readEnum());
      msg.setMode(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setNumFrames(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setNumTriggers(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCellNumber(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCellStart(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCellDelay(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPeriodUs(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setCountTimeUs(value);
      break;
    case 11:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setDelayUs(value);
      break;
    case 12:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setSoftTrigger(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorInput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorInput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorInput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getModulesNum();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getMode();
  if (f !== 0.0) {
    writer.writeEnum(
      2,
      f
    );
  }
  f = message.getNumFrames();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getNumTriggers();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getStorageCellNumber();
  if (f !== 0) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = message.getStorageCellStart();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
  f = message.getStorageCellDelay();
  if (f !== 0) {
    writer.writeInt64(
      7,
      f
    );
  }
  f = message.getPeriodUs();
  if (f !== 0) {
    writer.writeInt64(
      9,
      f
    );
  }
  f = message.getCountTimeUs();
  if (f !== 0) {
    writer.writeInt64(
      10,
      f
    );
  }
  f = message.getDelayUs();
  if (f !== 0) {
    writer.writeInt64(
      11,
      f
    );
  }
  f = message.getSoftTrigger();
  if (f) {
    writer.writeBool(
      12,
      f
    );
  }
};


/**
 * optional int64 modules_num = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getModulesNum = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setModulesNum = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional DetectorMode mode = 2;
 * @return {!proto.JFJochProtoBuf.DetectorMode}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getMode = function() {
  return /** @type {!proto.JFJochProtoBuf.DetectorMode} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorMode} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setMode = function(value) {
  return jspb.Message.setProto3EnumField(this, 2, value);
};


/**
 * optional int64 num_frames = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getNumFrames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setNumFrames = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 num_triggers = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getNumTriggers = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setNumTriggers = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional int64 storage_cell_number = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getStorageCellNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setStorageCellNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional int64 storage_cell_start = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getStorageCellStart = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setStorageCellStart = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};


/**
 * optional int64 storage_cell_delay = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getStorageCellDelay = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setStorageCellDelay = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional int64 period_us = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getPeriodUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setPeriodUs = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};


/**
 * optional int64 count_time_us = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getCountTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setCountTimeUs = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional int64 delay_us = 11;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getDelayUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 11, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setDelayUs = function(value) {
  return jspb.Message.setProto3IntField(this, 11, value);
};


/**
 * optional bool soft_trigger = 12;
 * @return {boolean}
 */
proto.JFJochProtoBuf.DetectorInput.prototype.getSoftTrigger = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 12, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.DetectorInput} returns this
 */
proto.JFJochProtoBuf.DetectorInput.prototype.setSoftTrigger = function(value) {
  return jspb.Message.setProto3BooleanField(this, 12, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorOutput.toObject = function(includeInstance, msg) {
  var f, obj = {

  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorOutput}
 */
proto.JFJochProtoBuf.DetectorOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorOutput;
  return proto.JFJochProtoBuf.DetectorOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorOutput}
 */
proto.JFJochProtoBuf.DetectorOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DetectorStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DetectorStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    status: jspb.Message.getFieldWithDefault(msg, 1, 0),
    fwVersion: jspb.Message.getFieldWithDefault(msg, 2, 0),
    serverVersion: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DetectorStatus}
 */
proto.JFJochProtoBuf.DetectorStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DetectorStatus;
  return proto.JFJochProtoBuf.DetectorStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DetectorStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DetectorStatus}
 */
proto.JFJochProtoBuf.DetectorStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.DetectorStatusEnum} */ (reader.readEnum());
      msg.setStatus(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setFwVersion(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setServerVersion(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DetectorStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DetectorStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DetectorStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getStatus();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getFwVersion();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getServerVersion();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional DetectorStatusEnum status = 1;
 * @return {!proto.JFJochProtoBuf.DetectorStatusEnum}
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.getStatus = function() {
  return /** @type {!proto.JFJochProtoBuf.DetectorStatusEnum} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.DetectorStatusEnum} value
 * @return {!proto.JFJochProtoBuf.DetectorStatus} returns this
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.setStatus = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional int64 fw_version = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.getFwVersion = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorStatus} returns this
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.setFwVersion = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 server_version = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.getServerVersion = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DetectorStatus} returns this
 */
proto.JFJochProtoBuf.DetectorStatus.prototype.setServerVersion = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.FPGAStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.FPGAStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FPGAStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    packetsEther: jspb.Message.getFieldWithDefault(msg, 2, 0),
    packetsUdp: jspb.Message.getFieldWithDefault(msg, 3, 0),
    packetsJfjoch: jspb.Message.getFieldWithDefault(msg, 4, 0),
    packetsIcmp: jspb.Message.getFieldWithDefault(msg, 5, 0),
    fpgaIdle: jspb.Message.getBooleanFieldWithDefault(msg, 6, false),
    hbmTemp: jspb.Message.getFieldWithDefault(msg, 7, 0),
    maxHbmTemp: jspb.Message.getFieldWithDefault(msg, 8, 0),
    stallsHbm: jspb.Message.getFieldWithDefault(msg, 9, 0),
    stallsHost: jspb.Message.getFieldWithDefault(msg, 10, 0),
    ethernetRxAligned: jspb.Message.getBooleanFieldWithDefault(msg, 11, false),
    ethernetBadFcs: jspb.Message.getBooleanFieldWithDefault(msg, 12, false),
    fullStatusRegister: jspb.Message.getFieldWithDefault(msg, 13, 0),
    fifoStatusMap: (f = msg.getFifoStatusMap()) ? f.toObject(includeInstance, undefined) : [],
    maxModules: jspb.Message.getFieldWithDefault(msg, 15, 0),
    gitSha1: jspb.Message.getFieldWithDefault(msg, 16, 0),
    mailboxErrReg: jspb.Message.getFieldWithDefault(msg, 17, 0),
    mailboxStatusReg: jspb.Message.getFieldWithDefault(msg, 18, 0),
    datamoverMm2sError: jspb.Message.getBooleanFieldWithDefault(msg, 19, false),
    datamoverS2mmError: jspb.Message.getBooleanFieldWithDefault(msg, 20, false),
    frameStatisticsAlignmentErr: jspb.Message.getBooleanFieldWithDefault(msg, 21, false),
    frameStatisticsTlastErr: jspb.Message.getBooleanFieldWithDefault(msg, 22, false),
    frameStatisticsWorkReqErr: jspb.Message.getBooleanFieldWithDefault(msg, 23, false),
    slowestHead: jspb.Message.getFieldWithDefault(msg, 24, 0),
    qsfpModulePresent: jspb.Message.getBooleanFieldWithDefault(msg, 25, false),
    fpgaTempDegc: jspb.Message.getFloatingPointFieldWithDefault(msg, 26, 0.0),
    currentEdge12vA: jspb.Message.getFloatingPointFieldWithDefault(msg, 27, 0.0),
    voltageEdge12vV: jspb.Message.getFloatingPointFieldWithDefault(msg, 28, 0.0),
    currentEdge3p3vA: jspb.Message.getFloatingPointFieldWithDefault(msg, 29, 0.0),
    voltageEdge3p3vV: jspb.Message.getFloatingPointFieldWithDefault(msg, 30, 0.0),
    pcieH2cDescriptors: jspb.Message.getFieldWithDefault(msg, 31, 0),
    pcieC2hDescriptors: jspb.Message.getFieldWithDefault(msg, 32, 0),
    pcieH2cBeats: jspb.Message.getFieldWithDefault(msg, 33, 0),
    pcieC2hBeats: jspb.Message.getFieldWithDefault(msg, 34, 0),
    pcieH2cStatus: jspb.Message.getFieldWithDefault(msg, 35, 0),
    pcieC2hStatus: jspb.Message.getFieldWithDefault(msg, 36, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.FPGAStatus}
 */
proto.JFJochProtoBuf.FPGAStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.FPGAStatus;
  return proto.JFJochProtoBuf.FPGAStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.FPGAStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.FPGAStatus}
 */
proto.JFJochProtoBuf.FPGAStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 2:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsEther(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsUdp(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsJfjoch(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPacketsIcmp(value);
      break;
    case 6:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setFpgaIdle(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setHbmTemp(value);
      break;
    case 8:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaxHbmTemp(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setStallsHbm(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setStallsHost(value);
      break;
    case 11:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setEthernetRxAligned(value);
      break;
    case 12:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setEthernetBadFcs(value);
      break;
    case 13:
      var value = /** @type {number} */ (reader.readUint32());
      msg.setFullStatusRegister(value);
      break;
    case 14:
      var value = msg.getFifoStatusMap();
      reader.readMessage(value, function(message, reader) {
        jspb.Map.deserializeBinary(message, reader, jspb.BinaryReader.prototype.readString, jspb.BinaryReader.prototype.readEnum, null, "", 0);
         });
      break;
    case 15:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaxModules(value);
      break;
    case 16:
      var value = /** @type {number} */ (reader.readUint32());
      msg.setGitSha1(value);
      break;
    case 17:
      var value = /** @type {number} */ (reader.readUint32());
      msg.setMailboxErrReg(value);
      break;
    case 18:
      var value = /** @type {number} */ (reader.readUint32());
      msg.setMailboxStatusReg(value);
      break;
    case 19:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setDatamoverMm2sError(value);
      break;
    case 20:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setDatamoverS2mmError(value);
      break;
    case 21:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setFrameStatisticsAlignmentErr(value);
      break;
    case 22:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setFrameStatisticsTlastErr(value);
      break;
    case 23:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setFrameStatisticsWorkReqErr(value);
      break;
    case 24:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setSlowestHead(value);
      break;
    case 25:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setQsfpModulePresent(value);
      break;
    case 26:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setFpgaTempDegc(value);
      break;
    case 27:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCurrentEdge12vA(value);
      break;
    case 28:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setVoltageEdge12vV(value);
      break;
    case 29:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCurrentEdge3p3vA(value);
      break;
    case 30:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setVoltageEdge3p3vV(value);
      break;
    case 31:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieH2cDescriptors(value);
      break;
    case 32:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieC2hDescriptors(value);
      break;
    case 33:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieH2cBeats(value);
      break;
    case 34:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieC2hBeats(value);
      break;
    case 35:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieH2cStatus(value);
      break;
    case 36:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setPcieC2hStatus(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.FPGAStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.FPGAStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.FPGAStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getPacketsEther();
  if (f !== 0) {
    writer.writeUint64(
      2,
      f
    );
  }
  f = message.getPacketsUdp();
  if (f !== 0) {
    writer.writeUint64(
      3,
      f
    );
  }
  f = message.getPacketsJfjoch();
  if (f !== 0) {
    writer.writeUint64(
      4,
      f
    );
  }
  f = message.getPacketsIcmp();
  if (f !== 0) {
    writer.writeUint64(
      5,
      f
    );
  }
  f = message.getFpgaIdle();
  if (f) {
    writer.writeBool(
      6,
      f
    );
  }
  f = message.getHbmTemp();
  if (f !== 0) {
    writer.writeUint64(
      7,
      f
    );
  }
  f = message.getMaxHbmTemp();
  if (f !== 0) {
    writer.writeUint64(
      8,
      f
    );
  }
  f = message.getStallsHbm();
  if (f !== 0) {
    writer.writeUint64(
      9,
      f
    );
  }
  f = message.getStallsHost();
  if (f !== 0) {
    writer.writeUint64(
      10,
      f
    );
  }
  f = message.getEthernetRxAligned();
  if (f) {
    writer.writeBool(
      11,
      f
    );
  }
  f = message.getEthernetBadFcs();
  if (f) {
    writer.writeBool(
      12,
      f
    );
  }
  f = message.getFullStatusRegister();
  if (f !== 0) {
    writer.writeUint32(
      13,
      f
    );
  }
  f = message.getFifoStatusMap(true);
  if (f && f.getLength() > 0) {
    f.serializeBinary(14, writer, jspb.BinaryWriter.prototype.writeString, jspb.BinaryWriter.prototype.writeEnum);
  }
  f = message.getMaxModules();
  if (f !== 0) {
    writer.writeUint64(
      15,
      f
    );
  }
  f = message.getGitSha1();
  if (f !== 0) {
    writer.writeUint32(
      16,
      f
    );
  }
  f = message.getMailboxErrReg();
  if (f !== 0) {
    writer.writeUint32(
      17,
      f
    );
  }
  f = message.getMailboxStatusReg();
  if (f !== 0) {
    writer.writeUint32(
      18,
      f
    );
  }
  f = message.getDatamoverMm2sError();
  if (f) {
    writer.writeBool(
      19,
      f
    );
  }
  f = message.getDatamoverS2mmError();
  if (f) {
    writer.writeBool(
      20,
      f
    );
  }
  f = message.getFrameStatisticsAlignmentErr();
  if (f) {
    writer.writeBool(
      21,
      f
    );
  }
  f = message.getFrameStatisticsTlastErr();
  if (f) {
    writer.writeBool(
      22,
      f
    );
  }
  f = message.getFrameStatisticsWorkReqErr();
  if (f) {
    writer.writeBool(
      23,
      f
    );
  }
  f = message.getSlowestHead();
  if (f !== 0) {
    writer.writeUint64(
      24,
      f
    );
  }
  f = message.getQsfpModulePresent();
  if (f) {
    writer.writeBool(
      25,
      f
    );
  }
  f = message.getFpgaTempDegc();
  if (f !== 0.0) {
    writer.writeDouble(
      26,
      f
    );
  }
  f = message.getCurrentEdge12vA();
  if (f !== 0.0) {
    writer.writeDouble(
      27,
      f
    );
  }
  f = message.getVoltageEdge12vV();
  if (f !== 0.0) {
    writer.writeDouble(
      28,
      f
    );
  }
  f = message.getCurrentEdge3p3vA();
  if (f !== 0.0) {
    writer.writeDouble(
      29,
      f
    );
  }
  f = message.getVoltageEdge3p3vV();
  if (f !== 0.0) {
    writer.writeDouble(
      30,
      f
    );
  }
  f = message.getPcieH2cDescriptors();
  if (f !== 0) {
    writer.writeUint64(
      31,
      f
    );
  }
  f = message.getPcieC2hDescriptors();
  if (f !== 0) {
    writer.writeUint64(
      32,
      f
    );
  }
  f = message.getPcieH2cBeats();
  if (f !== 0) {
    writer.writeUint64(
      33,
      f
    );
  }
  f = message.getPcieC2hBeats();
  if (f !== 0) {
    writer.writeUint64(
      34,
      f
    );
  }
  f = message.getPcieH2cStatus();
  if (f !== 0) {
    writer.writeUint64(
      35,
      f
    );
  }
  f = message.getPcieC2hStatus();
  if (f !== 0) {
    writer.writeUint64(
      36,
      f
    );
  }
};


/**
 * optional uint64 packets_ether = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPacketsEther = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPacketsEther = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional uint64 packets_udp = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPacketsUdp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPacketsUdp = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional uint64 packets_jfjoch = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPacketsJfjoch = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPacketsJfjoch = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional uint64 packets_icmp = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPacketsIcmp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPacketsIcmp = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional bool fpga_idle = 6;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFpgaIdle = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 6, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFpgaIdle = function(value) {
  return jspb.Message.setProto3BooleanField(this, 6, value);
};


/**
 * optional uint64 hbm_temp = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getHbmTemp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setHbmTemp = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional uint64 max_hbm_temp = 8;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getMaxHbmTemp = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 8, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setMaxHbmTemp = function(value) {
  return jspb.Message.setProto3IntField(this, 8, value);
};


/**
 * optional uint64 stalls_hbm = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getStallsHbm = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setStallsHbm = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};


/**
 * optional uint64 stalls_host = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getStallsHost = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setStallsHost = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional bool ethernet_rx_aligned = 11;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getEthernetRxAligned = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 11, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setEthernetRxAligned = function(value) {
  return jspb.Message.setProto3BooleanField(this, 11, value);
};


/**
 * optional bool ethernet_bad_fcs = 12;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getEthernetBadFcs = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 12, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setEthernetBadFcs = function(value) {
  return jspb.Message.setProto3BooleanField(this, 12, value);
};


/**
 * optional uint32 full_status_register = 13;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFullStatusRegister = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 13, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFullStatusRegister = function(value) {
  return jspb.Message.setProto3IntField(this, 13, value);
};


/**
 * map<string, FPGAFIFOStatus> fifo_status = 14;
 * @param {boolean=} opt_noLazyCreate Do not create the map if
 * empty, instead returning `undefined`
 * @return {!jspb.Map<string,!proto.JFJochProtoBuf.FPGAFIFOStatus>}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFifoStatusMap = function(opt_noLazyCreate) {
  return /** @type {!jspb.Map<string,!proto.JFJochProtoBuf.FPGAFIFOStatus>} */ (
      jspb.Message.getMapField(this, 14, opt_noLazyCreate,
      null));
};


/**
 * Clears values from the map. The map will be non-null.
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.clearFifoStatusMap = function() {
  this.getFifoStatusMap().clear();
  return this;};


/**
 * optional uint64 max_modules = 15;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getMaxModules = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 15, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setMaxModules = function(value) {
  return jspb.Message.setProto3IntField(this, 15, value);
};


/**
 * optional uint32 git_sha1 = 16;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getGitSha1 = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 16, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setGitSha1 = function(value) {
  return jspb.Message.setProto3IntField(this, 16, value);
};


/**
 * optional uint32 mailbox_err_reg = 17;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getMailboxErrReg = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 17, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setMailboxErrReg = function(value) {
  return jspb.Message.setProto3IntField(this, 17, value);
};


/**
 * optional uint32 mailbox_status_reg = 18;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getMailboxStatusReg = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 18, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setMailboxStatusReg = function(value) {
  return jspb.Message.setProto3IntField(this, 18, value);
};


/**
 * optional bool datamover_mm2s_error = 19;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getDatamoverMm2sError = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 19, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setDatamoverMm2sError = function(value) {
  return jspb.Message.setProto3BooleanField(this, 19, value);
};


/**
 * optional bool datamover_s2mm_error = 20;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getDatamoverS2mmError = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 20, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setDatamoverS2mmError = function(value) {
  return jspb.Message.setProto3BooleanField(this, 20, value);
};


/**
 * optional bool frame_statistics_alignment_err = 21;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFrameStatisticsAlignmentErr = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 21, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFrameStatisticsAlignmentErr = function(value) {
  return jspb.Message.setProto3BooleanField(this, 21, value);
};


/**
 * optional bool frame_statistics_tlast_err = 22;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFrameStatisticsTlastErr = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 22, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFrameStatisticsTlastErr = function(value) {
  return jspb.Message.setProto3BooleanField(this, 22, value);
};


/**
 * optional bool frame_statistics_work_req_err = 23;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFrameStatisticsWorkReqErr = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 23, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFrameStatisticsWorkReqErr = function(value) {
  return jspb.Message.setProto3BooleanField(this, 23, value);
};


/**
 * optional uint64 slowest_head = 24;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getSlowestHead = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 24, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setSlowestHead = function(value) {
  return jspb.Message.setProto3IntField(this, 24, value);
};


/**
 * optional bool qsfp_module_present = 25;
 * @return {boolean}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getQsfpModulePresent = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 25, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setQsfpModulePresent = function(value) {
  return jspb.Message.setProto3BooleanField(this, 25, value);
};


/**
 * optional double fpga_temp_degC = 26;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getFpgaTempDegc = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 26, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setFpgaTempDegc = function(value) {
  return jspb.Message.setProto3FloatField(this, 26, value);
};


/**
 * optional double current_edge_12V_A = 27;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getCurrentEdge12vA = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 27, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setCurrentEdge12vA = function(value) {
  return jspb.Message.setProto3FloatField(this, 27, value);
};


/**
 * optional double voltage_edge_12V_V = 28;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getVoltageEdge12vV = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 28, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setVoltageEdge12vV = function(value) {
  return jspb.Message.setProto3FloatField(this, 28, value);
};


/**
 * optional double current_edge_3p3V_A = 29;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getCurrentEdge3p3vA = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 29, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setCurrentEdge3p3vA = function(value) {
  return jspb.Message.setProto3FloatField(this, 29, value);
};


/**
 * optional double voltage_edge_3p3V_V = 30;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getVoltageEdge3p3vV = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 30, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setVoltageEdge3p3vV = function(value) {
  return jspb.Message.setProto3FloatField(this, 30, value);
};


/**
 * optional uint64 pcie_h2c_descriptors = 31;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieH2cDescriptors = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 31, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieH2cDescriptors = function(value) {
  return jspb.Message.setProto3IntField(this, 31, value);
};


/**
 * optional uint64 pcie_c2h_descriptors = 32;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieC2hDescriptors = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 32, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieC2hDescriptors = function(value) {
  return jspb.Message.setProto3IntField(this, 32, value);
};


/**
 * optional uint64 pcie_h2c_beats = 33;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieH2cBeats = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 33, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieH2cBeats = function(value) {
  return jspb.Message.setProto3IntField(this, 33, value);
};


/**
 * optional uint64 pcie_c2h_beats = 34;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieC2hBeats = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 34, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieC2hBeats = function(value) {
  return jspb.Message.setProto3IntField(this, 34, value);
};


/**
 * optional uint64 pcie_h2c_status = 35;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieH2cStatus = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 35, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieH2cStatus = function(value) {
  return jspb.Message.setProto3IntField(this, 35, value);
};


/**
 * optional uint64 pcie_c2h_status = 36;
 * @return {number}
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.getPcieC2hStatus = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 36, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.FPGAStatus} returns this
 */
proto.JFJochProtoBuf.FPGAStatus.prototype.setPcieC2hStatus = function(value) {
  return jspb.Message.setProto3IntField(this, 36, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.Plot.repeatedFields_ = [1,2];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.Plot.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.Plot.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.Plot} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Plot.toObject = function(includeInstance, msg) {
  var f, obj = {
    xList: (f = jspb.Message.getRepeatedFloatingPointField(msg, 1)) == null ? undefined : f,
    yList: (f = jspb.Message.getRepeatedFloatingPointField(msg, 2)) == null ? undefined : f
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.Plot.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.Plot;
  return proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.Plot} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedFloat() : [reader.readFloat()]);
      for (var i = 0; i < values.length; i++) {
        msg.addX(values[i]);
      }
      break;
    case 2:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedFloat() : [reader.readFloat()]);
      for (var i = 0; i < values.length; i++) {
        msg.addY(values[i]);
      }
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.Plot.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.Plot.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.Plot} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Plot.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getXList();
  if (f.length > 0) {
    writer.writePackedFloat(
      1,
      f
    );
  }
  f = message.getYList();
  if (f.length > 0) {
    writer.writePackedFloat(
      2,
      f
    );
  }
};


/**
 * repeated float x = 1;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.Plot.prototype.getXList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedFloatingPointField(this, 1));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.setXList = function(value) {
  return jspb.Message.setField(this, 1, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.addX = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 1, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.clearXList = function() {
  return this.setXList([]);
};


/**
 * repeated float y = 2;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.Plot.prototype.getYList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedFloatingPointField(this, 2));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.setYList = function(value) {
  return jspb.Message.setField(this, 2, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.addY = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 2, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.Plot} returns this
 */
proto.JFJochProtoBuf.Plot.prototype.clearYList = function() {
  return this.setYList([]);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.ReceiverStatus.repeatedFields_ = [2];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ReceiverStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ReceiverStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    progress: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    fpgaStatusList: jspb.Message.toObjectList(msg.getFpgaStatusList(),
    proto.JFJochProtoBuf.FPGAStatus.toObject, includeInstance),
    idle: jspb.Message.getBooleanFieldWithDefault(msg, 3, false),
    bkgEstimate: (f = msg.getBkgEstimate()) && proto.JFJochProtoBuf.Plot.toObject(includeInstance, f),
    radialIntProfile: (f = msg.getRadialIntProfile()) && proto.JFJochProtoBuf.Plot.toObject(includeInstance, f),
    spotCount: (f = msg.getSpotCount()) && proto.JFJochProtoBuf.Plot.toObject(includeInstance, f),
    masterFileName: jspb.Message.getFieldWithDefault(msg, 7, "")
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus}
 */
proto.JFJochProtoBuf.ReceiverStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ReceiverStatus;
  return proto.JFJochProtoBuf.ReceiverStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ReceiverStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus}
 */
proto.JFJochProtoBuf.ReceiverStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setProgress(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.FPGAStatus;
      reader.readMessage(value,proto.JFJochProtoBuf.FPGAStatus.deserializeBinaryFromReader);
      msg.addFpgaStatus(value);
      break;
    case 3:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setIdle(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.Plot;
      reader.readMessage(value,proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader);
      msg.setBkgEstimate(value);
      break;
    case 5:
      var value = new proto.JFJochProtoBuf.Plot;
      reader.readMessage(value,proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader);
      msg.setRadialIntProfile(value);
      break;
    case 6:
      var value = new proto.JFJochProtoBuf.Plot;
      reader.readMessage(value,proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader);
      msg.setSpotCount(value);
      break;
    case 7:
      var value = /** @type {string} */ (reader.readString());
      msg.setMasterFileName(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ReceiverStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ReceiverStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ReceiverStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getProgress();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getFpgaStatusList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      2,
      f,
      proto.JFJochProtoBuf.FPGAStatus.serializeBinaryToWriter
    );
  }
  f = message.getIdle();
  if (f) {
    writer.writeBool(
      3,
      f
    );
  }
  f = message.getBkgEstimate();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.Plot.serializeBinaryToWriter
    );
  }
  f = message.getRadialIntProfile();
  if (f != null) {
    writer.writeMessage(
      5,
      f,
      proto.JFJochProtoBuf.Plot.serializeBinaryToWriter
    );
  }
  f = message.getSpotCount();
  if (f != null) {
    writer.writeMessage(
      6,
      f,
      proto.JFJochProtoBuf.Plot.serializeBinaryToWriter
    );
  }
  f = message.getMasterFileName();
  if (f.length > 0) {
    writer.writeString(
      7,
      f
    );
  }
};


/**
 * optional double progress = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getProgress = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.setProgress = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * repeated FPGAStatus fpga_status = 2;
 * @return {!Array<!proto.JFJochProtoBuf.FPGAStatus>}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getFpgaStatusList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.FPGAStatus>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.FPGAStatus, 2));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.FPGAStatus>} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
*/
proto.JFJochProtoBuf.ReceiverStatus.prototype.setFpgaStatusList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 2, value);
};


/**
 * @param {!proto.JFJochProtoBuf.FPGAStatus=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.FPGAStatus}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.addFpgaStatus = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 2, opt_value, proto.JFJochProtoBuf.FPGAStatus, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.clearFpgaStatusList = function() {
  return this.setFpgaStatusList([]);
};


/**
 * optional bool idle = 3;
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getIdle = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 3, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.setIdle = function(value) {
  return jspb.Message.setProto3BooleanField(this, 3, value);
};


/**
 * optional Plot bkg_estimate = 4;
 * @return {?proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getBkgEstimate = function() {
  return /** @type{?proto.JFJochProtoBuf.Plot} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Plot, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.Plot|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
*/
proto.JFJochProtoBuf.ReceiverStatus.prototype.setBkgEstimate = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.clearBkgEstimate = function() {
  return this.setBkgEstimate(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.hasBkgEstimate = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional Plot radial_int_profile = 5;
 * @return {?proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getRadialIntProfile = function() {
  return /** @type{?proto.JFJochProtoBuf.Plot} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Plot, 5));
};


/**
 * @param {?proto.JFJochProtoBuf.Plot|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
*/
proto.JFJochProtoBuf.ReceiverStatus.prototype.setRadialIntProfile = function(value) {
  return jspb.Message.setWrapperField(this, 5, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.clearRadialIntProfile = function() {
  return this.setRadialIntProfile(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.hasRadialIntProfile = function() {
  return jspb.Message.getField(this, 5) != null;
};


/**
 * optional Plot spot_count = 6;
 * @return {?proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getSpotCount = function() {
  return /** @type{?proto.JFJochProtoBuf.Plot} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Plot, 6));
};


/**
 * @param {?proto.JFJochProtoBuf.Plot|undefined} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
*/
proto.JFJochProtoBuf.ReceiverStatus.prototype.setSpotCount = function(value) {
  return jspb.Message.setWrapperField(this, 6, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.clearSpotCount = function() {
  return this.setSpotCount(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.hasSpotCount = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * optional string master_file_name = 7;
 * @return {string}
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.getMasterFileName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 7, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.ReceiverStatus} returns this
 */
proto.JFJochProtoBuf.ReceiverStatus.prototype.setMasterFileName = function(value) {
  return jspb.Message.setProto3StringField(this, 7, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.DataProcessingSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DataProcessingSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    signalToNoiseThreshold: jspb.Message.getFloatingPointFieldWithDefault(msg, 1, 0.0),
    photonCountThreshold: jspb.Message.getFieldWithDefault(msg, 2, 0),
    minPixPerSpot: jspb.Message.getFieldWithDefault(msg, 3, 0),
    maxPixPerSpot: jspb.Message.getFieldWithDefault(msg, 4, 0),
    localBkgSize: jspb.Message.getFieldWithDefault(msg, 5, 0),
    highResolutionLimit: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0),
    lowResolutionLimit: jspb.Message.getFloatingPointFieldWithDefault(msg, 7, 0.0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings}
 */
proto.JFJochProtoBuf.DataProcessingSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.DataProcessingSettings;
  return proto.JFJochProtoBuf.DataProcessingSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings}
 */
proto.JFJochProtoBuf.DataProcessingSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setSignalToNoiseThreshold(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPhotonCountThreshold(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setMinPixPerSpot(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setMaxPixPerSpot(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setLocalBkgSize(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setHighResolutionLimit(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setLowResolutionLimit(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.DataProcessingSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.DataProcessingSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getSignalToNoiseThreshold();
  if (f !== 0.0) {
    writer.writeDouble(
      1,
      f
    );
  }
  f = message.getPhotonCountThreshold();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getMinPixPerSpot();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getMaxPixPerSpot();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getLocalBkgSize();
  if (f !== 0) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 6));
  if (f != null) {
    writer.writeDouble(
      6,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 7));
  if (f != null) {
    writer.writeDouble(
      7,
      f
    );
  }
};


/**
 * optional double signal_to_noise_threshold = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getSignalToNoiseThreshold = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 1, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setSignalToNoiseThreshold = function(value) {
  return jspb.Message.setProto3FloatField(this, 1, value);
};


/**
 * optional int64 photon_count_threshold = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getPhotonCountThreshold = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setPhotonCountThreshold = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 min_pix_per_spot = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getMinPixPerSpot = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setMinPixPerSpot = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 max_pix_per_spot = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getMaxPixPerSpot = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setMaxPixPerSpot = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional int64 local_bkg_size = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getLocalBkgSize = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setLocalBkgSize = function(value) {
  return jspb.Message.setProto3IntField(this, 5, value);
};


/**
 * optional double high_resolution_limit = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getHighResolutionLimit = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setHighResolutionLimit = function(value) {
  return jspb.Message.setField(this, 6, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.clearHighResolutionLimit = function() {
  return jspb.Message.setField(this, 6, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.hasHighResolutionLimit = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * optional double low_resolution_limit = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.getLowResolutionLimit = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 7, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.setLowResolutionLimit = function(value) {
  return jspb.Message.setField(this, 7, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.DataProcessingSettings} returns this
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.clearLowResolutionLimit = function() {
  return jspb.Message.setField(this, 7, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.DataProcessingSettings.prototype.hasLowResolutionLimit = function() {
  return jspb.Message.getField(this, 7) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.PreviewFrame.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.PreviewFrame} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PreviewFrame.toObject = function(includeInstance, msg) {
  var f, obj = {
    imageNumber: jspb.Message.getFieldWithDefault(msg, 1, 0),
    totalImages: jspb.Message.getFieldWithDefault(msg, 2, 0),
    wavelengthA: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    beamCenterX: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    beamCenterY: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0),
    detectorDistanceMm: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0),
    saturationValue: jspb.Message.getFieldWithDefault(msg, 7, 0),
    filePrefix: jspb.Message.getFieldWithDefault(msg, 8, ""),
    width: jspb.Message.getFieldWithDefault(msg, 9, 0),
    height: jspb.Message.getFieldWithDefault(msg, 10, 0),
    pixelDepth: jspb.Message.getFieldWithDefault(msg, 11, 0),
    compression: (f = msg.getCompression()) && proto.JFJochProtoBuf.CompressionSettings.toObject(includeInstance, f),
    data: msg.getData_asB64()
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.PreviewFrame}
 */
proto.JFJochProtoBuf.PreviewFrame.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.PreviewFrame;
  return proto.JFJochProtoBuf.PreviewFrame.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.PreviewFrame} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.PreviewFrame}
 */
proto.JFJochProtoBuf.PreviewFrame.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageNumber(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setTotalImages(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setWavelengthA(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamCenterX(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamCenterY(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setDetectorDistanceMm(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSaturationValue(value);
      break;
    case 8:
      var value = /** @type {string} */ (reader.readString());
      msg.setFilePrefix(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setWidth(value);
      break;
    case 10:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setHeight(value);
      break;
    case 11:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPixelDepth(value);
      break;
    case 12:
      var value = new proto.JFJochProtoBuf.CompressionSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.CompressionSettings.deserializeBinaryFromReader);
      msg.setCompression(value);
      break;
    case 13:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setData(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.PreviewFrame.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.PreviewFrame} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.PreviewFrame.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImageNumber();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getTotalImages();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getWavelengthA();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getBeamCenterX();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getBeamCenterY();
  if (f !== 0.0) {
    writer.writeDouble(
      5,
      f
    );
  }
  f = message.getDetectorDistanceMm();
  if (f !== 0.0) {
    writer.writeDouble(
      6,
      f
    );
  }
  f = message.getSaturationValue();
  if (f !== 0) {
    writer.writeInt64(
      7,
      f
    );
  }
  f = message.getFilePrefix();
  if (f.length > 0) {
    writer.writeString(
      8,
      f
    );
  }
  f = message.getWidth();
  if (f !== 0) {
    writer.writeInt64(
      9,
      f
    );
  }
  f = message.getHeight();
  if (f !== 0) {
    writer.writeInt64(
      10,
      f
    );
  }
  f = message.getPixelDepth();
  if (f !== 0) {
    writer.writeInt64(
      11,
      f
    );
  }
  f = message.getCompression();
  if (f != null) {
    writer.writeMessage(
      12,
      f,
      proto.JFJochProtoBuf.CompressionSettings.serializeBinaryToWriter
    );
  }
  f = message.getData_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      13,
      f
    );
  }
};


/**
 * optional int64 image_number = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setImageNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 total_images = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getTotalImages = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setTotalImages = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional double wavelength_A = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getWavelengthA = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setWavelengthA = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional double beam_center_x = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getBeamCenterX = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setBeamCenterX = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional double beam_center_y = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getBeamCenterY = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setBeamCenterY = function(value) {
  return jspb.Message.setProto3FloatField(this, 5, value);
};


/**
 * optional double detector_distance_mm = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getDetectorDistanceMm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setDetectorDistanceMm = function(value) {
  return jspb.Message.setProto3FloatField(this, 6, value);
};


/**
 * optional int64 saturation_value = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getSaturationValue = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setSaturationValue = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};


/**
 * optional string file_prefix = 8;
 * @return {string}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getFilePrefix = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 8, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setFilePrefix = function(value) {
  return jspb.Message.setProto3StringField(this, 8, value);
};


/**
 * optional int64 width = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getWidth = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setWidth = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};


/**
 * optional int64 height = 10;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getHeight = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 10, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setHeight = function(value) {
  return jspb.Message.setProto3IntField(this, 10, value);
};


/**
 * optional int64 pixel_depth = 11;
 * @return {number}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getPixelDepth = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 11, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setPixelDepth = function(value) {
  return jspb.Message.setProto3IntField(this, 11, value);
};


/**
 * optional CompressionSettings compression = 12;
 * @return {?proto.JFJochProtoBuf.CompressionSettings}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getCompression = function() {
  return /** @type{?proto.JFJochProtoBuf.CompressionSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.CompressionSettings, 12));
};


/**
 * @param {?proto.JFJochProtoBuf.CompressionSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
*/
proto.JFJochProtoBuf.PreviewFrame.prototype.setCompression = function(value) {
  return jspb.Message.setWrapperField(this, 12, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.clearCompression = function() {
  return this.setCompression(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.hasCompression = function() {
  return jspb.Message.getField(this, 12) != null;
};


/**
 * optional bytes data = 13;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getData = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 13, ""));
};


/**
 * optional bytes data = 13;
 * This is a type-conversion wrapper around `getData()`
 * @return {string}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getData_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getData()));
};


/**
 * optional bytes data = 13;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getData()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.getData_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getData()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.PreviewFrame} returns this
 */
proto.JFJochProtoBuf.PreviewFrame.prototype.setData = function(value) {
  return jspb.Message.setProto3BytesField(this, 13, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.repeatedFields_ = [3];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.SpotFinderImageOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.SpotFinderImageOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.toObject = function(includeInstance, msg) {
  var f, obj = {
    imageNumber: jspb.Message.getFieldWithDefault(msg, 1, 0),
    coordList: jspb.Message.toObjectList(msg.getCoordList(),
    proto.JFJochProtoBuf.Vector.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.SpotFinderImageOutput}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.SpotFinderImageOutput;
  return proto.JFJochProtoBuf.SpotFinderImageOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.SpotFinderImageOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.SpotFinderImageOutput}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageNumber(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.Vector;
      reader.readMessage(value,proto.JFJochProtoBuf.Vector.deserializeBinaryFromReader);
      msg.addCoord(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.SpotFinderImageOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.SpotFinderImageOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImageNumber();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getCoordList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      3,
      f,
      proto.JFJochProtoBuf.Vector.serializeBinaryToWriter
    );
  }
};


/**
 * optional int64 image_number = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.getImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.SpotFinderImageOutput} returns this
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.setImageNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * repeated Vector coord = 3;
 * @return {!Array<!proto.JFJochProtoBuf.Vector>}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.getCoordList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.Vector>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.Vector, 3));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.Vector>} value
 * @return {!proto.JFJochProtoBuf.SpotFinderImageOutput} returns this
*/
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.setCoordList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 3, value);
};


/**
 * @param {!proto.JFJochProtoBuf.Vector=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.Vector}
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.addCoord = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 3, opt_value, proto.JFJochProtoBuf.Vector, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.SpotFinderImageOutput} returns this
 */
proto.JFJochProtoBuf.SpotFinderImageOutput.prototype.clearCoordList = function() {
  return this.setCoordList([]);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.IndexerStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.IndexerStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    indexingRate: (f = msg.getIndexingRate()) && proto.JFJochProtoBuf.Plot.toObject(includeInstance, f),
    imagesAnalyzed: jspb.Message.getFieldWithDefault(msg, 3, 0),
    imagesIndexed: jspb.Message.getFieldWithDefault(msg, 4, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.IndexerStatus}
 */
proto.JFJochProtoBuf.IndexerStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.IndexerStatus;
  return proto.JFJochProtoBuf.IndexerStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.IndexerStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.IndexerStatus}
 */
proto.JFJochProtoBuf.IndexerStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.Plot;
      reader.readMessage(value,proto.JFJochProtoBuf.Plot.deserializeBinaryFromReader);
      msg.setIndexingRate(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesAnalyzed(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesIndexed(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.IndexerStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.IndexerStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getIndexingRate();
  if (f != null) {
    writer.writeMessage(
      1,
      f,
      proto.JFJochProtoBuf.Plot.serializeBinaryToWriter
    );
  }
  f = message.getImagesAnalyzed();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = message.getImagesIndexed();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
};


/**
 * optional Plot indexing_rate = 1;
 * @return {?proto.JFJochProtoBuf.Plot}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.getIndexingRate = function() {
  return /** @type{?proto.JFJochProtoBuf.Plot} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.Plot, 1));
};


/**
 * @param {?proto.JFJochProtoBuf.Plot|undefined} value
 * @return {!proto.JFJochProtoBuf.IndexerStatus} returns this
*/
proto.JFJochProtoBuf.IndexerStatus.prototype.setIndexingRate = function(value) {
  return jspb.Message.setWrapperField(this, 1, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.IndexerStatus} returns this
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.clearIndexingRate = function() {
  return this.setIndexingRate(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.hasIndexingRate = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional int64 images_analyzed = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.getImagesAnalyzed = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerStatus} returns this
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.setImagesAnalyzed = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};


/**
 * optional int64 images_indexed = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.getImagesIndexed = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerStatus} returns this
 */
proto.JFJochProtoBuf.IndexerStatus.prototype.setImagesIndexed = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.IndexerInput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.IndexerInput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerInput.toObject = function(includeInstance, msg) {
  var f, obj = {
    expectedImageNumber: jspb.Message.getFieldWithDefault(msg, 1, 0),
    imageStride: jspb.Message.getFieldWithDefault(msg, 2, 0),
    unitCell: (f = msg.getUnitCell()) && proto.JFJochProtoBuf.UnitCell.toObject(includeInstance, f),
    centering: jspb.Message.getFieldWithDefault(msg, 4, 0),
    zmqRecvPubAddr: jspb.Message.getFieldWithDefault(msg, 5, ""),
    binSize: jspb.Message.getFieldWithDefault(msg, 6, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.IndexerInput}
 */
proto.JFJochProtoBuf.IndexerInput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.IndexerInput;
  return proto.JFJochProtoBuf.IndexerInput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.IndexerInput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.IndexerInput}
 */
proto.JFJochProtoBuf.IndexerInput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setExpectedImageNumber(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageStride(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.UnitCell;
      reader.readMessage(value,proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader);
      msg.setUnitCell(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setCentering(value);
      break;
    case 5:
      var value = /** @type {string} */ (reader.readString());
      msg.setZmqRecvPubAddr(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBinSize(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.IndexerInput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.IndexerInput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerInput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getExpectedImageNumber();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getImageStride();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getUnitCell();
  if (f != null) {
    writer.writeMessage(
      3,
      f,
      proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter
    );
  }
  f = message.getCentering();
  if (f !== 0) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getZmqRecvPubAddr();
  if (f.length > 0) {
    writer.writeString(
      5,
      f
    );
  }
  f = message.getBinSize();
  if (f !== 0) {
    writer.writeInt64(
      6,
      f
    );
  }
};


/**
 * optional int64 expected_image_number = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getExpectedImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.setExpectedImageNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 image_stride = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getImageStride = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.setImageStride = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional UnitCell unit_cell = 3;
 * @return {?proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getUnitCell = function() {
  return /** @type{?proto.JFJochProtoBuf.UnitCell} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.UnitCell, 3));
};


/**
 * @param {?proto.JFJochProtoBuf.UnitCell|undefined} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
*/
proto.JFJochProtoBuf.IndexerInput.prototype.setUnitCell = function(value) {
  return jspb.Message.setWrapperField(this, 3, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.clearUnitCell = function() {
  return this.setUnitCell(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.hasUnitCell = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional int64 centering = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getCentering = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.setCentering = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * optional string zmq_recv_pub_addr = 5;
 * @return {string}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getZmqRecvPubAddr = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 5, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.setZmqRecvPubAddr = function(value) {
  return jspb.Message.setProto3StringField(this, 5, value);
};


/**
 * optional int64 bin_size = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerInput.prototype.getBinSize = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 6, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerInput} returns this
 */
proto.JFJochProtoBuf.IndexerInput.prototype.setBinSize = function(value) {
  return jspb.Message.setProto3IntField(this, 6, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.IndexerImageOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.IndexerImageOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerImageOutput.toObject = function(includeInstance, msg) {
  var f, obj = {
    imageNumber: jspb.Message.getFieldWithDefault(msg, 1, 0),
    spotCount: jspb.Message.getFieldWithDefault(msg, 2, 0),
    indexed: jspb.Message.getBooleanFieldWithDefault(msg, 3, false),
    unitCell: (f = msg.getUnitCell()) && proto.JFJochProtoBuf.UnitCell.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput}
 */
proto.JFJochProtoBuf.IndexerImageOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.IndexerImageOutput;
  return proto.JFJochProtoBuf.IndexerImageOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.IndexerImageOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput}
 */
proto.JFJochProtoBuf.IndexerImageOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImageNumber(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpotCount(value);
      break;
    case 3:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setIndexed(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.UnitCell;
      reader.readMessage(value,proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader);
      msg.setUnitCell(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.IndexerImageOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.IndexerImageOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerImageOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImageNumber();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getSpotCount();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getIndexed();
  if (f) {
    writer.writeBool(
      3,
      f
    );
  }
  f = message.getUnitCell();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter
    );
  }
};


/**
 * optional int64 image_number = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.getImageNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput} returns this
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.setImageNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 spot_count = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.getSpotCount = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput} returns this
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.setSpotCount = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional bool indexed = 3;
 * @return {boolean}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.getIndexed = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 3, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput} returns this
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.setIndexed = function(value) {
  return jspb.Message.setProto3BooleanField(this, 3, value);
};


/**
 * optional UnitCell unit_cell = 4;
 * @return {?proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.getUnitCell = function() {
  return /** @type{?proto.JFJochProtoBuf.UnitCell} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.UnitCell, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.UnitCell|undefined} value
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput} returns this
*/
proto.JFJochProtoBuf.IndexerImageOutput.prototype.setUnitCell = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput} returns this
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.clearUnitCell = function() {
  return this.setUnitCell(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.IndexerImageOutput.prototype.hasUnitCell = function() {
  return jspb.Message.getField(this, 4) != null;
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.IndexerOutput.repeatedFields_ = [1];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.IndexerOutput.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.IndexerOutput} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerOutput.toObject = function(includeInstance, msg) {
  var f, obj = {
    imageOutputList: jspb.Message.toObjectList(msg.getImageOutputList(),
    proto.JFJochProtoBuf.IndexerImageOutput.toObject, includeInstance),
    msPerImage: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    indexedImages: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.IndexerOutput}
 */
proto.JFJochProtoBuf.IndexerOutput.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.IndexerOutput;
  return proto.JFJochProtoBuf.IndexerOutput.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.IndexerOutput} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.IndexerOutput}
 */
proto.JFJochProtoBuf.IndexerOutput.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.IndexerImageOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.IndexerImageOutput.deserializeBinaryFromReader);
      msg.addImageOutput(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setMsPerImage(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setIndexedImages(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.IndexerOutput.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.IndexerOutput} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.IndexerOutput.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImageOutputList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      1,
      f,
      proto.JFJochProtoBuf.IndexerImageOutput.serializeBinaryToWriter
    );
  }
  f = message.getMsPerImage();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getIndexedImages();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * repeated IndexerImageOutput image_output = 1;
 * @return {!Array<!proto.JFJochProtoBuf.IndexerImageOutput>}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.getImageOutputList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.IndexerImageOutput>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.IndexerImageOutput, 1));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.IndexerImageOutput>} value
 * @return {!proto.JFJochProtoBuf.IndexerOutput} returns this
*/
proto.JFJochProtoBuf.IndexerOutput.prototype.setImageOutputList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 1, value);
};


/**
 * @param {!proto.JFJochProtoBuf.IndexerImageOutput=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.IndexerImageOutput}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.addImageOutput = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 1, opt_value, proto.JFJochProtoBuf.IndexerImageOutput, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.IndexerOutput} returns this
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.clearImageOutputList = function() {
  return this.setImageOutputList([]);
};


/**
 * optional double ms_per_image = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.getMsPerImage = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerOutput} returns this
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.setMsPerImage = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional int64 indexed_images = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.getIndexedImages = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.IndexerOutput} returns this
 */
proto.JFJochProtoBuf.IndexerOutput.prototype.setIndexedImages = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.ModuleStatistics.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.ModuleStatistics} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ModuleStatistics.toObject = function(includeInstance, msg) {
  var f, obj = {
    moduleNumber: jspb.Message.getFieldWithDefault(msg, 1, 0),
    storageCellNumber: jspb.Message.getFieldWithDefault(msg, 2, 0),
    pedestalG0Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    pedestalG1Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    pedestalG2Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0),
    pedestalRmsG0Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0),
    pedestalRmsG1Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 7, 0.0),
    pedestalRmsG2Mean: jspb.Message.getFloatingPointFieldWithDefault(msg, 8, 0.0),
    maskedPixels: jspb.Message.getFieldWithDefault(msg, 9, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.ModuleStatistics}
 */
proto.JFJochProtoBuf.ModuleStatistics.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.ModuleStatistics;
  return proto.JFJochProtoBuf.ModuleStatistics.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.ModuleStatistics} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.ModuleStatistics}
 */
proto.JFJochProtoBuf.ModuleStatistics.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setModuleNumber(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setStorageCellNumber(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalG0Mean(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalG1Mean(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalG2Mean(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalRmsG0Mean(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalRmsG1Mean(value);
      break;
    case 8:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPedestalRmsG2Mean(value);
      break;
    case 9:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setMaskedPixels(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.ModuleStatistics.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.ModuleStatistics} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.ModuleStatistics.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getModuleNumber();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = message.getStorageCellNumber();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getPedestalG0Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getPedestalG1Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getPedestalG2Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      5,
      f
    );
  }
  f = message.getPedestalRmsG0Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      6,
      f
    );
  }
  f = message.getPedestalRmsG1Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      7,
      f
    );
  }
  f = message.getPedestalRmsG2Mean();
  if (f !== 0.0) {
    writer.writeDouble(
      8,
      f
    );
  }
  f = message.getMaskedPixels();
  if (f !== 0) {
    writer.writeUint64(
      9,
      f
    );
  }
};


/**
 * optional int64 module_number = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getModuleNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setModuleNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 storage_cell_number = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getStorageCellNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setStorageCellNumber = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional double pedestal_g0_mean = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalG0Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalG0Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional double pedestal_g1_mean = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalG1Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalG1Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional double pedestal_g2_mean = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalG2Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalG2Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 5, value);
};


/**
 * optional double pedestal_rms_g0_mean = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalRmsG0Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalRmsG0Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 6, value);
};


/**
 * optional double pedestal_rms_g1_mean = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalRmsG1Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 7, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalRmsG1Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 7, value);
};


/**
 * optional double pedestal_rms_g2_mean = 8;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getPedestalRmsG2Mean = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 8, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setPedestalRmsG2Mean = function(value) {
  return jspb.Message.setProto3FloatField(this, 8, value);
};


/**
 * optional uint64 masked_pixels = 9;
 * @return {number}
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.getMaskedPixels = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 9, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.ModuleStatistics} returns this
 */
proto.JFJochProtoBuf.ModuleStatistics.prototype.setMaskedPixels = function(value) {
  return jspb.Message.setProto3IntField(this, 9, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.Image.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.Image.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.Image} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Image.toObject = function(includeInstance, msg) {
  var f, obj = {
    data: msg.getData_asB64(),
    width: jspb.Message.getFieldWithDefault(msg, 2, 0),
    height: jspb.Message.getFieldWithDefault(msg, 3, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.Image}
 */
proto.JFJochProtoBuf.Image.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.Image;
  return proto.JFJochProtoBuf.Image.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.Image} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.Image}
 */
proto.JFJochProtoBuf.Image.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setData(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setWidth(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setHeight(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.Image.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.Image.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.Image} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.Image.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getData_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      1,
      f
    );
  }
  f = message.getWidth();
  if (f !== 0) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getHeight();
  if (f !== 0) {
    writer.writeInt64(
      3,
      f
    );
  }
};


/**
 * optional bytes data = 1;
 * @return {!(string|Uint8Array)}
 */
proto.JFJochProtoBuf.Image.prototype.getData = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * optional bytes data = 1;
 * This is a type-conversion wrapper around `getData()`
 * @return {string}
 */
proto.JFJochProtoBuf.Image.prototype.getData_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getData()));
};


/**
 * optional bytes data = 1;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getData()`
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.Image.prototype.getData_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getData()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.JFJochProtoBuf.Image} returns this
 */
proto.JFJochProtoBuf.Image.prototype.setData = function(value) {
  return jspb.Message.setProto3BytesField(this, 1, value);
};


/**
 * optional int64 width = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.Image.prototype.getWidth = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.Image} returns this
 */
proto.JFJochProtoBuf.Image.prototype.setWidth = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};


/**
 * optional int64 height = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.Image.prototype.getHeight = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.Image} returns this
 */
proto.JFJochProtoBuf.Image.prototype.setHeight = function(value) {
  return jspb.Message.setProto3IntField(this, 3, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.MaskToLoad.repeatedFields_ = [1];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.MaskToLoad.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.MaskToLoad} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.MaskToLoad.toObject = function(includeInstance, msg) {
  var f, obj = {
    maskList: (f = jspb.Message.getRepeatedField(msg, 1)) == null ? undefined : f,
    bitToSet: jspb.Message.getFieldWithDefault(msg, 2, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.MaskToLoad}
 */
proto.JFJochProtoBuf.MaskToLoad.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.MaskToLoad;
  return proto.JFJochProtoBuf.MaskToLoad.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.MaskToLoad} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.MaskToLoad}
 */
proto.JFJochProtoBuf.MaskToLoad.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var values = /** @type {!Array<number>} */ (reader.isDelimited() ? reader.readPackedUint32() : [reader.readUint32()]);
      for (var i = 0; i < values.length; i++) {
        msg.addMask(values[i]);
      }
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt32());
      msg.setBitToSet(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.MaskToLoad.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.MaskToLoad} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.MaskToLoad.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getMaskList();
  if (f.length > 0) {
    writer.writePackedUint32(
      1,
      f
    );
  }
  f = message.getBitToSet();
  if (f !== 0) {
    writer.writeInt32(
      2,
      f
    );
  }
};


/**
 * repeated uint32 mask = 1;
 * @return {!Array<number>}
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.getMaskList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 1));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.JFJochProtoBuf.MaskToLoad} returns this
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.setMaskList = function(value) {
  return jspb.Message.setField(this, 1, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.MaskToLoad} returns this
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.addMask = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 1, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.MaskToLoad} returns this
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.clearMaskList = function() {
  return this.setMaskList([]);
};


/**
 * optional int32 bit_to_set = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.getBitToSet = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MaskToLoad} returns this
 */
proto.JFJochProtoBuf.MaskToLoad.prototype.setBitToSet = function(value) {
  return jspb.Message.setProto3IntField(this, 2, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BrokerSetup.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BrokerSetup} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerSetup.toObject = function(includeInstance, msg) {
  var f, obj = {
    imagesPerTrigger: jspb.Message.getFieldWithDefault(msg, 1, 0),
    summation: jspb.Message.getFieldWithDefault(msg, 2, 0),
    beamCenterXPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    beamCenterYPxl: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    detectorDistanceMm: jspb.Message.getFloatingPointFieldWithDefault(msg, 5, 0.0),
    photonEnergyKev: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0),
    namePattern: jspb.Message.getFieldWithDefault(msg, 7, ""),
    sampleName: jspb.Message.getFieldWithDefault(msg, 8, ""),
    softTrigger: jspb.Message.getBooleanFieldWithDefault(msg, 14, false),
    ntrigger: jspb.Message.getFieldWithDefault(msg, 15, 0),
    imagesPerFile: jspb.Message.getFieldWithDefault(msg, 16, 0),
    compression: jspb.Message.getFieldWithDefault(msg, 17, ""),
    omegaStartDeg: jspb.Message.getFloatingPointFieldWithDefault(msg, 18, 0.0),
    omegaIncrementDeg: jspb.Message.getFloatingPointFieldWithDefault(msg, 19, 0.0),
    timeResolvedMode: jspb.Message.getBooleanFieldWithDefault(msg, 20, false),
    dataCollectionMode: jspb.Message.getFieldWithDefault(msg, 23, ""),
    previewRateMs: jspb.Message.getFieldWithDefault(msg, 27, 0),
    unitCell: (f = msg.getUnitCell()) && proto.JFJochProtoBuf.UnitCell.toObject(includeInstance, f),
    spotFindingRateMs: jspb.Message.getFieldWithDefault(msg, 30, 0),
    bkgEstimateRateMs: jspb.Message.getFieldWithDefault(msg, 31, 0),
    spaceGroupNumber: jspb.Message.getFieldWithDefault(msg, 33, 0),
    detectorDelayAfterTriggerUs: jspb.Message.getFieldWithDefault(msg, 35, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BrokerSetup}
 */
proto.JFJochProtoBuf.BrokerSetup.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BrokerSetup;
  return proto.JFJochProtoBuf.BrokerSetup.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BrokerSetup} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BrokerSetup}
 */
proto.JFJochProtoBuf.BrokerSetup.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesPerTrigger(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSummation(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamCenterXPxl(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setBeamCenterYPxl(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setDetectorDistanceMm(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setPhotonEnergyKev(value);
      break;
    case 7:
      var value = /** @type {string} */ (reader.readString());
      msg.setNamePattern(value);
      break;
    case 8:
      var value = /** @type {string} */ (reader.readString());
      msg.setSampleName(value);
      break;
    case 14:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setSoftTrigger(value);
      break;
    case 15:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setNtrigger(value);
      break;
    case 16:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setImagesPerFile(value);
      break;
    case 17:
      var value = /** @type {string} */ (reader.readString());
      msg.setCompression(value);
      break;
    case 18:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setOmegaStartDeg(value);
      break;
    case 19:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setOmegaIncrementDeg(value);
      break;
    case 20:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setTimeResolvedMode(value);
      break;
    case 23:
      var value = /** @type {string} */ (reader.readString());
      msg.setDataCollectionMode(value);
      break;
    case 27:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPreviewRateMs(value);
      break;
    case 29:
      var value = new proto.JFJochProtoBuf.UnitCell;
      reader.readMessage(value,proto.JFJochProtoBuf.UnitCell.deserializeBinaryFromReader);
      msg.setUnitCell(value);
      break;
    case 30:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpotFindingRateMs(value);
      break;
    case 31:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setBkgEstimateRateMs(value);
      break;
    case 33:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setSpaceGroupNumber(value);
      break;
    case 35:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setDetectorDelayAfterTriggerUs(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BrokerSetup.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BrokerSetup} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerSetup.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImagesPerTrigger();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 2));
  if (f != null) {
    writer.writeInt64(
      2,
      f
    );
  }
  f = message.getBeamCenterXPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = message.getBeamCenterYPxl();
  if (f !== 0.0) {
    writer.writeDouble(
      4,
      f
    );
  }
  f = message.getDetectorDistanceMm();
  if (f !== 0.0) {
    writer.writeDouble(
      5,
      f
    );
  }
  f = message.getPhotonEnergyKev();
  if (f !== 0.0) {
    writer.writeDouble(
      6,
      f
    );
  }
  f = /** @type {string} */ (jspb.Message.getField(message, 7));
  if (f != null) {
    writer.writeString(
      7,
      f
    );
  }
  f = message.getSampleName();
  if (f.length > 0) {
    writer.writeString(
      8,
      f
    );
  }
  f = /** @type {boolean} */ (jspb.Message.getField(message, 14));
  if (f != null) {
    writer.writeBool(
      14,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 15));
  if (f != null) {
    writer.writeInt64(
      15,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 16));
  if (f != null) {
    writer.writeInt64(
      16,
      f
    );
  }
  f = /** @type {string} */ (jspb.Message.getField(message, 17));
  if (f != null) {
    writer.writeString(
      17,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 18));
  if (f != null) {
    writer.writeDouble(
      18,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 19));
  if (f != null) {
    writer.writeDouble(
      19,
      f
    );
  }
  f = /** @type {boolean} */ (jspb.Message.getField(message, 20));
  if (f != null) {
    writer.writeBool(
      20,
      f
    );
  }
  f = /** @type {string} */ (jspb.Message.getField(message, 23));
  if (f != null) {
    writer.writeString(
      23,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 27));
  if (f != null) {
    writer.writeInt64(
      27,
      f
    );
  }
  f = message.getUnitCell();
  if (f != null) {
    writer.writeMessage(
      29,
      f,
      proto.JFJochProtoBuf.UnitCell.serializeBinaryToWriter
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 30));
  if (f != null) {
    writer.writeInt64(
      30,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 31));
  if (f != null) {
    writer.writeInt64(
      31,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 33));
  if (f != null) {
    writer.writeInt64(
      33,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 35));
  if (f != null) {
    writer.writeInt64(
      35,
      f
    );
  }
};


/**
 * optional int64 images_per_trigger = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getImagesPerTrigger = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setImagesPerTrigger = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 summation = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getSummation = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setSummation = function(value) {
  return jspb.Message.setField(this, 2, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearSummation = function() {
  return jspb.Message.setField(this, 2, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasSummation = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional double beam_center_x_pxl = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getBeamCenterXPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setBeamCenterXPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional double beam_center_y_pxl = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getBeamCenterYPxl = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setBeamCenterYPxl = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional double detector_distance_mm = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getDetectorDistanceMm = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 5, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setDetectorDistanceMm = function(value) {
  return jspb.Message.setProto3FloatField(this, 5, value);
};


/**
 * optional double photon_energy_kev = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getPhotonEnergyKev = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setPhotonEnergyKev = function(value) {
  return jspb.Message.setProto3FloatField(this, 6, value);
};


/**
 * optional string name_pattern = 7;
 * @return {string}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getNamePattern = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 7, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setNamePattern = function(value) {
  return jspb.Message.setField(this, 7, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearNamePattern = function() {
  return jspb.Message.setField(this, 7, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasNamePattern = function() {
  return jspb.Message.getField(this, 7) != null;
};


/**
 * optional string sample_name = 8;
 * @return {string}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getSampleName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 8, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setSampleName = function(value) {
  return jspb.Message.setProto3StringField(this, 8, value);
};


/**
 * optional bool soft_trigger = 14;
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getSoftTrigger = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 14, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setSoftTrigger = function(value) {
  return jspb.Message.setField(this, 14, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearSoftTrigger = function() {
  return jspb.Message.setField(this, 14, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasSoftTrigger = function() {
  return jspb.Message.getField(this, 14) != null;
};


/**
 * optional int64 ntrigger = 15;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getNtrigger = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 15, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setNtrigger = function(value) {
  return jspb.Message.setField(this, 15, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearNtrigger = function() {
  return jspb.Message.setField(this, 15, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasNtrigger = function() {
  return jspb.Message.getField(this, 15) != null;
};


/**
 * optional int64 images_per_file = 16;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getImagesPerFile = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 16, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setImagesPerFile = function(value) {
  return jspb.Message.setField(this, 16, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearImagesPerFile = function() {
  return jspb.Message.setField(this, 16, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasImagesPerFile = function() {
  return jspb.Message.getField(this, 16) != null;
};


/**
 * optional string compression = 17;
 * @return {string}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getCompression = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 17, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setCompression = function(value) {
  return jspb.Message.setField(this, 17, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearCompression = function() {
  return jspb.Message.setField(this, 17, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasCompression = function() {
  return jspb.Message.getField(this, 17) != null;
};


/**
 * optional double omega_start_deg = 18;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getOmegaStartDeg = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 18, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setOmegaStartDeg = function(value) {
  return jspb.Message.setField(this, 18, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearOmegaStartDeg = function() {
  return jspb.Message.setField(this, 18, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasOmegaStartDeg = function() {
  return jspb.Message.getField(this, 18) != null;
};


/**
 * optional double omega_increment_deg = 19;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getOmegaIncrementDeg = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 19, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setOmegaIncrementDeg = function(value) {
  return jspb.Message.setField(this, 19, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearOmegaIncrementDeg = function() {
  return jspb.Message.setField(this, 19, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasOmegaIncrementDeg = function() {
  return jspb.Message.getField(this, 19) != null;
};


/**
 * optional bool time_resolved_mode = 20;
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getTimeResolvedMode = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 20, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setTimeResolvedMode = function(value) {
  return jspb.Message.setField(this, 20, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearTimeResolvedMode = function() {
  return jspb.Message.setField(this, 20, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasTimeResolvedMode = function() {
  return jspb.Message.getField(this, 20) != null;
};


/**
 * optional string data_collection_mode = 23;
 * @return {string}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getDataCollectionMode = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 23, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setDataCollectionMode = function(value) {
  return jspb.Message.setField(this, 23, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearDataCollectionMode = function() {
  return jspb.Message.setField(this, 23, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasDataCollectionMode = function() {
  return jspb.Message.getField(this, 23) != null;
};


/**
 * optional int64 preview_rate_ms = 27;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getPreviewRateMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 27, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setPreviewRateMs = function(value) {
  return jspb.Message.setField(this, 27, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearPreviewRateMs = function() {
  return jspb.Message.setField(this, 27, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasPreviewRateMs = function() {
  return jspb.Message.getField(this, 27) != null;
};


/**
 * optional UnitCell unit_cell = 29;
 * @return {?proto.JFJochProtoBuf.UnitCell}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getUnitCell = function() {
  return /** @type{?proto.JFJochProtoBuf.UnitCell} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.UnitCell, 29));
};


/**
 * @param {?proto.JFJochProtoBuf.UnitCell|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
*/
proto.JFJochProtoBuf.BrokerSetup.prototype.setUnitCell = function(value) {
  return jspb.Message.setWrapperField(this, 29, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearUnitCell = function() {
  return this.setUnitCell(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasUnitCell = function() {
  return jspb.Message.getField(this, 29) != null;
};


/**
 * optional int64 spot_finding_rate_ms = 30;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getSpotFindingRateMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 30, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setSpotFindingRateMs = function(value) {
  return jspb.Message.setField(this, 30, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearSpotFindingRateMs = function() {
  return jspb.Message.setField(this, 30, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasSpotFindingRateMs = function() {
  return jspb.Message.getField(this, 30) != null;
};


/**
 * optional int64 bkg_estimate_rate_ms = 31;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getBkgEstimateRateMs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 31, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setBkgEstimateRateMs = function(value) {
  return jspb.Message.setField(this, 31, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearBkgEstimateRateMs = function() {
  return jspb.Message.setField(this, 31, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasBkgEstimateRateMs = function() {
  return jspb.Message.getField(this, 31) != null;
};


/**
 * optional int64 space_group_number = 33;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getSpaceGroupNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 33, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setSpaceGroupNumber = function(value) {
  return jspb.Message.setField(this, 33, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearSpaceGroupNumber = function() {
  return jspb.Message.setField(this, 33, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasSpaceGroupNumber = function() {
  return jspb.Message.getField(this, 33) != null;
};


/**
 * optional int64 detector_delay_after_trigger_us = 35;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.getDetectorDelayAfterTriggerUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 35, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.setDetectorDelayAfterTriggerUs = function(value) {
  return jspb.Message.setField(this, 35, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerSetup} returns this
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.clearDetectorDelayAfterTriggerUs = function() {
  return jspb.Message.setField(this, 35, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerSetup.prototype.hasDetectorDelayAfterTriggerUs = function() {
  return jspb.Message.getField(this, 35) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BrokerPersistentTimingSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    frameTimeUs: jspb.Message.getFieldWithDefault(msg, 1, 0),
    countTimeUs: jspb.Message.getFieldWithDefault(msg, 2, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BrokerPersistentTimingSettings;
  return proto.JFJochProtoBuf.BrokerPersistentTimingSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setFrameTimeUs(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setCountTimeUs(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BrokerPersistentTimingSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getFrameTimeUs();
  if (f !== 0) {
    writer.writeInt64(
      1,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 2));
  if (f != null) {
    writer.writeInt64(
      2,
      f
    );
  }
};


/**
 * optional int64 frame_time_us = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.getFrameTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.setFrameTimeUs = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional int64 count_time_us = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.getCountTimeUs = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 2, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.setCountTimeUs = function(value) {
  return jspb.Message.setField(this, 2, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentTimingSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.clearCountTimeUs = function() {
  return jspb.Message.setField(this, 2, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentTimingSettings.prototype.hasCountTimeUs = function() {
  return jspb.Message.getField(this, 2) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BrokerPersistentSettings.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BrokerPersistentSettings} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.toObject = function(includeInstance, msg) {
  var f, obj = {
    useStorageCells: jspb.Message.getBooleanFieldWithDefault(msg, 1, false),
    timing: (f = msg.getTiming()) && proto.JFJochProtoBuf.BrokerPersistentTimingSettings.toObject(includeInstance, f),
    pedestalG0Frames: jspb.Message.getFieldWithDefault(msg, 3, 0),
    pedestalG1Frames: jspb.Message.getFieldWithDefault(msg, 4, 0),
    pedestalG2Frames: jspb.Message.getFieldWithDefault(msg, 5, 0),
    useInternalPacketGenerator: jspb.Message.getBooleanFieldWithDefault(msg, 6, false),
    runNumber: jspb.Message.getFieldWithDefault(msg, 7, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BrokerPersistentSettings;
  return proto.JFJochProtoBuf.BrokerPersistentSettings.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BrokerPersistentSettings} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setUseStorageCells(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.BrokerPersistentTimingSettings;
      reader.readMessage(value,proto.JFJochProtoBuf.BrokerPersistentTimingSettings.deserializeBinaryFromReader);
      msg.setTiming(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPedestalG0Frames(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPedestalG1Frames(value);
      break;
    case 5:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setPedestalG2Frames(value);
      break;
    case 6:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setUseInternalPacketGenerator(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setRunNumber(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BrokerPersistentSettings.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BrokerPersistentSettings} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = /** @type {boolean} */ (jspb.Message.getField(message, 1));
  if (f != null) {
    writer.writeBool(
      1,
      f
    );
  }
  f = message.getTiming();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.BrokerPersistentTimingSettings.serializeBinaryToWriter
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 3));
  if (f != null) {
    writer.writeInt64(
      3,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 4));
  if (f != null) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 5));
  if (f != null) {
    writer.writeInt64(
      5,
      f
    );
  }
  f = /** @type {boolean} */ (jspb.Message.getField(message, 6));
  if (f != null) {
    writer.writeBool(
      6,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 7));
  if (f != null) {
    writer.writeInt64(
      7,
      f
    );
  }
};


/**
 * optional bool use_storage_cells = 1;
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getUseStorageCells = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 1, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setUseStorageCells = function(value) {
  return jspb.Message.setField(this, 1, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearUseStorageCells = function() {
  return jspb.Message.setField(this, 1, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasUseStorageCells = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional BrokerPersistentTimingSettings timing = 2;
 * @return {?proto.JFJochProtoBuf.BrokerPersistentTimingSettings}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getTiming = function() {
  return /** @type{?proto.JFJochProtoBuf.BrokerPersistentTimingSettings} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.BrokerPersistentTimingSettings, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.BrokerPersistentTimingSettings|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
*/
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setTiming = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearTiming = function() {
  return this.setTiming(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasTiming = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional int64 pedestal_g0_frames = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getPedestalG0Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setPedestalG0Frames = function(value) {
  return jspb.Message.setField(this, 3, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearPedestalG0Frames = function() {
  return jspb.Message.setField(this, 3, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasPedestalG0Frames = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional int64 pedestal_g1_frames = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getPedestalG1Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setPedestalG1Frames = function(value) {
  return jspb.Message.setField(this, 4, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearPedestalG1Frames = function() {
  return jspb.Message.setField(this, 4, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasPedestalG1Frames = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional int64 pedestal_g2_frames = 5;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getPedestalG2Frames = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 5, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setPedestalG2Frames = function(value) {
  return jspb.Message.setField(this, 5, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearPedestalG2Frames = function() {
  return jspb.Message.setField(this, 5, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasPedestalG2Frames = function() {
  return jspb.Message.getField(this, 5) != null;
};


/**
 * optional bool use_internal_packet_generator = 6;
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getUseInternalPacketGenerator = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 6, false));
};


/**
 * @param {boolean} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setUseInternalPacketGenerator = function(value) {
  return jspb.Message.setField(this, 6, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearUseInternalPacketGenerator = function() {
  return jspb.Message.setField(this, 6, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasUseInternalPacketGenerator = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * optional int64 run_number = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.getRunNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.setRunNumber = function(value) {
  return jspb.Message.setField(this, 7, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerPersistentSettings} returns this
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.clearRunNumber = function() {
  return jspb.Message.setField(this, 7, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerPersistentSettings.prototype.hasRunNumber = function() {
  return jspb.Message.getField(this, 7) != null;
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.MeasurementStatistics.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.MeasurementStatistics} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.MeasurementStatistics.toObject = function(includeInstance, msg) {
  var f, obj = {
    imagesCollected: jspb.Message.getFieldWithDefault(msg, 1, 0),
    collectionEfficiency: jspb.Message.getFloatingPointFieldWithDefault(msg, 2, 0.0),
    compressionRatio: jspb.Message.getFloatingPointFieldWithDefault(msg, 3, 0.0),
    runNumber: jspb.Message.getFieldWithDefault(msg, 4, 0),
    fileName: jspb.Message.getFieldWithDefault(msg, 5, ""),
    writerPerformanceMbs: jspb.Message.getFloatingPointFieldWithDefault(msg, 6, 0.0),
    imagesWritten: jspb.Message.getFieldWithDefault(msg, 7, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics}
 */
proto.JFJochProtoBuf.MeasurementStatistics.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.MeasurementStatistics;
  return proto.JFJochProtoBuf.MeasurementStatistics.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.MeasurementStatistics} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics}
 */
proto.JFJochProtoBuf.MeasurementStatistics.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setImagesCollected(value);
      break;
    case 2:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCollectionEfficiency(value);
      break;
    case 3:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setCompressionRatio(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt64());
      msg.setRunNumber(value);
      break;
    case 5:
      var value = /** @type {string} */ (reader.readString());
      msg.setFileName(value);
      break;
    case 6:
      var value = /** @type {number} */ (reader.readDouble());
      msg.setWriterPerformanceMbs(value);
      break;
    case 7:
      var value = /** @type {number} */ (reader.readUint64());
      msg.setImagesWritten(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.MeasurementStatistics.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.MeasurementStatistics} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.MeasurementStatistics.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getImagesCollected();
  if (f !== 0) {
    writer.writeUint64(
      1,
      f
    );
  }
  f = message.getCollectionEfficiency();
  if (f !== 0.0) {
    writer.writeDouble(
      2,
      f
    );
  }
  f = message.getCompressionRatio();
  if (f !== 0.0) {
    writer.writeDouble(
      3,
      f
    );
  }
  f = /** @type {number} */ (jspb.Message.getField(message, 4));
  if (f != null) {
    writer.writeInt64(
      4,
      f
    );
  }
  f = message.getFileName();
  if (f.length > 0) {
    writer.writeString(
      5,
      f
    );
  }
  f = message.getWriterPerformanceMbs();
  if (f !== 0.0) {
    writer.writeDouble(
      6,
      f
    );
  }
  f = message.getImagesWritten();
  if (f !== 0) {
    writer.writeUint64(
      7,
      f
    );
  }
};


/**
 * optional uint64 images_collected = 1;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getImagesCollected = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setImagesCollected = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional double collection_efficiency = 2;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getCollectionEfficiency = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 2, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setCollectionEfficiency = function(value) {
  return jspb.Message.setProto3FloatField(this, 2, value);
};


/**
 * optional double compression_ratio = 3;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getCompressionRatio = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 3, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setCompressionRatio = function(value) {
  return jspb.Message.setProto3FloatField(this, 3, value);
};


/**
 * optional int64 run_number = 4;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getRunNumber = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setRunNumber = function(value) {
  return jspb.Message.setField(this, 4, value);
};


/**
 * Clears the field making it undefined.
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.clearRunNumber = function() {
  return jspb.Message.setField(this, 4, undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.hasRunNumber = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional string file_name = 5;
 * @return {string}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getFileName = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 5, ""));
};


/**
 * @param {string} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setFileName = function(value) {
  return jspb.Message.setProto3StringField(this, 5, value);
};


/**
 * optional double writer_performance_MBs = 6;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getWriterPerformanceMbs = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 6, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setWriterPerformanceMbs = function(value) {
  return jspb.Message.setProto3FloatField(this, 6, value);
};


/**
 * optional uint64 images_written = 7;
 * @return {number}
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.getImagesWritten = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 7, 0));
};


/**
 * @param {number} value
 * @return {!proto.JFJochProtoBuf.MeasurementStatistics} returns this
 */
proto.JFJochProtoBuf.MeasurementStatistics.prototype.setImagesWritten = function(value) {
  return jspb.Message.setProto3IntField(this, 7, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BrokerStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BrokerStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    currentState: jspb.Message.getFieldWithDefault(msg, 1, 0),
    receiverStatus: (f = msg.getReceiverStatus()) && proto.JFJochProtoBuf.ReceiverStatus.toObject(includeInstance, f),
    indexerStatus: (f = msg.getIndexerStatus()) && proto.JFJochProtoBuf.IndexerStatus.toObject(includeInstance, f),
    measurementStatistics: (f = msg.getMeasurementStatistics()) && proto.JFJochProtoBuf.MeasurementStatistics.toObject(includeInstance, f),
    calibrationStatistics: (f = msg.getCalibrationStatistics()) && proto.JFJochProtoBuf.JFCalibrationStatistics.toObject(includeInstance, f),
    detectorState: (f = msg.getDetectorState()) && proto.JFJochProtoBuf.DetectorStatus.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BrokerStatus}
 */
proto.JFJochProtoBuf.BrokerStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BrokerStatus;
  return proto.JFJochProtoBuf.BrokerStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BrokerStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BrokerStatus}
 */
proto.JFJochProtoBuf.BrokerStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.JFJochProtoBuf.BrokerStatus.State} */ (reader.readEnum());
      msg.setCurrentState(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.ReceiverStatus;
      reader.readMessage(value,proto.JFJochProtoBuf.ReceiverStatus.deserializeBinaryFromReader);
      msg.setReceiverStatus(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.IndexerStatus;
      reader.readMessage(value,proto.JFJochProtoBuf.IndexerStatus.deserializeBinaryFromReader);
      msg.setIndexerStatus(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.MeasurementStatistics;
      reader.readMessage(value,proto.JFJochProtoBuf.MeasurementStatistics.deserializeBinaryFromReader);
      msg.setMeasurementStatistics(value);
      break;
    case 10:
      var value = new proto.JFJochProtoBuf.JFCalibrationStatistics;
      reader.readMessage(value,proto.JFJochProtoBuf.JFCalibrationStatistics.deserializeBinaryFromReader);
      msg.setCalibrationStatistics(value);
      break;
    case 11:
      var value = new proto.JFJochProtoBuf.DetectorStatus;
      reader.readMessage(value,proto.JFJochProtoBuf.DetectorStatus.deserializeBinaryFromReader);
      msg.setDetectorState(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BrokerStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BrokerStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getCurrentState();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getReceiverStatus();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.ReceiverStatus.serializeBinaryToWriter
    );
  }
  f = message.getIndexerStatus();
  if (f != null) {
    writer.writeMessage(
      3,
      f,
      proto.JFJochProtoBuf.IndexerStatus.serializeBinaryToWriter
    );
  }
  f = message.getMeasurementStatistics();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.MeasurementStatistics.serializeBinaryToWriter
    );
  }
  f = message.getCalibrationStatistics();
  if (f != null) {
    writer.writeMessage(
      10,
      f,
      proto.JFJochProtoBuf.JFCalibrationStatistics.serializeBinaryToWriter
    );
  }
  f = message.getDetectorState();
  if (f != null) {
    writer.writeMessage(
      11,
      f,
      proto.JFJochProtoBuf.DetectorStatus.serializeBinaryToWriter
    );
  }
};


/**
 * @enum {number}
 */
proto.JFJochProtoBuf.BrokerStatus.State = {
  NOT_INITIALIZED: 0,
  IDLE: 1,
  BUSY: 2,
  PEDESTAL: 3,
  DATA_COLLECTION: 4,
  ERROR: 5
};

/**
 * optional State current_state = 1;
 * @return {!proto.JFJochProtoBuf.BrokerStatus.State}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getCurrentState = function() {
  return /** @type {!proto.JFJochProtoBuf.BrokerStatus.State} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.JFJochProtoBuf.BrokerStatus.State} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.setCurrentState = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional ReceiverStatus receiver_status = 2;
 * @return {?proto.JFJochProtoBuf.ReceiverStatus}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getReceiverStatus = function() {
  return /** @type{?proto.JFJochProtoBuf.ReceiverStatus} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.ReceiverStatus, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.ReceiverStatus|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
*/
proto.JFJochProtoBuf.BrokerStatus.prototype.setReceiverStatus = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.clearReceiverStatus = function() {
  return this.setReceiverStatus(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.hasReceiverStatus = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional IndexerStatus indexer_status = 3;
 * @return {?proto.JFJochProtoBuf.IndexerStatus}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getIndexerStatus = function() {
  return /** @type{?proto.JFJochProtoBuf.IndexerStatus} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.IndexerStatus, 3));
};


/**
 * @param {?proto.JFJochProtoBuf.IndexerStatus|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
*/
proto.JFJochProtoBuf.BrokerStatus.prototype.setIndexerStatus = function(value) {
  return jspb.Message.setWrapperField(this, 3, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.clearIndexerStatus = function() {
  return this.setIndexerStatus(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.hasIndexerStatus = function() {
  return jspb.Message.getField(this, 3) != null;
};


/**
 * optional MeasurementStatistics measurement_statistics = 4;
 * @return {?proto.JFJochProtoBuf.MeasurementStatistics}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getMeasurementStatistics = function() {
  return /** @type{?proto.JFJochProtoBuf.MeasurementStatistics} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.MeasurementStatistics, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.MeasurementStatistics|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
*/
proto.JFJochProtoBuf.BrokerStatus.prototype.setMeasurementStatistics = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.clearMeasurementStatistics = function() {
  return this.setMeasurementStatistics(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.hasMeasurementStatistics = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * optional JFCalibrationStatistics calibration_statistics = 10;
 * @return {?proto.JFJochProtoBuf.JFCalibrationStatistics}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getCalibrationStatistics = function() {
  return /** @type{?proto.JFJochProtoBuf.JFCalibrationStatistics} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.JFCalibrationStatistics, 10));
};


/**
 * @param {?proto.JFJochProtoBuf.JFCalibrationStatistics|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
*/
proto.JFJochProtoBuf.BrokerStatus.prototype.setCalibrationStatistics = function(value) {
  return jspb.Message.setWrapperField(this, 10, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.clearCalibrationStatistics = function() {
  return this.setCalibrationStatistics(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.hasCalibrationStatistics = function() {
  return jspb.Message.getField(this, 10) != null;
};


/**
 * optional DetectorStatus detector_state = 11;
 * @return {?proto.JFJochProtoBuf.DetectorStatus}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.getDetectorState = function() {
  return /** @type{?proto.JFJochProtoBuf.DetectorStatus} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.DetectorStatus, 11));
};


/**
 * @param {?proto.JFJochProtoBuf.DetectorStatus|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
*/
proto.JFJochProtoBuf.BrokerStatus.prototype.setDetectorState = function(value) {
  return jspb.Message.setWrapperField(this, 11, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerStatus} returns this
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.clearDetectorState = function() {
  return this.setDetectorState(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerStatus.prototype.hasDetectorState = function() {
  return jspb.Message.getField(this, 11) != null;
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.JFJochProtoBuf.BrokerFullStatus.repeatedFields_ = [3];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.toObject = function(opt_includeInstance) {
  return proto.JFJochProtoBuf.BrokerFullStatus.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.JFJochProtoBuf.BrokerFullStatus} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerFullStatus.toObject = function(includeInstance, msg) {
  var f, obj = {
    receiver: (f = msg.getReceiver()) && proto.JFJochProtoBuf.ReceiverOutput.toObject(includeInstance, f),
    detector: (f = msg.getDetector()) && proto.JFJochProtoBuf.DetectorOutput.toObject(includeInstance, f),
    writerList: jspb.Message.toObjectList(msg.getWriterList(),
    proto.JFJochProtoBuf.WriterOutput.toObject, includeInstance),
    indexer: (f = msg.getIndexer()) && proto.JFJochProtoBuf.IndexerOutput.toObject(includeInstance, f)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus}
 */
proto.JFJochProtoBuf.BrokerFullStatus.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.JFJochProtoBuf.BrokerFullStatus;
  return proto.JFJochProtoBuf.BrokerFullStatus.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.JFJochProtoBuf.BrokerFullStatus} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus}
 */
proto.JFJochProtoBuf.BrokerFullStatus.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = new proto.JFJochProtoBuf.ReceiverOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.ReceiverOutput.deserializeBinaryFromReader);
      msg.setReceiver(value);
      break;
    case 2:
      var value = new proto.JFJochProtoBuf.DetectorOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.DetectorOutput.deserializeBinaryFromReader);
      msg.setDetector(value);
      break;
    case 3:
      var value = new proto.JFJochProtoBuf.WriterOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.WriterOutput.deserializeBinaryFromReader);
      msg.addWriter(value);
      break;
    case 4:
      var value = new proto.JFJochProtoBuf.IndexerOutput;
      reader.readMessage(value,proto.JFJochProtoBuf.IndexerOutput.deserializeBinaryFromReader);
      msg.setIndexer(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.JFJochProtoBuf.BrokerFullStatus.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.JFJochProtoBuf.BrokerFullStatus} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.JFJochProtoBuf.BrokerFullStatus.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getReceiver();
  if (f != null) {
    writer.writeMessage(
      1,
      f,
      proto.JFJochProtoBuf.ReceiverOutput.serializeBinaryToWriter
    );
  }
  f = message.getDetector();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      proto.JFJochProtoBuf.DetectorOutput.serializeBinaryToWriter
    );
  }
  f = message.getWriterList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      3,
      f,
      proto.JFJochProtoBuf.WriterOutput.serializeBinaryToWriter
    );
  }
  f = message.getIndexer();
  if (f != null) {
    writer.writeMessage(
      4,
      f,
      proto.JFJochProtoBuf.IndexerOutput.serializeBinaryToWriter
    );
  }
};


/**
 * optional ReceiverOutput receiver = 1;
 * @return {?proto.JFJochProtoBuf.ReceiverOutput}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.getReceiver = function() {
  return /** @type{?proto.JFJochProtoBuf.ReceiverOutput} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.ReceiverOutput, 1));
};


/**
 * @param {?proto.JFJochProtoBuf.ReceiverOutput|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
*/
proto.JFJochProtoBuf.BrokerFullStatus.prototype.setReceiver = function(value) {
  return jspb.Message.setWrapperField(this, 1, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.clearReceiver = function() {
  return this.setReceiver(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.hasReceiver = function() {
  return jspb.Message.getField(this, 1) != null;
};


/**
 * optional DetectorOutput detector = 2;
 * @return {?proto.JFJochProtoBuf.DetectorOutput}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.getDetector = function() {
  return /** @type{?proto.JFJochProtoBuf.DetectorOutput} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.DetectorOutput, 2));
};


/**
 * @param {?proto.JFJochProtoBuf.DetectorOutput|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
*/
proto.JFJochProtoBuf.BrokerFullStatus.prototype.setDetector = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.clearDetector = function() {
  return this.setDetector(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.hasDetector = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * repeated WriterOutput writer = 3;
 * @return {!Array<!proto.JFJochProtoBuf.WriterOutput>}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.getWriterList = function() {
  return /** @type{!Array<!proto.JFJochProtoBuf.WriterOutput>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.JFJochProtoBuf.WriterOutput, 3));
};


/**
 * @param {!Array<!proto.JFJochProtoBuf.WriterOutput>} value
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
*/
proto.JFJochProtoBuf.BrokerFullStatus.prototype.setWriterList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 3, value);
};


/**
 * @param {!proto.JFJochProtoBuf.WriterOutput=} opt_value
 * @param {number=} opt_index
 * @return {!proto.JFJochProtoBuf.WriterOutput}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.addWriter = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 3, opt_value, proto.JFJochProtoBuf.WriterOutput, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.clearWriterList = function() {
  return this.setWriterList([]);
};


/**
 * optional IndexerOutput indexer = 4;
 * @return {?proto.JFJochProtoBuf.IndexerOutput}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.getIndexer = function() {
  return /** @type{?proto.JFJochProtoBuf.IndexerOutput} */ (
    jspb.Message.getWrapperField(this, proto.JFJochProtoBuf.IndexerOutput, 4));
};


/**
 * @param {?proto.JFJochProtoBuf.IndexerOutput|undefined} value
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
*/
proto.JFJochProtoBuf.BrokerFullStatus.prototype.setIndexer = function(value) {
  return jspb.Message.setWrapperField(this, 4, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.JFJochProtoBuf.BrokerFullStatus} returns this
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.clearIndexer = function() {
  return this.setIndexer(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.JFJochProtoBuf.BrokerFullStatus.prototype.hasIndexer = function() {
  return jspb.Message.getField(this, 4) != null;
};


/**
 * @enum {number}
 */
proto.JFJochProtoBuf.Compression = {
  BSHUF_LZ4: 0,
  BSHUF_ZSTD: 1,
  NO_COMPRESSION: 2
};

/**
 * @enum {number}
 */
proto.JFJochProtoBuf.DetectorType = {
  JUNGFRAU: 0,
  EIGER: 1
};

/**
 * @enum {number}
 */
proto.JFJochProtoBuf.DetectorMode = {
  CONVERSION: 0,
  RAW: 1,
  PEDESTAL_G0: 2,
  PEDESTAL_G1: 3,
  PEDESTAL_G2: 4
};

/**
 * @enum {number}
 */
proto.JFJochProtoBuf.FPGAFIFOStatus = {
  EMPTY: 0,
  FULL: 1,
  PARTIAL: 2
};

/**
 * @enum {number}
 */
proto.JFJochProtoBuf.WriterFrameType = {
  END_ACQUISITION: 0,
  START_ACQUISITION: 1,
  IMAGE: 2
};

/**
 * @enum {number}
 */
proto.JFJochProtoBuf.DetectorStatusEnum = {
  IDLE: 0,
  ERROR: 1,
  BUSY: 2,
  NO_DETECTOR: 3
};

goog.object.extend(exports, proto.JFJochProtoBuf);
