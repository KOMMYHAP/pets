// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Main.proto

#include "Main.pb.h"

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
class MousePositionMessageRequestDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<MousePositionMessageRequest> _instance;
} _MousePositionMessageRequest_default_instance_;
class MousePositionMessageResponseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<MousePositionMessageResponse> _instance;
} _MousePositionMessageResponse_default_instance_;
}  // namespace package
}  // namespace proto
}  // namespace my
static void InitDefaultsscc_info_MousePositionMessageRequest_Main_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::my::proto::package::_MousePositionMessageRequest_default_instance_;
    new (ptr) ::my::proto::package::MousePositionMessageRequest();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_MousePositionMessageRequest_Main_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_MousePositionMessageRequest_Main_2eproto}, {}};

static void InitDefaultsscc_info_MousePositionMessageResponse_Main_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::my::proto::package::_MousePositionMessageResponse_default_instance_;
    new (ptr) ::my::proto::package::MousePositionMessageResponse();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_MousePositionMessageResponse_Main_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_MousePositionMessageResponse_Main_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_Main_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_Main_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_Main_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_Main_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::my::proto::package::MousePositionMessageRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::my::proto::package::MousePositionMessageResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::my::proto::package::MousePositionMessageResponse, x_),
  PROTOBUF_FIELD_OFFSET(::my::proto::package::MousePositionMessageResponse, y_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::my::proto::package::MousePositionMessageRequest)},
  { 5, -1, sizeof(::my::proto::package::MousePositionMessageResponse)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::my::proto::package::_MousePositionMessageRequest_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::my::proto::package::_MousePositionMessageResponse_default_instance_),
};

const char descriptor_table_protodef_Main_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nMain.proto\022\020my.proto.package\032\017PacketsI"
  "d.proto\"#\n\033MousePositionMessageRequest:\004"
  "\200\265\030d\":\n\034MousePositionMessageResponse\022\t\n\001"
  "x\030\002 \001(\005\022\t\n\001y\030\003 \001(\005:\004\200\265\030eb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_Main_2eproto_deps[1] = {
  &::descriptor_table_PacketsId_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_Main_2eproto_sccs[2] = {
  &scc_info_MousePositionMessageRequest_Main_2eproto.base,
  &scc_info_MousePositionMessageResponse_Main_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_Main_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Main_2eproto = {
  false, false, descriptor_table_protodef_Main_2eproto, "Main.proto", 152,
  &descriptor_table_Main_2eproto_once, descriptor_table_Main_2eproto_sccs, descriptor_table_Main_2eproto_deps, 2, 1,
  schemas, file_default_instances, TableStruct_Main_2eproto::offsets,
  file_level_metadata_Main_2eproto, 2, file_level_enum_descriptors_Main_2eproto, file_level_service_descriptors_Main_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_Main_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_Main_2eproto)), true);
namespace my {
namespace proto {
namespace package {

// ===================================================================

class MousePositionMessageRequest::_Internal {
 public:
};

MousePositionMessageRequest::MousePositionMessageRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:my.proto.package.MousePositionMessageRequest)
}
MousePositionMessageRequest::MousePositionMessageRequest(const MousePositionMessageRequest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:my.proto.package.MousePositionMessageRequest)
}

void MousePositionMessageRequest::SharedCtor() {
}

MousePositionMessageRequest::~MousePositionMessageRequest() {
  // @@protoc_insertion_point(destructor:my.proto.package.MousePositionMessageRequest)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void MousePositionMessageRequest::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void MousePositionMessageRequest::ArenaDtor(void* object) {
  MousePositionMessageRequest* _this = reinterpret_cast< MousePositionMessageRequest* >(object);
  (void)_this;
}
void MousePositionMessageRequest::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void MousePositionMessageRequest::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const MousePositionMessageRequest& MousePositionMessageRequest::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_MousePositionMessageRequest_Main_2eproto.base);
  return *internal_default_instance();
}


void MousePositionMessageRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:my.proto.package.MousePositionMessageRequest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* MousePositionMessageRequest::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* MousePositionMessageRequest::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:my.proto.package.MousePositionMessageRequest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:my.proto.package.MousePositionMessageRequest)
  return target;
}

size_t MousePositionMessageRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:my.proto.package.MousePositionMessageRequest)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void MousePositionMessageRequest::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:my.proto.package.MousePositionMessageRequest)
  GOOGLE_DCHECK_NE(&from, this);
  const MousePositionMessageRequest* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<MousePositionMessageRequest>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:my.proto.package.MousePositionMessageRequest)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:my.proto.package.MousePositionMessageRequest)
    MergeFrom(*source);
  }
}

void MousePositionMessageRequest::MergeFrom(const MousePositionMessageRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:my.proto.package.MousePositionMessageRequest)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void MousePositionMessageRequest::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:my.proto.package.MousePositionMessageRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MousePositionMessageRequest::CopyFrom(const MousePositionMessageRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:my.proto.package.MousePositionMessageRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MousePositionMessageRequest::IsInitialized() const {
  return true;
}

void MousePositionMessageRequest::InternalSwap(MousePositionMessageRequest* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata MousePositionMessageRequest::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class MousePositionMessageResponse::_Internal {
 public:
};

MousePositionMessageResponse::MousePositionMessageResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:my.proto.package.MousePositionMessageResponse)
}
MousePositionMessageResponse::MousePositionMessageResponse(const MousePositionMessageResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&x_, &from.x_,
    static_cast<size_t>(reinterpret_cast<char*>(&y_) -
    reinterpret_cast<char*>(&x_)) + sizeof(y_));
  // @@protoc_insertion_point(copy_constructor:my.proto.package.MousePositionMessageResponse)
}

void MousePositionMessageResponse::SharedCtor() {
  ::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
      reinterpret_cast<char*>(&x_) - reinterpret_cast<char*>(this)),
      0, static_cast<size_t>(reinterpret_cast<char*>(&y_) -
      reinterpret_cast<char*>(&x_)) + sizeof(y_));
}

MousePositionMessageResponse::~MousePositionMessageResponse() {
  // @@protoc_insertion_point(destructor:my.proto.package.MousePositionMessageResponse)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void MousePositionMessageResponse::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void MousePositionMessageResponse::ArenaDtor(void* object) {
  MousePositionMessageResponse* _this = reinterpret_cast< MousePositionMessageResponse* >(object);
  (void)_this;
}
void MousePositionMessageResponse::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void MousePositionMessageResponse::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const MousePositionMessageResponse& MousePositionMessageResponse::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_MousePositionMessageResponse_Main_2eproto.base);
  return *internal_default_instance();
}


void MousePositionMessageResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:my.proto.package.MousePositionMessageResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&x_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&y_) -
      reinterpret_cast<char*>(&x_)) + sizeof(y_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* MousePositionMessageResponse::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 x = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          x_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 y = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          y_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* MousePositionMessageResponse::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:my.proto.package.MousePositionMessageResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 x = 2;
  if (this->x() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_x(), target);
  }

  // int32 y = 3;
  if (this->y() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_y(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:my.proto.package.MousePositionMessageResponse)
  return target;
}

size_t MousePositionMessageResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:my.proto.package.MousePositionMessageResponse)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 x = 2;
  if (this->x() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_x());
  }

  // int32 y = 3;
  if (this->y() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_y());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void MousePositionMessageResponse::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:my.proto.package.MousePositionMessageResponse)
  GOOGLE_DCHECK_NE(&from, this);
  const MousePositionMessageResponse* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<MousePositionMessageResponse>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:my.proto.package.MousePositionMessageResponse)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:my.proto.package.MousePositionMessageResponse)
    MergeFrom(*source);
  }
}

void MousePositionMessageResponse::MergeFrom(const MousePositionMessageResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:my.proto.package.MousePositionMessageResponse)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.x() != 0) {
    _internal_set_x(from._internal_x());
  }
  if (from.y() != 0) {
    _internal_set_y(from._internal_y());
  }
}

void MousePositionMessageResponse::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:my.proto.package.MousePositionMessageResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MousePositionMessageResponse::CopyFrom(const MousePositionMessageResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:my.proto.package.MousePositionMessageResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MousePositionMessageResponse::IsInitialized() const {
  return true;
}

void MousePositionMessageResponse::InternalSwap(MousePositionMessageResponse* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(MousePositionMessageResponse, y_)
      + sizeof(MousePositionMessageResponse::y_)
      - PROTOBUF_FIELD_OFFSET(MousePositionMessageResponse, x_)>(
          reinterpret_cast<char*>(&x_),
          reinterpret_cast<char*>(&other->x_));
}

::PROTOBUF_NAMESPACE_ID::Metadata MousePositionMessageResponse::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace package
}  // namespace proto
}  // namespace my
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::my::proto::package::MousePositionMessageRequest* Arena::CreateMaybeMessage< ::my::proto::package::MousePositionMessageRequest >(Arena* arena) {
  return Arena::CreateMessageInternal< ::my::proto::package::MousePositionMessageRequest >(arena);
}
template<> PROTOBUF_NOINLINE ::my::proto::package::MousePositionMessageResponse* Arena::CreateMaybeMessage< ::my::proto::package::MousePositionMessageResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::my::proto::package::MousePositionMessageResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
