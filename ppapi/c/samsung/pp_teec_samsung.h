/* Copyright (c) 2017 Samsung Electronics. All rights reserved.
 */

/* From samsung/pp_teec_samsung.idl modified Mon May  8 09:40:26 2017. */

#ifndef PPAPI_C_SAMSUNG_PP_TEEC_SAMSUNG_H_
#define PPAPI_C_SAMSUNG_PP_TEEC_SAMSUNG_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

/**
 * @file
 * This file defines common structures used with the
 * <code>PPB_TEEC</code> interfaces.
 */

/**
 * @addtogroup Enums
 * @{
 */
/**
 * Return Code Constants.
 *
 * Values map directly to error codes from TEE Client API specification
 */
typedef enum {
  /**
   * The operation was successful
   */
  PP_TEEC_SUCCESS = 0x00000000,
  /**
   * Non-specific cause
   */
  PP_TEEC_ERROR_GENERIC = -65536,
  /**
   * Access privileges are not sufficient
   */
  PP_TEEC_ERROR_ACCESS_DENIED = -65535,
  /**
   * The operation was cancelled
   */
  PP_TEEC_ERROR_CANCEL = -65534,
  /**
   * Concurrent accesses caused conflict
   */
  PP_TEEC_ERROR_ACCESS_CONFLICT = -65533,
  /**
   * Too much data for the requested operation was passed
   */
  PP_TEEC_ERROR_EXCESS_DATA = -65532,
  /**
   * Input data was of invalid format
   */
  PP_TEEC_ERROR_BAD_FORMAT = -65531,
  /**
   * Input parameters were invalid
   */
  PP_TEEC_ERROR_BAD_PARAMETERS = -65530,
  /**
   * Operation is not valid in the current state
   */
  PP_TEEC_ERROR_BAD_STATE = -65529,
  /**
   * The requested data item is not found
   */
  PP_TEEC_ERROR_ITEM_NOT_FOUND = -65528,
  /**
   * The requested operation should exist but is not yet implemented
   */
  PP_TEEC_ERROR_NOT_IMPLEMENTED = -65527,
  /**
   * The requested operation is valid but is not supported in this
   * Implementation
   */
  PP_TEEC_ERROR_NOT_SUPPORTED = -65526,
  /**
   * Expected data was missing
   */
  PP_TEEC_ERROR_NO_DATA = -65525,
  /**
   * System ran out of resources
   */
  PP_TEEC_ERROR_OUT_OF_MEMORY = -65524,
  /**
   * The system is busy working on something else.
   */
  PP_TEEC_ERROR_BUSY = -65523,
  /**
   * Communication with a remote party failed.
   */
  PP_TEEC_ERROR_COMMUNICATION = -65522,
  /**
   * A security fault was detected.
   */
  PP_TEEC_ERROR_SECURITY = -65521,
  /**
   * The supplied buffer is too short for the generated output.
   */
  PP_TEEC_ERROR_SHORT_BUFFER = -65520,
  /**
   * An external event has caused a User Interface operation to be aborted.
   */
  PP_TEE_ERROR_EXTERNAL_CANCEL = -65519,
  /**
   * Targed TA panic'ed
   */
  PP_TEEC_ERROR_TARGET_DEAD = -53212,
  PP_TEE_ERROR_TARGET_DEAD = PP_TEEC_ERROR_TARGET_DEAD
} PP_TEEC_Return_Code;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TEEC_Return_Code, 4);

/**
 * Function error origins. These indicate where in the software stack
 * a particular return value originates from.
 */
typedef enum {
  /**
   * The return code is an error that originated within
   * the TEE Client API implementation.
   */
  PP_TEEC_ORIGIN_API = 0x1,
  /**
   * The return code is an error that originated within the underlying
   * communications stack linking the rich OS with the TEE.
   */
  PP_TEEC_ORIGIN_COMMS = 0x2,
  /**
   * The return code is an error that originated within the common TEE code.
   */
  PP_TEEC_ORIGIN_TEE = 0x3,
  /**
   * The return code originated within the Trusted Application code.
   * This includes the case where the return code is a success.
   */
  PP_TEEC_ORIGIN_TRUSTED_APP = 0x4,
  PP_TEEC_ORIGIN_LAST = PP_TEEC_ORIGIN_TRUSTED_APP
} PP_TEEC_Return_Origin;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TEEC_Return_Origin, 4);

/**
 * Shared Memory Control Flags
 */
typedef enum {
  /**
   * The Shared Memory can carry data from the Client Application to the
   * Trusted Application.
   */
  PP_TEEC_MEM_INPUT = 1,
  /**
   * The Shared Memory can carry data from the Trusted Application to the
   * Client Application.
   */
  PP_TEEC_MEM_OUTPUT = 2
} PP_TEEC_MemoryType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TEEC_MemoryType, 4);

/**
 * Parameter Type
 */
typedef enum {
  /**
   * The Parameter is not used
   */
  PP_TEEC_NONE = 0x00000000,
  /**
   * The Parameter is a PP_TEEC_Value tagged as input.
   */
  PP_TEEC_VALUE_INPUT = 0x00000001,
  /**
   * The Parameter is a PP_TEEC_Value tagged as output
   */
  PP_TEEC_VALUE_OUTPUT = 0x00000002,
  /**
   * The Parameter is a PP_TEEC_Value tagged as both as input and output,
   * i.e., for which both the behaviors of TEEC_VALUE_INPUT and
   * TEEC_VALUE_OUTPUT apply.
   */
  PP_TEEC_VALUE_INOUT = 0x00000003,
  /**
   * The Parameter is a PP_TEEC_TempMemoryReference describing a region of
   * memory which needs to be temporarily registered for the duration of
   * the Operation and is tagged as input.
   */
  PP_TEEC_MEMREF_TEMP_INPUT = 0x00000005,
  /**
   * Same as TEEC_MEMREF_TEMP_INPUT, but the Memory Reference is tagged
   * as output.
   */
  PP_TEEC_MEMREF_TEMP_OUTPUT = 0x00000006,
  /**
   * A Temporary Memory Reference tagged as both input and output,
   * i.e., for which both the behaviors of PP_TEEC_MEMREF_TEMP_INPUT and
   * PP_TEEC_MEMREF_TEMP_OUTPUT apply.
   */
  PP_TEEC_MEMREF_TEMP_INOUT = 0x00000007,
  /**
   * The Parameter is a Registered Memory Reference that refers
   * to the entirety of its parent Shared Memory block. The parameter
   * structure is a PP_TEEC_RegisteredMemoryReference.
   */
  PP_TEEC_MEMREF_WHOLE = 0x0000000C,
  /**
   * A Registered Memory Reference structure that refers to a partial region
   * of its parent Shared Memory block and is tagged as input. The parameter
   * structure is a PP_TEEC_RegisteredMemoryReference.
   */
  PP_TEEC_MEMREF_PARTIAL_INPUT = 0x0000000D,
  /**
   * A Registered Memory Reference structure that refers to a partial region
   * of its parent Shared Memory block and is tagged as output. The parameter
   * structure is a PP_TEEC_RegisteredMemoryReference.
   */
  PP_TEEC_MEMREF_PARTIAL_OUTPUT = 0x0000000E,
  /**
   * The Registered Memory Reference structure that refers to a partial region
   * of its parent Shared Memory block and is tagged as both input and output,
   * i.e., for which both the behaviors of TEEC_MEMREF_PARTIAL_INPUT and
   * TEEC_MEMREF_PARTIAL_OUTPUT apply. The parameter  structure is
   * a PP_TEEC_RegisteredMemoryReference.
   */
  PP_TEEC_MEMREF_PARTIAL_INOUT = 0x0000000F
} PP_TEEC_ParameterType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TEEC_ParameterType, 4);

/**
 * Session Login Methods
 */
typedef enum {
  /**
   * No login data is provided.
   */
  PP_TEEC_LOGIN_PUBLIC = 0x00000000,
  /**
   * Login data about the user running the Client Application process
   * is provided.
   */
  PP_TEEC_LOGIN_USER = 0x00000001,
  /**
   * Login data about the group running the Client Application process
   * is provided.
   */
  PP_TEEC_LOGIN_GROUP = 0x00000002,
  /**
   * Login data about the running Client Application itself is provided.
   */
  PP_TEEC_LOGIN_APPLICATION = 0x00000004,
  /**
   * Login data about the user running the Client Application and about
   * the Client Application itself is provided.
   */
  PP_TEEC_LOGIN_USER_APPLICATION = 0x00000005,
  /**
   * Login data about the group running the Client Application and about
   * the Client Application itself is provided.
   */
  PP_TEEC_LOGIN_GROUP_APPLICATION = 0x00000006
} PP_TEEC_LoginMethods;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TEEC_LoginMethods, 4);
/**
 * @}
 */

/**
 * @addtogroup Structs
 * @{
 */
/**
 * This type defines result returned from PPB_TEEC methods.
 * It consists of PP_TEEC_Return_Code and PP_TEEC_Return_Origin fields
 */
struct PP_TEEC_Result {
  int32_t return_code;
  int32_t return_origin;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TEEC_Result, 8);

/**
 * This type contains a Universally Unique Resource Identifier (UUID) type
 * as defined in RFC4122.
 * These UUID values are used to identify Trusted Applications.
 */
struct PP_TEEC_UUID {
  uint32_t time_low;
  uint16_t time_mid;
  uint16_t time_hi_and_version;
  uint8_t clock_seq_and_node[8];
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TEEC_UUID, 16);

/**
 * This type defines a Temporary Memory Reference. It is used as
 * a TEEC_Operation parameter when the corresponding parameter type
 * is one of TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_OUTPUT,
 * or TEEC_MEMREF_TEMP_INOUT.
 */
struct PP_TEEC_TempMemoryReference {
  void* buffer;
  uint32_t size;
};

/**
 * This type defines a Registered Memory Reference, i.e., that uses
 * a pre-registered or pre-allocated  Shared Memory block.
 * It is used as a TEEC_Operation parameter when the corresponding
 * parameter type is one of TEEC_MEMREF_WHOLE, TEEC_MEMREF_PARTIAL_INPUT,
 * TEEC_MEMREF_PARTIAL_OUTPUT, or TEEC_MEMREF_PARTIAL_INOUT.
 * See the PPB_TEECSharedMemory_Samsung functions Register() and Allocate()
 * for more details.
 */
struct PP_TEEC_RegisteredMemoryReference {
  PP_Resource parent;
  uint32_t offset;
  uint32_t size;
};

/**
 * This type defines a parameter that is not referencing shared memory,
 * but carries instead small raw data passed by value.
 * It is used as a TEEC_Operation parameter when the corresponding parameter
 * type is one of TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT, or TEEC_VALUE_INOUT.
 */
struct PP_TEEC_Value {
  uint32_t a;
  uint32_t b;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TEEC_Value, 8);

/**
 * This type defines a Parameter of a TEEC_Operation.
 * It can be a Temporary Memory Reference, a Registered
 * Memory Reference, or a Value Parameter.
 */
union PP_TEEC_Parameter {
  struct PP_TEEC_TempMemoryReference tmpref;
  struct PP_TEEC_RegisteredMemoryReference memref;
  struct PP_TEEC_Value value;
};

/**
 * This type defines the payload of either an open Session operation
 * or an invoke Command operation.
 * It is also used for cancellation of operations, which may be desirable
 * even if no payload is passed.
 */
struct PP_TEEC_Operation {
  uint32_t operation_id;
  uint32_t started;
  uint32_t param_types;
  union PP_TEEC_Parameter params[4];
};
/**
 * @}
 */

#endif /* PPAPI_C_SAMSUNG_PP_TEEC_SAMSUNG_H_ */
