// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PacketsId.proto

#include "PacketsId.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace my {
namespace proto {
namespace package {
}  // namespace package
}  // namespace proto
}  // namespace my
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_PacketsId_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_PacketsId_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_PacketsId_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_PacketsId_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_PacketsId_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017PacketsId.proto\022\020my.proto.package\032 goo"
  "gle/protobuf/descriptor.proto*\265\001\n\tPackag"
  "eId\022\026\n\022UNKNOWN_MESSAGE_ID\020\000\022\026\n\022CONNECTIO"
  "N_REQUEST\020\n\022\027\n\023CONNECTION_RESPONSE\020\013\022\023\n\017"
  "CONNECTION_PING\020\014\022\023\n\017CONNECTION_PONG\020\r\022\031"
  "\n\025CONNECTION_DISCONNECT\020\016\022\032\n\026MOUSE_POSIT"
  "ION_MESSAGE\020d:R\n\npackage_id\022\037.google.pro"
  "tobuf.MessageOptions\030\320\206\003 \001(\0162\033.my.proto."
  "package.PackageIdb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_PacketsId_2eproto_deps[1] = {
  &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_PacketsId_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_PacketsId_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_PacketsId_2eproto = {
  false, false, descriptor_table_protodef_PacketsId_2eproto, "PacketsId.proto", 345,
  &descriptor_table_PacketsId_2eproto_once, descriptor_table_PacketsId_2eproto_sccs, descriptor_table_PacketsId_2eproto_deps, 0, 1,
  schemas, file_default_instances, TableStruct_PacketsId_2eproto::offsets,
  file_level_metadata_PacketsId_2eproto, 0, file_level_enum_descriptors_PacketsId_2eproto, file_level_service_descriptors_PacketsId_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_PacketsId_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_PacketsId_2eproto)), true);
namespace my {
namespace proto {
namespace package {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PackageId_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_PacketsId_2eproto);
  return file_level_enum_descriptors_PacketsId_2eproto[0];
}
bool PackageId_IsValid(int value) {
  switch (value) {
    case 0:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 100:
      return true;
    default:
      return false;
  }
}

::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::google::protobuf::MessageOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::EnumTypeTraits< ::my::proto::package::PackageId, ::my::proto::package::PackageId_IsValid>, 14, false >
  package_id(kPackageIdFieldNumber, static_cast< ::my::proto::package::PackageId >(0));

// @@protoc_insertion_point(namespace_scope)
}  // namespace package
}  // namespace proto
}  // namespace my
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
