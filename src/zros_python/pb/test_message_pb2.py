# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: test_message.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='test_message.proto',
  package='zros_example',
  syntax='proto3',
  serialized_pb=_b('\n\x12test_message.proto\x12\x0czros_example\",\n\x0bTestMessage\x12\r\n\x05\x63ount\x18\x01 \x01(\x05\x12\x0e\n\x06\x64\x65tail\x18\x02 \x01(\tb\x06proto3')
)




_TESTMESSAGE = _descriptor.Descriptor(
  name='TestMessage',
  full_name='zros_example.TestMessage',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='count', full_name='zros_example.TestMessage.count', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='detail', full_name='zros_example.TestMessage.detail', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=36,
  serialized_end=80,
)

DESCRIPTOR.message_types_by_name['TestMessage'] = _TESTMESSAGE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

TestMessage = _reflection.GeneratedProtocolMessageType('TestMessage', (_message.Message,), dict(
  DESCRIPTOR = _TESTMESSAGE,
  __module__ = 'test_message_pb2'
  # @@protoc_insertion_point(class_scope:zros_example.TestMessage)
  ))
_sym_db.RegisterMessage(TestMessage)


# @@protoc_insertion_point(module_scope)
