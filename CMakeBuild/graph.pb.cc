// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: graph.proto

#include "graph.pb.h"

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

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace serializator {
PROTOBUF_CONSTEXPR RouteSettings::RouteSettings(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.bus_velocity_)*/0
  , /*decltype(_impl_.bus_wait_time_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct RouteSettingsDefaultTypeInternal {
  PROTOBUF_CONSTEXPR RouteSettingsDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~RouteSettingsDefaultTypeInternal() {}
  union {
    RouteSettings _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 RouteSettingsDefaultTypeInternal _RouteSettings_default_instance_;
}  // namespace serializator
static ::_pb::Metadata file_level_metadata_graph_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_graph_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_graph_2eproto = nullptr;

const uint32_t TableStruct_graph_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::serializator::RouteSettings, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::serializator::RouteSettings, _impl_.bus_velocity_),
  PROTOBUF_FIELD_OFFSET(::serializator::RouteSettings, _impl_.bus_wait_time_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::serializator::RouteSettings)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::serializator::_RouteSettings_default_instance_._instance,
};

const char descriptor_table_protodef_graph_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013graph.proto\022\014serializator\032\026transport_r"
  "outer.proto\"<\n\rRouteSettings\022\024\n\014bus_velo"
  "city\030\001 \001(\001\022\025\n\rbus_wait_time\030\002 \001(\001b\006proto"
  "3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_graph_2eproto_deps[1] = {
  &::descriptor_table_transport_5frouter_2eproto,
};
static ::_pbi::once_flag descriptor_table_graph_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_graph_2eproto = {
    false, false, 121, descriptor_table_protodef_graph_2eproto,
    "graph.proto",
    &descriptor_table_graph_2eproto_once, descriptor_table_graph_2eproto_deps, 1, 1,
    schemas, file_default_instances, TableStruct_graph_2eproto::offsets,
    file_level_metadata_graph_2eproto, file_level_enum_descriptors_graph_2eproto,
    file_level_service_descriptors_graph_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_graph_2eproto_getter() {
  return &descriptor_table_graph_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_graph_2eproto(&descriptor_table_graph_2eproto);
namespace serializator {

// ===================================================================

class RouteSettings::_Internal {
 public:
};

RouteSettings::RouteSettings(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:serializator.RouteSettings)
}
RouteSettings::RouteSettings(const RouteSettings& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  RouteSettings* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.bus_velocity_){}
    , decltype(_impl_.bus_wait_time_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.bus_velocity_, &from._impl_.bus_velocity_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.bus_wait_time_) -
    reinterpret_cast<char*>(&_impl_.bus_velocity_)) + sizeof(_impl_.bus_wait_time_));
  // @@protoc_insertion_point(copy_constructor:serializator.RouteSettings)
}

inline void RouteSettings::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.bus_velocity_){0}
    , decltype(_impl_.bus_wait_time_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

RouteSettings::~RouteSettings() {
  // @@protoc_insertion_point(destructor:serializator.RouteSettings)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void RouteSettings::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void RouteSettings::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void RouteSettings::Clear() {
// @@protoc_insertion_point(message_clear_start:serializator.RouteSettings)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&_impl_.bus_velocity_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.bus_wait_time_) -
      reinterpret_cast<char*>(&_impl_.bus_velocity_)) + sizeof(_impl_.bus_wait_time_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RouteSettings::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // double bus_velocity = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 9)) {
          _impl_.bus_velocity_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // double bus_wait_time = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 17)) {
          _impl_.bus_wait_time_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* RouteSettings::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:serializator.RouteSettings)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // double bus_velocity = 1;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_velocity = this->_internal_bus_velocity();
  uint64_t raw_bus_velocity;
  memcpy(&raw_bus_velocity, &tmp_bus_velocity, sizeof(tmp_bus_velocity));
  if (raw_bus_velocity != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(1, this->_internal_bus_velocity(), target);
  }

  // double bus_wait_time = 2;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_wait_time = this->_internal_bus_wait_time();
  uint64_t raw_bus_wait_time;
  memcpy(&raw_bus_wait_time, &tmp_bus_wait_time, sizeof(tmp_bus_wait_time));
  if (raw_bus_wait_time != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(2, this->_internal_bus_wait_time(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:serializator.RouteSettings)
  return target;
}

size_t RouteSettings::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:serializator.RouteSettings)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // double bus_velocity = 1;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_velocity = this->_internal_bus_velocity();
  uint64_t raw_bus_velocity;
  memcpy(&raw_bus_velocity, &tmp_bus_velocity, sizeof(tmp_bus_velocity));
  if (raw_bus_velocity != 0) {
    total_size += 1 + 8;
  }

  // double bus_wait_time = 2;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_wait_time = this->_internal_bus_wait_time();
  uint64_t raw_bus_wait_time;
  memcpy(&raw_bus_wait_time, &tmp_bus_wait_time, sizeof(tmp_bus_wait_time));
  if (raw_bus_wait_time != 0) {
    total_size += 1 + 8;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RouteSettings::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    RouteSettings::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RouteSettings::GetClassData() const { return &_class_data_; }


void RouteSettings::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<RouteSettings*>(&to_msg);
  auto& from = static_cast<const RouteSettings&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:serializator.RouteSettings)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_velocity = from._internal_bus_velocity();
  uint64_t raw_bus_velocity;
  memcpy(&raw_bus_velocity, &tmp_bus_velocity, sizeof(tmp_bus_velocity));
  if (raw_bus_velocity != 0) {
    _this->_internal_set_bus_velocity(from._internal_bus_velocity());
  }
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_bus_wait_time = from._internal_bus_wait_time();
  uint64_t raw_bus_wait_time;
  memcpy(&raw_bus_wait_time, &tmp_bus_wait_time, sizeof(tmp_bus_wait_time));
  if (raw_bus_wait_time != 0) {
    _this->_internal_set_bus_wait_time(from._internal_bus_wait_time());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RouteSettings::CopyFrom(const RouteSettings& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:serializator.RouteSettings)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RouteSettings::IsInitialized() const {
  return true;
}

void RouteSettings::InternalSwap(RouteSettings* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(RouteSettings, _impl_.bus_wait_time_)
      + sizeof(RouteSettings::_impl_.bus_wait_time_)
      - PROTOBUF_FIELD_OFFSET(RouteSettings, _impl_.bus_velocity_)>(
          reinterpret_cast<char*>(&_impl_.bus_velocity_),
          reinterpret_cast<char*>(&other->_impl_.bus_velocity_));
}

::PROTOBUF_NAMESPACE_ID::Metadata RouteSettings::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_graph_2eproto_getter, &descriptor_table_graph_2eproto_once,
      file_level_metadata_graph_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace serializator
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::serializator::RouteSettings*
Arena::CreateMaybeMessage< ::serializator::RouteSettings >(Arena* arena) {
  return Arena::CreateMessageInternal< ::serializator::RouteSettings >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
