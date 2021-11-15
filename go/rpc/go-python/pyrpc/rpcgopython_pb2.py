# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: rpcgopython.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='rpcgopython.proto',
  package='rpcgopython',
  syntax='proto3',
  serialized_options=b'Z\014gorpc/;gorpc',
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x11rpcgopython.proto\x12\x0brpcgopython\"G\n\x07Message\x12\x0f\n\x07\x63ontent\x18\x01 \x01(\t\x12\x0c\n\x04\x66rom\x18\x02 \x01(\t\x12\n\n\x02to\x18\x03 \x01(\t\x12\x11\n\tcreatedAt\x18\x04 \x01(\x04\"3\n\x04User\x12\x0c\n\x04name\x18\x01 \x01(\t\x12\n\n\x02ip\x18\x02 \x01(\t\x12\x11\n\tcreatedAt\x18\x03 \x01(\x04\"7\n\x06Result\x12\x0b\n\x03res\x18\x01 \x01(\x08\x12\r\n\x05\x65rror\x18\x02 \x01(\t\x12\x11\n\tcreatedAt\x18\x03 \x01(\x04\x32\x44\n\x0eGatewayService\x12\x32\n\x08Register\x12\x11.rpcgopython.User\x1a\x13.rpcgopython.Result2B\n\x0cHelloService\x12\x32\n\x05Hello\x12\x14.rpcgopython.Message\x1a\x13.rpcgopython.ResultB\x0eZ\x0cgorpc/;gorpcb\x06proto3'
)




_MESSAGE = _descriptor.Descriptor(
  name='Message',
  full_name='rpcgopython.Message',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='content', full_name='rpcgopython.Message.content', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='from', full_name='rpcgopython.Message.from', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='to', full_name='rpcgopython.Message.to', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='createdAt', full_name='rpcgopython.Message.createdAt', index=3,
      number=4, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=34,
  serialized_end=105,
)


_USER = _descriptor.Descriptor(
  name='User',
  full_name='rpcgopython.User',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='name', full_name='rpcgopython.User.name', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='ip', full_name='rpcgopython.User.ip', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='createdAt', full_name='rpcgopython.User.createdAt', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=107,
  serialized_end=158,
)


_RESULT = _descriptor.Descriptor(
  name='Result',
  full_name='rpcgopython.Result',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='res', full_name='rpcgopython.Result.res', index=0,
      number=1, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='error', full_name='rpcgopython.Result.error', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='createdAt', full_name='rpcgopython.Result.createdAt', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=160,
  serialized_end=215,
)

DESCRIPTOR.message_types_by_name['Message'] = _MESSAGE
DESCRIPTOR.message_types_by_name['User'] = _USER
DESCRIPTOR.message_types_by_name['Result'] = _RESULT
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

Message = _reflection.GeneratedProtocolMessageType('Message', (_message.Message,), {
  'DESCRIPTOR' : _MESSAGE,
  '__module__' : 'rpcgopython_pb2'
  # @@protoc_insertion_point(class_scope:rpcgopython.Message)
  })
_sym_db.RegisterMessage(Message)

User = _reflection.GeneratedProtocolMessageType('User', (_message.Message,), {
  'DESCRIPTOR' : _USER,
  '__module__' : 'rpcgopython_pb2'
  # @@protoc_insertion_point(class_scope:rpcgopython.User)
  })
_sym_db.RegisterMessage(User)

Result = _reflection.GeneratedProtocolMessageType('Result', (_message.Message,), {
  'DESCRIPTOR' : _RESULT,
  '__module__' : 'rpcgopython_pb2'
  # @@protoc_insertion_point(class_scope:rpcgopython.Result)
  })
_sym_db.RegisterMessage(Result)


DESCRIPTOR._options = None

_GATEWAYSERVICE = _descriptor.ServiceDescriptor(
  name='GatewayService',
  full_name='rpcgopython.GatewayService',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_start=217,
  serialized_end=285,
  methods=[
  _descriptor.MethodDescriptor(
    name='Register',
    full_name='rpcgopython.GatewayService.Register',
    index=0,
    containing_service=None,
    input_type=_USER,
    output_type=_RESULT,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
])
_sym_db.RegisterServiceDescriptor(_GATEWAYSERVICE)

DESCRIPTOR.services_by_name['GatewayService'] = _GATEWAYSERVICE


_HELLOSERVICE = _descriptor.ServiceDescriptor(
  name='HelloService',
  full_name='rpcgopython.HelloService',
  file=DESCRIPTOR,
  index=1,
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_start=287,
  serialized_end=353,
  methods=[
  _descriptor.MethodDescriptor(
    name='Hello',
    full_name='rpcgopython.HelloService.Hello',
    index=0,
    containing_service=None,
    input_type=_MESSAGE,
    output_type=_RESULT,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
])
_sym_db.RegisterServiceDescriptor(_HELLOSERVICE)

DESCRIPTOR.services_by_name['HelloService'] = _HELLOSERVICE

# @@protoc_insertion_point(module_scope)
