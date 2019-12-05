project(protobuf)

include_directories(
    "${PROJECT_SOURCE_DIR}"
)

set(protobuf_source_dir protobuf)

add_library(protobuf
  ./google/protobuf/any.cc
  ./google/protobuf/any.pb.cc
  ./google/protobuf/api.pb.cc
  ./google/protobuf/compiler/importer.cc
  ./google/protobuf/compiler/parser.cc
  ./google/protobuf/descriptor.cc
  ./google/protobuf/descriptor.pb.cc
  ./google/protobuf/descriptor_database.cc
  ./google/protobuf/duration.pb.cc
  ./google/protobuf/dynamic_message.cc
  ./google/protobuf/empty.pb.cc
  ./google/protobuf/extension_set_heavy.cc
  ./google/protobuf/field_mask.pb.cc
  ./google/protobuf/generated_message_reflection.cc
  ./google/protobuf/io/gzip_stream.cc
  ./google/protobuf/io/printer.cc
  ./google/protobuf/io/strtod.cc
  ./google/protobuf/io/tokenizer.cc
  ./google/protobuf/io/zero_copy_stream_impl.cc
  ./google/protobuf/map_field.cc
  ./google/protobuf/message.cc
  ./google/protobuf/reflection_ops.cc
  ./google/protobuf/service.cc
  ./google/protobuf/source_context.pb.cc
  ./google/protobuf/struct.pb.cc
  ./google/protobuf/stubs/mathlimits.cc
  ./google/protobuf/stubs/substitute.cc
  ./google/protobuf/text_format.cc
  ./google/protobuf/timestamp.pb.cc
  ./google/protobuf/type.pb.cc
  ./google/protobuf/unknown_field_set.cc
  ./google/protobuf/util/field_comparator.cc
  ./google/protobuf/util/field_mask_util.cc
  ./google/protobuf/util/internal/datapiece.cc
  ./google/protobuf/util/internal/default_value_objectwriter.cc
  ./google/protobuf/util/internal/error_listener.cc
  ./google/protobuf/util/internal/field_mask_utility.cc
  ./google/protobuf/util/internal/json_escaping.cc
  ./google/protobuf/util/internal/json_objectwriter.cc
  ./google/protobuf/util/internal/json_stream_parser.cc
  ./google/protobuf/util/internal/object_writer.cc
  ./google/protobuf/util/internal/proto_writer.cc
  ./google/protobuf/util/internal/protostream_objectsource.cc
  ./google/protobuf/util/internal/protostream_objectwriter.cc
  ./google/protobuf/util/internal/type_info.cc
  ./google/protobuf/util/internal/type_info_test_helper.cc
  ./google/protobuf/util/internal/utility.cc
  ./google/protobuf/util/json_util.cc
  ./google/protobuf/util/message_differencer.cc
  ./google/protobuf/util/time_util.cc
  ./google/protobuf/util/type_resolver_util.cc
  ./google/protobuf/wire_format.cc
  ./google/protobuf/wrappers.pb.cc
  ./google/protobuf/arena.cc
  ./google/protobuf/arenastring.cc
  ./google/protobuf/extension_set.cc
  ./google/protobuf/generated_message_util.cc
  ./google/protobuf/io/coded_stream.cc
  ./google/protobuf/io/zero_copy_stream.cc
  ./google/protobuf/io/zero_copy_stream_impl_lite.cc
  ./google/protobuf/message_lite.cc
  ./google/protobuf/repeated_field.cc
  ./google/protobuf/stubs/atomicops_internals_x86_gcc.cc
  ./google/protobuf/stubs/atomicops_internals_x86_msvc.cc
  ./google/protobuf/stubs/bytestream.cc
  ./google/protobuf/stubs/common.cc
  ./google/protobuf/stubs/int128.cc
  ./google/protobuf/stubs/once.cc
  ./google/protobuf/stubs/status.cc
  ./google/protobuf/stubs/statusor.cc
  ./google/protobuf/stubs/stringpiece.cc
  ./google/protobuf/stubs/stringprintf.cc
  ./google/protobuf/stubs/structurally_valid.cc
  ./google/protobuf/stubs/strutil.cc
  ./google/protobuf/stubs/time.cc
  ./google/protobuf/wire_format_lite.cc
)

if(MSVC AND protobuf_BUILD_SHARED_LIBS)
  target_compile_definitions(libprotobuf
    PUBLIC  PROTOBUF_USE_DLLS
    PRIVATE LIBPROTOBUF_EXPORTS)
endif()

