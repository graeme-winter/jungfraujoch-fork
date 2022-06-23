/**
 * @fileoverview gRPC-Web generated client stub for JFJochProtoBuf
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');

const proto = {};
proto.JFJochProtoBuf = require('./jfjoch_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.BrokerSetup,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Start = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Start',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.BrokerSetup,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.BrokerSetup} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.BrokerSetup} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.start =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Start,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.BrokerSetup} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.start =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Start);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Stop = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Stop',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.stop =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Stop,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.stop =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Stop);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Abort = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Abort',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.abort =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Abort,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.abort =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Abort);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Cancel = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Cancel',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.cancel =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Cancel',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Cancel,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.cancel =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Cancel',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Cancel);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Pedestal = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Pedestal',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.pedestal =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Pedestal',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Pedestal,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.pedestal =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Pedestal',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Pedestal);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Initialize = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Initialize',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.initialize =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Initialize',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Initialize,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.initialize =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Initialize',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Initialize);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_Deactivate = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/Deactivate',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.deactivate =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Deactivate',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Deactivate,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.deactivate =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/Deactivate',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_Deactivate);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.BrokerStatus>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetStatus = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetStatus',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.BrokerStatus,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.BrokerStatus.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.BrokerStatus)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.BrokerStatus>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getStatus =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetStatus,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.BrokerStatus>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getStatus =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetStatus);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.JFCalibration>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetCalibration = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetCalibration',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.JFCalibration,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.JFCalibration.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.JFCalibration)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.JFCalibration>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getCalibration =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetCalibration',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetCalibration,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.JFCalibration>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getCalibration =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetCalibration',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetCalibration);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.JFJochReceiverOutput>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetDetailedReceiverOutput = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetDetailedReceiverOutput',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.JFJochReceiverOutput,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.JFJochReceiverOutput.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.JFJochReceiverOutput)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.JFJochReceiverOutput>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getDetailedReceiverOutput =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetDetailedReceiverOutput',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetDetailedReceiverOutput,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.JFJochReceiverOutput>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getDetailedReceiverOutput =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetDetailedReceiverOutput',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetDetailedReceiverOutput);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.DataProcessingSettings>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetDataProcessingSettings = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetDataProcessingSettings',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.DataProcessingSettings,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.DataProcessingSettings.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.DataProcessingSettings)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.DataProcessingSettings>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getDataProcessingSettings =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetDataProcessingSettings,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.DataProcessingSettings>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getDataProcessingSettings =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetDataProcessingSettings);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.DataProcessingSettings,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_SetDataProcessingSettings = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/SetDataProcessingSettings',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.DataProcessingSettings,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.setDataProcessingSettings =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/SetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_SetDataProcessingSettings,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.setDataProcessingSettings =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/SetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_SetDataProcessingSettings);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.MaskToLoad,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochBroker_LoadMask = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/LoadMask',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.MaskToLoad,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.MaskToLoad} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.MaskToLoad} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.loadMask =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/LoadMask',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_LoadMask,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.MaskToLoad} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.loadMask =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/LoadMask',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_LoadMask);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Image>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetMask = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetMask',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Image,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Image.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Image)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Image>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getMask =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetMask',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetMask,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Image>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getMask =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetMask',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetMask);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Image>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetMaskRawCoord = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetMaskRawCoord',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Image,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Image.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Image)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Image>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getMaskRawCoord =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetMaskRawCoord',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetMaskRawCoord,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Image>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getMaskRawCoord =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetMaskRawCoord',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetMaskRawCoord);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Image>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetPedestalG0 = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG0',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Image,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Image.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Image)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Image>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getPedestalG0 =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG0',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG0,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Image>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getPedestalG0 =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG0',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG0);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Image>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetPedestalG1 = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG1',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Image,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Image.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Image)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Image>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getPedestalG1 =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG1',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG1,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Image>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getPedestalG1 =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG1',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG1);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Image>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetPedestalG2 = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG2',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Image,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Image.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Image)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Image>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getPedestalG2 =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG2',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG2,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Image>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getPedestalG2 =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPedestalG2',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPedestalG2);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.PreviewFrame>}
 */
const methodDescriptor_gRPC_JFJochBroker_GetPreviewFrame = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochBroker/GetPreviewFrame',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.PreviewFrame,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.PreviewFrame.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.PreviewFrame)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.PreviewFrame>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerClient.prototype.getPreviewFrame =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPreviewFrame',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPreviewFrame,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.PreviewFrame>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochBrokerPromiseClient.prototype.getPreviewFrame =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochBroker/GetPreviewFrame',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochBroker_GetPreviewFrame);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochReceiverInput,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochReceiver_Start = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/Start',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochReceiverInput,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochReceiverInput} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochReceiverInput} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.start =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Start,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochReceiverInput} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.start =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Start);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochReceiver_Abort = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/Abort',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.abort =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Abort,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.abort =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Abort);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochReceiver_Cancel = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/Cancel',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.cancel =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Cancel',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Cancel,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.cancel =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Cancel',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Cancel);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.JFJochReceiverOutput>}
 */
const methodDescriptor_gRPC_JFJochReceiver_Stop = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/Stop',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.JFJochReceiverOutput,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.JFJochReceiverOutput.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.JFJochReceiverOutput)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.JFJochReceiverOutput>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.stop =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Stop,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.JFJochReceiverOutput>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.stop =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_Stop);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.ReceiverStatus>}
 */
const methodDescriptor_gRPC_JFJochReceiver_GetStatus = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/GetStatus',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.ReceiverStatus,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.ReceiverStatus.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.ReceiverStatus)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.ReceiverStatus>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.getStatus =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetStatus,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.ReceiverStatus>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.getStatus =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetStatus);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.DataProcessingSettings,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochReceiver_SetDataProcessingSettings = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/SetDataProcessingSettings',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.DataProcessingSettings,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.setDataProcessingSettings =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/SetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_SetDataProcessingSettings,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.DataProcessingSettings} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.setDataProcessingSettings =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/SetDataProcessingSettings',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_SetDataProcessingSettings);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.PreviewFrame>}
 */
const methodDescriptor_gRPC_JFJochReceiver_GetPreviewFrame = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/GetPreviewFrame',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.PreviewFrame,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.PreviewFrame.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.PreviewFrame)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.PreviewFrame>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.getPreviewFrame =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetPreviewFrame',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetPreviewFrame,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.PreviewFrame>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.getPreviewFrame =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetPreviewFrame',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetPreviewFrame);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.JFJochReceiverNetworkConfig>}
 */
const methodDescriptor_gRPC_JFJochReceiver_GetNetworkConfig = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochReceiver/GetNetworkConfig',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.JFJochReceiverNetworkConfig,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.JFJochReceiverNetworkConfig.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.JFJochReceiverNetworkConfig)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.JFJochReceiverNetworkConfig>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverClient.prototype.getNetworkConfig =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetNetworkConfig',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetNetworkConfig,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.JFJochReceiverNetworkConfig>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochReceiverPromiseClient.prototype.getNetworkConfig =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochReceiver/GetNetworkConfig',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochReceiver_GetNetworkConfig);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochWriterInput,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochWriter_Start = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochWriter/Start',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochWriterInput,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochWriterInput} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochWriterInput} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterClient.prototype.start =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Start,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochWriterInput} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterPromiseClient.prototype.start =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Start);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochWriter_Abort = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochWriter/Abort',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterClient.prototype.abort =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Abort,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterPromiseClient.prototype.abort =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Abort',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Abort);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochWriter_Stop = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochWriter/Stop',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterClient.prototype.stop =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Stop,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterPromiseClient.prototype.stop =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_Stop);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochReceiverOutput,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochWriter_WriteMasterFile = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochWriter/WriteMasterFile',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochReceiverOutput,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochReceiverOutput} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochReceiverOutput} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterClient.prototype.writeMasterFile =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/WriteMasterFile',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_WriteMasterFile,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochReceiverOutput} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochWriterPromiseClient.prototype.writeMasterFile =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochWriter/WriteMasterFile',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochWriter_WriteMasterFile);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochDetectorInput,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochDetector_Start = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/Start',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochDetectorInput,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochDetectorInput} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochDetectorInput} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.start =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Start,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochDetectorInput} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.start =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Start);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochDetector_Stop = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/Stop',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.stop =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Stop,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.stop =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Stop);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.DetectorStatus>}
 */
const methodDescriptor_gRPC_JFJochDetector_Status = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/Status',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.DetectorStatus,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.DetectorStatus.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.DetectorStatus)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.DetectorStatus>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.status =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Status',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Status,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.DetectorStatus>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.status =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Status',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Status);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochDetectorConfig,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochDetector_On = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/On',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochDetectorConfig,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochDetectorConfig} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochDetectorConfig} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.on =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/On',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_On,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochDetectorConfig} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.on =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/On',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_On);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochDetector_Off = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/Off',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.off =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Off',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Off,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.off =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Off',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Off);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochDetector_Trigger = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochDetector/Trigger',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorClient.prototype.trigger =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Trigger',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Trigger,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochDetectorPromiseClient.prototype.trigger =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochDetector/Trigger',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochDetector_Trigger);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochTriggerClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochTriggerPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochTrigger_Trigger = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochTrigger/Trigger',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochTriggerClient.prototype.trigger =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochTrigger/Trigger',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochTrigger_Trigger,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochTriggerPromiseClient.prototype.trigger =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochTrigger/Trigger',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochTrigger_Trigger);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.JFJochIndexerInput,
 *   !proto.JFJochProtoBuf.Empty>}
 */
const methodDescriptor_gRPC_JFJochIndexer_Start = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochIndexer/Start',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.JFJochIndexerInput,
  proto.JFJochProtoBuf.Empty,
  /**
   * @param {!proto.JFJochProtoBuf.JFJochIndexerInput} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.Empty.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.JFJochIndexerInput} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerClient.prototype.start =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_Start,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.JFJochIndexerInput} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.Empty>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerPromiseClient.prototype.start =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/Start',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_Start);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.JFJochIndexerOutput>}
 */
const methodDescriptor_gRPC_JFJochIndexer_Stop = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochIndexer/Stop',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.JFJochIndexerOutput,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.JFJochIndexerOutput.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.JFJochIndexerOutput)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.JFJochIndexerOutput>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerClient.prototype.stop =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_Stop,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.JFJochIndexerOutput>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerPromiseClient.prototype.stop =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/Stop',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_Stop);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.JFJochProtoBuf.Empty,
 *   !proto.JFJochProtoBuf.IndexerStatus>}
 */
const methodDescriptor_gRPC_JFJochIndexer_GetStatus = new grpc.web.MethodDescriptor(
  '/JFJochProtoBuf.gRPC_JFJochIndexer/GetStatus',
  grpc.web.MethodType.UNARY,
  proto.JFJochProtoBuf.Empty,
  proto.JFJochProtoBuf.IndexerStatus,
  /**
   * @param {!proto.JFJochProtoBuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.JFJochProtoBuf.IndexerStatus.deserializeBinary
);


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.JFJochProtoBuf.IndexerStatus)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.JFJochProtoBuf.IndexerStatus>|undefined}
 *     The XHR Node Readable Stream
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerClient.prototype.getStatus =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_GetStatus,
      callback);
};


/**
 * @param {!proto.JFJochProtoBuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.JFJochProtoBuf.IndexerStatus>}
 *     Promise that resolves to the response
 */
proto.JFJochProtoBuf.gRPC_JFJochIndexerPromiseClient.prototype.getStatus =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/JFJochProtoBuf.gRPC_JFJochIndexer/GetStatus',
      request,
      metadata || {},
      methodDescriptor_gRPC_JFJochIndexer_GetStatus);
};


module.exports = proto.JFJochProtoBuf;

