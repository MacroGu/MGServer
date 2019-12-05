// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_optimize_for.proto

#ifndef PROTOBUF_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto__INCLUDED
#define PROTOBUF_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000002 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/unittest.pb.h>
// @@protoc_insertion_point(includes)

namespace protobuf_unittest {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();

class TestOptimizedForSize;
class TestOptionalOptimizedForSize;
class TestRequiredOptimizedForSize;

// ===================================================================

class TestOptimizedForSize : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:protobuf_unittest.TestOptimizedForSize) */ {
 public:
  TestOptimizedForSize();
  virtual ~TestOptimizedForSize();

  TestOptimizedForSize(const TestOptimizedForSize& from);

  inline TestOptimizedForSize& operator=(const TestOptimizedForSize& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TestOptimizedForSize& default_instance();

  enum FooCase {
    kIntegerField = 2,
    kStringField = 3,
    FOO_NOT_SET = 0,
  };

  void Swap(TestOptimizedForSize* other);

  // implements Message ----------------------------------------------

  inline TestOptimizedForSize* New() const { return New(NULL); }

  TestOptimizedForSize* New(::google::protobuf::Arena* arena) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(TestOptimizedForSize* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 i = 1;
  bool has_i() const;
  void clear_i();
  static const int kIFieldNumber = 1;
  ::google::protobuf::int32 i() const;
  void set_i(::google::protobuf::int32 value);

  // optional .protobuf_unittest.ForeignMessage msg = 19;
  bool has_msg() const;
  void clear_msg();
  static const int kMsgFieldNumber = 19;
  const ::protobuf_unittest::ForeignMessage& msg() const;
  ::protobuf_unittest::ForeignMessage* mutable_msg();
  ::protobuf_unittest::ForeignMessage* release_msg();
  void set_allocated_msg(::protobuf_unittest::ForeignMessage* msg);

  // optional int32 integer_field = 2;
  bool has_integer_field() const;
  void clear_integer_field();
  static const int kIntegerFieldFieldNumber = 2;
  ::google::protobuf::int32 integer_field() const;
  void set_integer_field(::google::protobuf::int32 value);

  // optional string string_field = 3;
  bool has_string_field() const;
  void clear_string_field();
  static const int kStringFieldFieldNumber = 3;
  const ::std::string& string_field() const;
  void set_string_field(const ::std::string& value);
  void set_string_field(const char* value);
  void set_string_field(const char* value, size_t size);
  ::std::string* mutable_string_field();
  ::std::string* release_string_field();
  void set_allocated_string_field(::std::string* string_field);

  GOOGLE_PROTOBUF_EXTENSION_ACCESSORS(TestOptimizedForSize)
  FooCase foo_case() const;
  static const int kTestExtensionFieldNumber = 1234;
  static ::google::protobuf::internal::ExtensionIdentifier< ::protobuf_unittest::TestOptimizedForSize,
      ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int32 >, 5, false >
    test_extension;
  static const int kTestExtension2FieldNumber = 1235;
  static ::google::protobuf::internal::ExtensionIdentifier< ::protobuf_unittest::TestOptimizedForSize,
      ::google::protobuf::internal::MessageTypeTraits< ::protobuf_unittest::TestRequiredOptimizedForSize >, 11, false >
    test_extension2;
  // @@protoc_insertion_point(class_scope:protobuf_unittest.TestOptimizedForSize)
 private:
  inline void set_has_i();
  inline void clear_has_i();
  inline void set_has_msg();
  inline void clear_has_msg();
  inline void set_has_integer_field();
  inline void set_has_string_field();

  inline bool has_foo() const;
  void clear_foo();
  inline void clear_has_foo();

  ::google::protobuf::internal::ExtensionSet _extensions_;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::protobuf_unittest::ForeignMessage* msg_;
  ::google::protobuf::int32 i_;
  union FooUnion {
    FooUnion() {}
    ::google::protobuf::int32 integer_field_;
    ::google::protobuf::internal::ArenaStringPtr string_field_;
  } foo_;
  ::google::protobuf::uint32 _oneof_case_[1];

  friend void  protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();

  void InitAsDefaultInstance();
  static TestOptimizedForSize* default_instance_;
};
// -------------------------------------------------------------------

class TestRequiredOptimizedForSize : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:protobuf_unittest.TestRequiredOptimizedForSize) */ {
 public:
  TestRequiredOptimizedForSize();
  virtual ~TestRequiredOptimizedForSize();

  TestRequiredOptimizedForSize(const TestRequiredOptimizedForSize& from);

  inline TestRequiredOptimizedForSize& operator=(const TestRequiredOptimizedForSize& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TestRequiredOptimizedForSize& default_instance();

  void Swap(TestRequiredOptimizedForSize* other);

  // implements Message ----------------------------------------------

  inline TestRequiredOptimizedForSize* New() const { return New(NULL); }

  TestRequiredOptimizedForSize* New(::google::protobuf::Arena* arena) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(TestRequiredOptimizedForSize* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 x = 1;
  bool has_x() const;
  void clear_x();
  static const int kXFieldNumber = 1;
  ::google::protobuf::int32 x() const;
  void set_x(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf_unittest.TestRequiredOptimizedForSize)
 private:
  inline void set_has_x();
  inline void clear_has_x();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 x_;
  friend void  protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();

  void InitAsDefaultInstance();
  static TestRequiredOptimizedForSize* default_instance_;
};
// -------------------------------------------------------------------

class TestOptionalOptimizedForSize : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:protobuf_unittest.TestOptionalOptimizedForSize) */ {
 public:
  TestOptionalOptimizedForSize();
  virtual ~TestOptionalOptimizedForSize();

  TestOptionalOptimizedForSize(const TestOptionalOptimizedForSize& from);

  inline TestOptionalOptimizedForSize& operator=(const TestOptionalOptimizedForSize& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TestOptionalOptimizedForSize& default_instance();

  void Swap(TestOptionalOptimizedForSize* other);

  // implements Message ----------------------------------------------

  inline TestOptionalOptimizedForSize* New() const { return New(NULL); }

  TestOptionalOptimizedForSize* New(::google::protobuf::Arena* arena) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(TestOptionalOptimizedForSize* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .protobuf_unittest.TestRequiredOptimizedForSize o = 1;
  bool has_o() const;
  void clear_o();
  static const int kOFieldNumber = 1;
  const ::protobuf_unittest::TestRequiredOptimizedForSize& o() const;
  ::protobuf_unittest::TestRequiredOptimizedForSize* mutable_o();
  ::protobuf_unittest::TestRequiredOptimizedForSize* release_o();
  void set_allocated_o(::protobuf_unittest::TestRequiredOptimizedForSize* o);

  // @@protoc_insertion_point(class_scope:protobuf_unittest.TestOptionalOptimizedForSize)
 private:
  inline void set_has_o();
  inline void clear_has_o();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::protobuf_unittest::TestRequiredOptimizedForSize* o_;
  friend void  protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();

  void InitAsDefaultInstance();
  static TestOptionalOptimizedForSize* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TestOptimizedForSize

// optional int32 i = 1;
inline bool TestOptimizedForSize::has_i() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TestOptimizedForSize::set_has_i() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TestOptimizedForSize::clear_has_i() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TestOptimizedForSize::clear_i() {
  i_ = 0;
  clear_has_i();
}
inline ::google::protobuf::int32 TestOptimizedForSize::i() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.i)
  return i_;
}
inline void TestOptimizedForSize::set_i(::google::protobuf::int32 value) {
  set_has_i();
  i_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.i)
}

// optional .protobuf_unittest.ForeignMessage msg = 19;
inline bool TestOptimizedForSize::has_msg() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TestOptimizedForSize::set_has_msg() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TestOptimizedForSize::clear_has_msg() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TestOptimizedForSize::clear_msg() {
  if (msg_ != NULL) msg_->::protobuf_unittest::ForeignMessage::Clear();
  clear_has_msg();
}
inline const ::protobuf_unittest::ForeignMessage& TestOptimizedForSize::msg() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.msg)
  return msg_ != NULL ? *msg_ : *default_instance_->msg_;
}
inline ::protobuf_unittest::ForeignMessage* TestOptimizedForSize::mutable_msg() {
  set_has_msg();
  if (msg_ == NULL) {
    msg_ = new ::protobuf_unittest::ForeignMessage;
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptimizedForSize.msg)
  return msg_;
}
inline ::protobuf_unittest::ForeignMessage* TestOptimizedForSize::release_msg() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptimizedForSize.msg)
  clear_has_msg();
  ::protobuf_unittest::ForeignMessage* temp = msg_;
  msg_ = NULL;
  return temp;
}
inline void TestOptimizedForSize::set_allocated_msg(::protobuf_unittest::ForeignMessage* msg) {
  delete msg_;
  if (msg != NULL && msg->GetArena() != NULL) {
    ::protobuf_unittest::ForeignMessage* new_msg = new ::protobuf_unittest::ForeignMessage;
    new_msg->CopyFrom(*msg);
    msg = new_msg;
  }
  msg_ = msg;
  if (msg) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptimizedForSize.msg)
}

// optional int32 integer_field = 2;
inline bool TestOptimizedForSize::has_integer_field() const {
  return foo_case() == kIntegerField;
}
inline void TestOptimizedForSize::set_has_integer_field() {
  _oneof_case_[0] = kIntegerField;
}
inline void TestOptimizedForSize::clear_integer_field() {
  if (has_integer_field()) {
    foo_.integer_field_ = 0;
    clear_has_foo();
  }
}
inline ::google::protobuf::int32 TestOptimizedForSize::integer_field() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.integer_field)
  if (has_integer_field()) {
    return foo_.integer_field_;
  }
  return 0;
}
inline void TestOptimizedForSize::set_integer_field(::google::protobuf::int32 value) {
  if (!has_integer_field()) {
    clear_foo();
    set_has_integer_field();
  }
  foo_.integer_field_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.integer_field)
}

// optional string string_field = 3;
inline bool TestOptimizedForSize::has_string_field() const {
  return foo_case() == kStringField;
}
inline void TestOptimizedForSize::set_has_string_field() {
  _oneof_case_[0] = kStringField;
}
inline void TestOptimizedForSize::clear_string_field() {
  if (has_string_field()) {
    foo_.string_field_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    clear_has_foo();
  }
}
inline const ::std::string& TestOptimizedForSize::string_field() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.string_field)
  if (has_string_field()) {
    return foo_.string_field_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  return *&::google::protobuf::internal::GetEmptyStringAlreadyInited();
}
inline void TestOptimizedForSize::set_string_field(const ::std::string& value) {
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.string_field)
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.string_field)
}
inline void TestOptimizedForSize::set_string_field(const char* value) {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(value));
  // @@protoc_insertion_point(field_set_char:protobuf_unittest.TestOptimizedForSize.string_field)
}
inline void TestOptimizedForSize::set_string_field(const char* value, size_t size) {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:protobuf_unittest.TestOptimizedForSize.string_field)
}
inline ::std::string* TestOptimizedForSize::mutable_string_field() {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptimizedForSize.string_field)
  return foo_.string_field_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TestOptimizedForSize::release_string_field() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptimizedForSize.string_field)
  if (has_string_field()) {
    clear_has_foo();
    return foo_.string_field_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  } else {
    return NULL;
  }
}
inline void TestOptimizedForSize::set_allocated_string_field(::std::string* string_field) {
  if (!has_string_field()) {
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  clear_foo();
  if (string_field != NULL) {
    set_has_string_field();
    foo_.string_field_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        string_field);
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptimizedForSize.string_field)
}

inline bool TestOptimizedForSize::has_foo() const {
  return foo_case() != FOO_NOT_SET;
}
inline void TestOptimizedForSize::clear_has_foo() {
  _oneof_case_[0] = FOO_NOT_SET;
}
inline TestOptimizedForSize::FooCase TestOptimizedForSize::foo_case() const {
  return TestOptimizedForSize::FooCase(_oneof_case_[0]);
}
// -------------------------------------------------------------------

// TestRequiredOptimizedForSize

// required int32 x = 1;
inline bool TestRequiredOptimizedForSize::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TestRequiredOptimizedForSize::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TestRequiredOptimizedForSize::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TestRequiredOptimizedForSize::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline ::google::protobuf::int32 TestRequiredOptimizedForSize::x() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestRequiredOptimizedForSize.x)
  return x_;
}
inline void TestRequiredOptimizedForSize::set_x(::google::protobuf::int32 value) {
  set_has_x();
  x_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestRequiredOptimizedForSize.x)
}

// -------------------------------------------------------------------

// TestOptionalOptimizedForSize

// optional .protobuf_unittest.TestRequiredOptimizedForSize o = 1;
inline bool TestOptionalOptimizedForSize::has_o() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TestOptionalOptimizedForSize::set_has_o() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TestOptionalOptimizedForSize::clear_has_o() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TestOptionalOptimizedForSize::clear_o() {
  if (o_ != NULL) o_->::protobuf_unittest::TestRequiredOptimizedForSize::Clear();
  clear_has_o();
}
inline const ::protobuf_unittest::TestRequiredOptimizedForSize& TestOptionalOptimizedForSize::o() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptionalOptimizedForSize.o)
  return o_ != NULL ? *o_ : *default_instance_->o_;
}
inline ::protobuf_unittest::TestRequiredOptimizedForSize* TestOptionalOptimizedForSize::mutable_o() {
  set_has_o();
  if (o_ == NULL) {
    o_ = new ::protobuf_unittest::TestRequiredOptimizedForSize;
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptionalOptimizedForSize.o)
  return o_;
}
inline ::protobuf_unittest::TestRequiredOptimizedForSize* TestOptionalOptimizedForSize::release_o() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptionalOptimizedForSize.o)
  clear_has_o();
  ::protobuf_unittest::TestRequiredOptimizedForSize* temp = o_;
  o_ = NULL;
  return temp;
}
inline void TestOptionalOptimizedForSize::set_allocated_o(::protobuf_unittest::TestRequiredOptimizedForSize* o) {
  delete o_;
  o_ = o;
  if (o) {
    set_has_o();
  } else {
    clear_has_o();
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptionalOptimizedForSize.o)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf_unittest

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto__INCLUDED
