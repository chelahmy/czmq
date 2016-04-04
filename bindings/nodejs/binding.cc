/*  =========================================================================
    CZMQ Node.js binding implementation

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of CZMQ, the high-level C binding for 0MQ:       
    http://czmq.zeromq.org.                                            
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

#include "binding.h"

using namespace v8;
using namespace Nan;

NAN_MODULE_INIT (Zarmour::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zarmour").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "encode", _encode);
    Nan::SetPrototypeMethod (tpl, "decode", _decode);
    Nan::SetPrototypeMethod (tpl, "mode", _mode);
    Nan::SetPrototypeMethod (tpl, "modeStr", _mode_str);
    Nan::SetPrototypeMethod (tpl, "pad", _pad);
    Nan::SetPrototypeMethod (tpl, "padChar", _pad_char);
    Nan::SetPrototypeMethod (tpl, "lineBreaks", _line_breaks);
    Nan::SetPrototypeMethod (tpl, "lineLength", _line_length);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zarmour").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zarmour::Zarmour (void) {
    self = zarmour_new ();
}

Zarmour::Zarmour (zarmour_t *self_) {
    self = self_;
}

Zarmour::~Zarmour () {
}

NAN_METHOD (Zarmour::New) {
    assert (info.IsConstructCall ());
    Zarmour *zarmour = new Zarmour ();
    if (zarmour) {
        zarmour->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zarmour::destroy) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    zarmour_destroy (&zarmour->self);
}


NAN_METHOD (Zarmour::defined) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zarmour->self != NULL));
}

NAN_METHOD (Zarmour::_encode) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    char *result = (char *) zarmour_encode (zarmour->self, (const byte *)data, (size_t)size);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zarmour::_decode) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    char *data;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `data`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`data` must be a string");
    else {
        Nan::Utf8String data_utf8 (info [0].As<String>());
        data = *data_utf8;
    }
    zchunk_t *result = zarmour_decode (zarmour->self, (const char *)data);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zarmour::_mode) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    int result = zarmour_mode (zarmour->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zarmour::_mode_str) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    char *result = (char *) zarmour_mode_str (zarmour->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zarmour::_pad) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    bool result = zarmour_pad (zarmour->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zarmour::_pad_char) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    char result [2] = " ";
    result [0] = zarmour_pad_char (zarmour->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zarmour::_line_breaks) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    bool result = zarmour_line_breaks (zarmour->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zarmour::_line_length) {
    Zarmour *zarmour = Nan::ObjectWrap::Unwrap <Zarmour> (info.Holder ());
    size_t result = zarmour_line_length (zarmour->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

Nan::Persistent <Function> &Zarmour::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zcert::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zcert").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "publicKey", _public_key);
    Nan::SetPrototypeMethod (tpl, "secretKey", _secret_key);
    Nan::SetPrototypeMethod (tpl, "publicTxt", _public_txt);
    Nan::SetPrototypeMethod (tpl, "secretTxt", _secret_txt);
    Nan::SetPrototypeMethod (tpl, "meta", _meta);
    Nan::SetPrototypeMethod (tpl, "metaKeys", _meta_keys);
    Nan::SetPrototypeMethod (tpl, "save", _save);
    Nan::SetPrototypeMethod (tpl, "savePublic", _save_public);
    Nan::SetPrototypeMethod (tpl, "saveSecret", _save_secret);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "eq", _eq);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zcert").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zcert::Zcert (void) {
    self = zcert_new ();
}

Zcert::Zcert (zcert_t *self_) {
    self = self_;
}

Zcert::~Zcert () {
}

NAN_METHOD (Zcert::New) {
    assert (info.IsConstructCall ());
    Zcert *zcert = new Zcert ();
    if (zcert) {
        zcert->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zcert::destroy) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    zcert_destroy (&zcert->self);
}


NAN_METHOD (Zcert::defined) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zcert->self != NULL));
}

NAN_METHOD (Zcert::_public_key) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    const char *result = (const char *) zcert_public_key (zcert->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, 32).ToLocalChecked ());
}

NAN_METHOD (Zcert::_secret_key) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    const char *result = (const char *) zcert_secret_key (zcert->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, 32).ToLocalChecked ());
}

NAN_METHOD (Zcert::_public_txt) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *result = (char *) zcert_public_txt (zcert->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zcert::_secret_txt) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *result = (char *) zcert_secret_txt (zcert->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zcert::_meta) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *name;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `name`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`name` must be a string");
    else {
        Nan::Utf8String name_utf8 (info [0].As<String>());
        name = *name_utf8;
    }
    char *result = (char *) zcert_meta (zcert->self, (const char *)name);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zcert::_meta_keys) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    zlist_t *result = zcert_meta_keys (zcert->self);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zcert::_save) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zcert_save (zcert->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zcert::_save_public) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zcert_save_public (zcert->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zcert::_save_secret) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zcert_save_secret (zcert->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zcert::_dup) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    zcert_t *result = zcert_dup (zcert->self);
    Zcert *zcert_result = new Zcert (result);
    if (zcert_result) {
    //  Don't yet know how to return a new object
    //      zcert->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zcert::_eq) {
    Zcert *zcert = Nan::ObjectWrap::Unwrap <Zcert> (info.Holder ());
    Zcert *compare = Nan::ObjectWrap::Unwrap<Zcert>(info [0].As<Object>());
    bool result = zcert_eq (zcert->self, compare->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

Nan::Persistent <Function> &Zcert::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zcertstore::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zcertstore").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "lookup", _lookup);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zcertstore").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zcertstore::Zcertstore (const char *location) {
    self = zcertstore_new ((const char *)location);
}

Zcertstore::Zcertstore (zcertstore_t *self_) {
    self = self_;
}

Zcertstore::~Zcertstore () {
}

NAN_METHOD (Zcertstore::New) {
    assert (info.IsConstructCall ());
    char *location;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `location`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`location` must be a string");
    else {
        Nan::Utf8String location_utf8 (info [0].As<String>());
        location = *location_utf8;
    }
    Zcertstore *zcertstore = new Zcertstore ((const char *)location);
    if (zcertstore) {
        zcertstore->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zcertstore::destroy) {
    Zcertstore *zcertstore = Nan::ObjectWrap::Unwrap <Zcertstore> (info.Holder ());
    zcertstore_destroy (&zcertstore->self);
}


NAN_METHOD (Zcertstore::defined) {
    Zcertstore *zcertstore = Nan::ObjectWrap::Unwrap <Zcertstore> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zcertstore->self != NULL));
}

NAN_METHOD (Zcertstore::_lookup) {
    Zcertstore *zcertstore = Nan::ObjectWrap::Unwrap <Zcertstore> (info.Holder ());
    char *public_key;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `public key`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`public key` must be a string");
    else {
        Nan::Utf8String public_key_utf8 (info [0].As<String>());
        public_key = *public_key_utf8;
    }
    zcert_t *result = zcertstore_lookup (zcertstore->self, (const char *)public_key);
    Zcert *zcert_result = new Zcert (result);
    if (zcert_result) {
    //  Don't yet know how to return a new object
    //      zcert->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

Nan::Persistent <Function> &Zcertstore::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zchunk::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zchunk").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "maxSize", _max_size);
    Nan::SetPrototypeMethod (tpl, "data", _data);
    Nan::SetPrototypeMethod (tpl, "set", _set);
    Nan::SetPrototypeMethod (tpl, "append", _append);
    Nan::SetPrototypeMethod (tpl, "extend", _extend);
    Nan::SetPrototypeMethod (tpl, "consume", _consume);
    Nan::SetPrototypeMethod (tpl, "exhausted", _exhausted);
    Nan::SetPrototypeMethod (tpl, "slurp", _slurp);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "strhex", _strhex);
    Nan::SetPrototypeMethod (tpl, "strdup", _strdup);
    Nan::SetPrototypeMethod (tpl, "streq", _streq);
    Nan::SetPrototypeMethod (tpl, "pack", _pack);
    Nan::SetPrototypeMethod (tpl, "unpack", _unpack);
    Nan::SetPrototypeMethod (tpl, "digest", _digest);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zchunk").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zchunk::Zchunk (const void *data, size_t size) {
    self = zchunk_new ((const void *)data, (size_t)size);
}

Zchunk::Zchunk (zchunk_t *self_) {
    self = self_;
}

Zchunk::~Zchunk () {
}

NAN_METHOD (Zchunk::New) {
    assert (info.IsConstructCall ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    Zchunk *zchunk = new Zchunk ((const void *)data, (size_t)size);
    if (zchunk) {
        zchunk->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zchunk::destroy) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    zchunk_destroy (&zchunk->self);
}


NAN_METHOD (Zchunk::defined) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zchunk->self != NULL));
}

NAN_METHOD (Zchunk::_size) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    size_t result = zchunk_size (zchunk->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_max_size) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    size_t result = zchunk_max_size (zchunk->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_data) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    const char *result = (const char *) zchunk_data (zchunk->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, zchunk_size (zchunk->self)).ToLocalChecked ());
}

NAN_METHOD (Zchunk::_set) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    size_t result = zchunk_set (zchunk->self, (const void *)data, (size_t)size);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_append) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    size_t result = zchunk_append (zchunk->self, (const void *)data, (size_t)size);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_extend) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    size_t result = zchunk_extend (zchunk->self, (const void *)data, (size_t)size);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_consume) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    Zchunk *source = Nan::ObjectWrap::Unwrap<Zchunk>(info [0].As<Object>());
    size_t result = zchunk_consume (zchunk->self, source->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zchunk::_exhausted) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    bool result = zchunk_exhausted (zchunk->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zchunk::_slurp) {
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `maxsize`");
    else
    if (!info [1]->IsNumber ())
        return Nan::ThrowTypeError ("`maxsize` must be a number");
    size_t maxsize = Nan::To<int64_t>(info [1]).FromJust ();
    zchunk_t *result = zchunk_slurp ((const char *)filename, (size_t)maxsize);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zchunk::_dup) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    zchunk_t *result = zchunk_dup (zchunk->self);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zchunk::_strhex) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    char *result = (char *) zchunk_strhex (zchunk->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zchunk::_strdup) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    char *result = (char *) zchunk_strdup (zchunk->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zchunk::_streq) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    char *string;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `string`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`string` must be a string");
    else {
        Nan::Utf8String string_utf8 (info [0].As<String>());
        string = *string_utf8;
    }
    bool result = zchunk_streq (zchunk->self, (const char *)string);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zchunk::_pack) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    zframe_t *result = zchunk_pack (zchunk->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zchunk::_unpack) {
    Zframe *frame = Nan::ObjectWrap::Unwrap<Zframe>(info [0].As<Object>());
    zchunk_t *result = zchunk_unpack (frame->self);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zchunk::_digest) {
    Zchunk *zchunk = Nan::ObjectWrap::Unwrap <Zchunk> (info.Holder ());
    char *result = (char *) zchunk_digest (zchunk->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zchunk::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zclock::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zclock").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "time", _time);
    Nan::SetPrototypeMethod (tpl, "mono", _mono);
    Nan::SetPrototypeMethod (tpl, "usecs", _usecs);
    Nan::SetPrototypeMethod (tpl, "timestr", _timestr);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zclock").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zclock::Zclock () {
}

Zclock::~Zclock () {
}

NAN_METHOD (Zclock::New) {
    assert (info.IsConstructCall ());
    Zclock *zclock = new Zclock ();
    if (zclock) {
        zclock->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zclock::_time) {
    int64_t result = zclock_time ();
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zclock::_mono) {
    int64_t result = zclock_mono ();
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zclock::_usecs) {
    int64_t result = zclock_usecs ();
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zclock::_timestr) {
    char *result = (char *) zclock_timestr ();
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zclock::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zconfig::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zconfig").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "name", _name);
    Nan::SetPrototypeMethod (tpl, "value", _value);
    Nan::SetPrototypeMethod (tpl, "get", _get);
    Nan::SetPrototypeMethod (tpl, "child", _child);
    Nan::SetPrototypeMethod (tpl, "next", _next);
    Nan::SetPrototypeMethod (tpl, "locate", _locate);
    Nan::SetPrototypeMethod (tpl, "atDepth", _at_depth);
    Nan::SetPrototypeMethod (tpl, "comments", _comments);
    Nan::SetPrototypeMethod (tpl, "save", _save);
    Nan::SetPrototypeMethod (tpl, "savef", _savef);
    Nan::SetPrototypeMethod (tpl, "filename", _filename);
    Nan::SetPrototypeMethod (tpl, "reload", _reload);
    Nan::SetPrototypeMethod (tpl, "chunkLoad", _chunk_load);
    Nan::SetPrototypeMethod (tpl, "chunkSave", _chunk_save);
    Nan::SetPrototypeMethod (tpl, "strLoad", _str_load);
    Nan::SetPrototypeMethod (tpl, "strSave", _str_save);
    Nan::SetPrototypeMethod (tpl, "hasChanged", _has_changed);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zconfig").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zconfig::Zconfig (const char *name, zconfig_t *parent) {
    self = zconfig_new ((const char *)name, parent);
}

Zconfig::Zconfig (zconfig_t *self_) {
    self = self_;
}

Zconfig::~Zconfig () {
}

NAN_METHOD (Zconfig::New) {
    assert (info.IsConstructCall ());
    char *name;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `name`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`name` must be a string");
    else {
        Nan::Utf8String name_utf8 (info [0].As<String>());
        name = *name_utf8;
    }
    Zconfig *parent = Nan::ObjectWrap::Unwrap<Zconfig>(info [1].As<Object>());
    Zconfig *zconfig = new Zconfig ((const char *)name, parent->self);
    if (zconfig) {
        zconfig->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zconfig::destroy) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    zconfig_destroy (&zconfig->self);
}


NAN_METHOD (Zconfig::defined) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zconfig->self != NULL));
}

NAN_METHOD (Zconfig::_name) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *result = (char *) zconfig_name (zconfig->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zconfig::_value) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *result = (char *) zconfig_value (zconfig->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zconfig::_get) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    char *default_value;
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `default value`");
    else
    if (!info [1]->IsString ())
        return Nan::ThrowTypeError ("`default value` must be a string");
    else {
        Nan::Utf8String default_value_utf8 (info [1].As<String>());
        default_value = *default_value_utf8;
    }
    char *result = (char *) zconfig_get (zconfig->self, (const char *)path, (const char *)default_value);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zconfig::_child) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    zconfig_t *result = zconfig_child (zconfig->self);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_next) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    zconfig_t *result = zconfig_next (zconfig->self);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_locate) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    zconfig_t *result = zconfig_locate (zconfig->self, (const char *)path);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_at_depth) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `level`");
    else
    if (!info [0]->IsNumber ())
        return Nan::ThrowTypeError ("`level` must be a number");
    int level = Nan::To<int>(info [0]).FromJust ();

    zconfig_t *result = zconfig_at_depth (zconfig->self, (int)level);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_comments) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    zlist_t *result = zconfig_comments (zconfig->self);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_save) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zconfig_save (zconfig->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zconfig::_savef) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zconfig_savef (zconfig->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zconfig::_filename) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *result = (char *) zconfig_filename (zconfig->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zconfig::_reload) {
    Zconfig *self_p = Nan::ObjectWrap::Unwrap<Zconfig>(info [0].As<Object>());
    int result = zconfig_reload (&self_p->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zconfig::_chunk_load) {
    Zchunk *chunk = Nan::ObjectWrap::Unwrap<Zchunk>(info [0].As<Object>());
    zconfig_t *result = zconfig_chunk_load (chunk->self);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_chunk_save) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    zchunk_t *result = zconfig_chunk_save (zconfig->self);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_str_load) {
    char *string;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `string`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`string` must be a string");
    else {
        Nan::Utf8String string_utf8 (info [0].As<String>());
        string = *string_utf8;
    }
    zconfig_t *result = zconfig_str_load ((const char *)string);
    Zconfig *zconfig_result = new Zconfig (result);
    if (zconfig_result) {
    //  Don't yet know how to return a new object
    //      zconfig->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zconfig::_str_save) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    char *result = (char *) zconfig_str_save (zconfig->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zconfig::_has_changed) {
    Zconfig *zconfig = Nan::ObjectWrap::Unwrap <Zconfig> (info.Holder ());
    bool result = zconfig_has_changed (zconfig->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

Nan::Persistent <Function> &Zconfig::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zdigest::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zdigest").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "data", _data);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "string", _string);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zdigest").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zdigest::Zdigest (void) {
    self = zdigest_new ();
}

Zdigest::Zdigest (zdigest_t *self_) {
    self = self_;
}

Zdigest::~Zdigest () {
}

NAN_METHOD (Zdigest::New) {
    assert (info.IsConstructCall ());
    Zdigest *zdigest = new Zdigest ();
    if (zdigest) {
        zdigest->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zdigest::destroy) {
    Zdigest *zdigest = Nan::ObjectWrap::Unwrap <Zdigest> (info.Holder ());
    zdigest_destroy (&zdigest->self);
}


NAN_METHOD (Zdigest::defined) {
    Zdigest *zdigest = Nan::ObjectWrap::Unwrap <Zdigest> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zdigest->self != NULL));
}

NAN_METHOD (Zdigest::_data) {
    Zdigest *zdigest = Nan::ObjectWrap::Unwrap <Zdigest> (info.Holder ());
    const char *result = (const char *) zdigest_data (zdigest->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, zdigest_size (zdigest->self)).ToLocalChecked ());
}

NAN_METHOD (Zdigest::_size) {
    Zdigest *zdigest = Nan::ObjectWrap::Unwrap <Zdigest> (info.Holder ());
    size_t result = zdigest_size (zdigest->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zdigest::_string) {
    Zdigest *zdigest = Nan::ObjectWrap::Unwrap <Zdigest> (info.Holder ());
    char *result = (char *) zdigest_string (zdigest->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zdigest::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zdir::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zdir").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "path", _path);
    Nan::SetPrototypeMethod (tpl, "modified", _modified);
    Nan::SetPrototypeMethod (tpl, "cursize", _cursize);
    Nan::SetPrototypeMethod (tpl, "count", _count);
    Nan::SetPrototypeMethod (tpl, "list", _list);
    Nan::SetPrototypeMethod (tpl, "diff", _diff);
    Nan::SetPrototypeMethod (tpl, "resync", _resync);
    Nan::SetPrototypeMethod (tpl, "cache", _cache);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zdir").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zdir::Zdir (const char *path, const char *parent) {
    self = zdir_new ((const char *)path, (const char *)parent);
}

Zdir::Zdir (zdir_t *self_) {
    self = self_;
}

Zdir::~Zdir () {
}

NAN_METHOD (Zdir::New) {
    assert (info.IsConstructCall ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    char *parent;
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `parent`");
    else
    if (!info [1]->IsString ())
        return Nan::ThrowTypeError ("`parent` must be a string");
    else {
        Nan::Utf8String parent_utf8 (info [1].As<String>());
        parent = *parent_utf8;
    }
    Zdir *zdir = new Zdir ((const char *)path, (const char *)parent);
    if (zdir) {
        zdir->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zdir::destroy) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    zdir_destroy (&zdir->self);
}


NAN_METHOD (Zdir::defined) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zdir->self != NULL));
}

NAN_METHOD (Zdir::_path) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    char *result = (char *) zdir_path (zdir->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zdir::_modified) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    time_t result = zdir_modified (zdir->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zdir::_cursize) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    off_t result = zdir_cursize (zdir->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zdir::_count) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    size_t result = zdir_count (zdir->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zdir::_list) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    zlist_t *result = zdir_list (zdir->self);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zdir::_diff) {
    Zdir *older = Nan::ObjectWrap::Unwrap<Zdir>(info [0].As<Object>());
    Zdir *newer = Nan::ObjectWrap::Unwrap<Zdir>(info [1].As<Object>());
    char *alias;
    if (info [2]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `alias`");
    else
    if (!info [2]->IsString ())
        return Nan::ThrowTypeError ("`alias` must be a string");
    else {
        Nan::Utf8String alias_utf8 (info [2].As<String>());
        alias = *alias_utf8;
    }
    zlist_t *result = zdir_diff (older->self, newer->self, (const char *)alias);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zdir::_resync) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    char *alias;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `alias`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`alias` must be a string");
    else {
        Nan::Utf8String alias_utf8 (info [0].As<String>());
        alias = *alias_utf8;
    }
    zlist_t *result = zdir_resync (zdir->self, (const char *)alias);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zdir::_cache) {
    Zdir *zdir = Nan::ObjectWrap::Unwrap <Zdir> (info.Holder ());
    zhash_t *result = zdir_cache (zdir->self);
    Zhash *zhash_result = new Zhash (result);
    if (zhash_result) {
    //  Don't yet know how to return a new object
    //      zhash->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

Nan::Persistent <Function> &Zdir::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (ZdirPatch::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("ZdirPatch").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "path", _path);
    Nan::SetPrototypeMethod (tpl, "file", _file);
    Nan::SetPrototypeMethod (tpl, "op", _op);
    Nan::SetPrototypeMethod (tpl, "vpath", _vpath);
    Nan::SetPrototypeMethod (tpl, "digest", _digest);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("ZdirPatch").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

ZdirPatch::ZdirPatch (const char *path, zfile_t *file, int op, const char *alias) {
    self = zdir_patch_new ((const char *)path, file, (int)op, (const char *)alias);
}

ZdirPatch::ZdirPatch (zdir_patch_t *self_) {
    self = self_;
}

ZdirPatch::~ZdirPatch () {
}

NAN_METHOD (ZdirPatch::New) {
    assert (info.IsConstructCall ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    Zfile *file = Nan::ObjectWrap::Unwrap<Zfile>(info [1].As<Object>());
    if (info [2]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `op`");
    else
    if (!info [2]->IsNumber ())
        return Nan::ThrowTypeError ("`op` must be a number");
    int op = Nan::To<int>(info [2]).FromJust ();

    char *alias;
    if (info [3]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `alias`");
    else
    if (!info [3]->IsString ())
        return Nan::ThrowTypeError ("`alias` must be a string");
    else {
        Nan::Utf8String alias_utf8 (info [3].As<String>());
        alias = *alias_utf8;
    }
    ZdirPatch *zdir_patch = new ZdirPatch ((const char *)path, file->self, (int)op, (const char *)alias);
    if (zdir_patch) {
        zdir_patch->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (ZdirPatch::destroy) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    zdir_patch_destroy (&zdir_patch->self);
}


NAN_METHOD (ZdirPatch::defined) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zdir_patch->self != NULL));
}

NAN_METHOD (ZdirPatch::_dup) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    zdir_patch_t *result = zdir_patch_dup (zdir_patch->self);
    ZdirPatch *zdir_patch_result = new ZdirPatch (result);
    if (zdir_patch_result) {
    //  Don't yet know how to return a new object
    //      zdir_patch->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (ZdirPatch::_path) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    char *result = (char *) zdir_patch_path (zdir_patch->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (ZdirPatch::_file) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    zfile_t *result = zdir_patch_file (zdir_patch->self);
    Zfile *zfile_result = new Zfile (result);
    if (zfile_result) {
    //  Don't yet know how to return a new object
    //      zfile->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (ZdirPatch::_op) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    int result = zdir_patch_op (zdir_patch->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (ZdirPatch::_vpath) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    char *result = (char *) zdir_patch_vpath (zdir_patch->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (ZdirPatch::_digest) {
    ZdirPatch *zdir_patch = Nan::ObjectWrap::Unwrap <ZdirPatch> (info.Holder ());
    char *result = (char *) zdir_patch_digest (zdir_patch->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &ZdirPatch::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zfile::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zfile").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "filename", _filename);
    Nan::SetPrototypeMethod (tpl, "modified", _modified);
    Nan::SetPrototypeMethod (tpl, "cursize", _cursize);
    Nan::SetPrototypeMethod (tpl, "isDirectory", _is_directory);
    Nan::SetPrototypeMethod (tpl, "isRegular", _is_regular);
    Nan::SetPrototypeMethod (tpl, "isReadable", _is_readable);
    Nan::SetPrototypeMethod (tpl, "isWriteable", _is_writeable);
    Nan::SetPrototypeMethod (tpl, "isStable", _is_stable);
    Nan::SetPrototypeMethod (tpl, "hasChanged", _has_changed);
    Nan::SetPrototypeMethod (tpl, "input", _input);
    Nan::SetPrototypeMethod (tpl, "output", _output);
    Nan::SetPrototypeMethod (tpl, "read", _read);
    Nan::SetPrototypeMethod (tpl, "eof", _eof);
    Nan::SetPrototypeMethod (tpl, "write", _write);
    Nan::SetPrototypeMethod (tpl, "readln", _readln);
    Nan::SetPrototypeMethod (tpl, "digest", _digest);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zfile").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zfile::Zfile (const char *path, const char *name) {
    self = zfile_new ((const char *)path, (const char *)name);
}

Zfile::Zfile (zfile_t *self_) {
    self = self_;
}

Zfile::~Zfile () {
}

NAN_METHOD (Zfile::New) {
    assert (info.IsConstructCall ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    char *name;
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `name`");
    else
    if (!info [1]->IsString ())
        return Nan::ThrowTypeError ("`name` must be a string");
    else {
        Nan::Utf8String name_utf8 (info [1].As<String>());
        name = *name_utf8;
    }
    Zfile *zfile = new Zfile ((const char *)path, (const char *)name);
    if (zfile) {
        zfile->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zfile::destroy) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    zfile_destroy (&zfile->self);
}


NAN_METHOD (Zfile::defined) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zfile->self != NULL));
}

NAN_METHOD (Zfile::_dup) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    zfile_t *result = zfile_dup (zfile->self);
    Zfile *zfile_result = new Zfile (result);
    if (zfile_result) {
    //  Don't yet know how to return a new object
    //      zfile->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zfile::_filename) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    char *result = (char *) zfile_filename (zfile->self, (const char *)path);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zfile::_modified) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    time_t result = zfile_modified (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zfile::_cursize) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    off_t result = zfile_cursize (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zfile::_is_directory) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_is_directory (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_is_regular) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_is_regular (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_is_readable) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_is_readable (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_is_writeable) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_is_writeable (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_is_stable) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_is_stable (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_has_changed) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_has_changed (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_input) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    int result = zfile_input (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zfile::_output) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    int result = zfile_output (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zfile::_read) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `bytes`");
    else
    if (!info [0]->IsNumber ())
        return Nan::ThrowTypeError ("`bytes` must be a number");
    size_t bytes = Nan::To<int64_t>(info [0]).FromJust ();
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `offset`");
    else
    if (!info [1]->IsNumber ())
        return Nan::ThrowTypeError ("`offset` must be a number");
    off_t offset = Nan::To<int64_t>(info [1]).FromJust ();

    zchunk_t *result = zfile_read (zfile->self, (size_t)bytes, (off_t)offset);
    Zchunk *zchunk_result = new Zchunk (result);
    if (zchunk_result) {
    //  Don't yet know how to return a new object
    //      zchunk->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zfile::_eof) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    bool result = zfile_eof (zfile->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zfile::_write) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    Zchunk *chunk = Nan::ObjectWrap::Unwrap<Zchunk>(info [0].As<Object>());
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `offset`");
    else
    if (!info [1]->IsNumber ())
        return Nan::ThrowTypeError ("`offset` must be a number");
    off_t offset = Nan::To<int64_t>(info [1]).FromJust ();

    int result = zfile_write (zfile->self, chunk->self, (off_t)offset);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zfile::_readln) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    char *result = (char *) zfile_readln (zfile->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zfile::_digest) {
    Zfile *zfile = Nan::ObjectWrap::Unwrap <Zfile> (info.Holder ());
    char *result = (char *) zfile_digest (zfile->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zfile::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zframe::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zframe").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "data", _data);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "strhex", _strhex);
    Nan::SetPrototypeMethod (tpl, "strdup", _strdup);
    Nan::SetPrototypeMethod (tpl, "streq", _streq);
    Nan::SetPrototypeMethod (tpl, "more", _more);
    Nan::SetPrototypeMethod (tpl, "routingId", _routing_id);
    Nan::SetPrototypeMethod (tpl, "group", _group);
    Nan::SetPrototypeMethod (tpl, "setGroup", _set_group);
    Nan::SetPrototypeMethod (tpl, "eq", _eq);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zframe").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zframe::Zframe (const void *data, size_t size) {
    self = zframe_new ((const void *)data, (size_t)size);
}

Zframe::Zframe (zframe_t *self_) {
    self = self_;
}

Zframe::~Zframe () {
}

NAN_METHOD (Zframe::New) {
    assert (info.IsConstructCall ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *data = (const byte *) node::Buffer::Data (buffer_node);
    size_t size = node::Buffer::Length (buffer_node);
    Zframe *zframe = new Zframe ((const void *)data, (size_t)size);
    if (zframe) {
        zframe->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zframe::destroy) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    zframe_destroy (&zframe->self);
}


NAN_METHOD (Zframe::defined) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zframe->self != NULL));
}

NAN_METHOD (Zframe::_size) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    size_t result = zframe_size (zframe->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zframe::_data) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    const char *result = (const char *) zframe_data (zframe->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, zframe_size (zframe->self)).ToLocalChecked ());
}

NAN_METHOD (Zframe::_dup) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    zframe_t *result = zframe_dup (zframe->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zframe::_strhex) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    char *result = (char *) zframe_strhex (zframe->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zframe::_strdup) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    char *result = (char *) zframe_strdup (zframe->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zframe::_streq) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    char *string;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `string`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`string` must be a string");
    else {
        Nan::Utf8String string_utf8 (info [0].As<String>());
        string = *string_utf8;
    }
    bool result = zframe_streq (zframe->self, (const char *)string);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zframe::_more) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    int result = zframe_more (zframe->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zframe::_routing_id) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    uint32_t result = zframe_routing_id (zframe->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zframe::_group) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    char *result = (char *) zframe_group (zframe->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zframe::_set_group) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    char *group;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `group`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`group` must be a string");
    else {
        Nan::Utf8String group_utf8 (info [0].As<String>());
        group = *group_utf8;
    }
    int result = zframe_set_group (zframe->self, (const char *)group);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zframe::_eq) {
    Zframe *zframe = Nan::ObjectWrap::Unwrap <Zframe> (info.Holder ());
    Zframe *other = Nan::ObjectWrap::Unwrap<Zframe>(info [0].As<Object>());
    bool result = zframe_eq (zframe->self, other->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

Nan::Persistent <Function> &Zframe::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zhash::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zhash").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "rename", _rename);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "keys", _keys);
    Nan::SetPrototypeMethod (tpl, "cursor", _cursor);
    Nan::SetPrototypeMethod (tpl, "pack", _pack);
    Nan::SetPrototypeMethod (tpl, "save", _save);
    Nan::SetPrototypeMethod (tpl, "load", _load);
    Nan::SetPrototypeMethod (tpl, "refresh", _refresh);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zhash").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zhash::Zhash (void) {
    self = zhash_new ();
}

Zhash::Zhash (zhash_t *self_) {
    self = self_;
}

Zhash::~Zhash () {
}

NAN_METHOD (Zhash::New) {
    assert (info.IsConstructCall ());
    Zhash *zhash = new Zhash ();
    if (zhash) {
        zhash->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zhash::destroy) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    zhash_destroy (&zhash->self);
}


NAN_METHOD (Zhash::defined) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zhash->self != NULL));
}

NAN_METHOD (Zhash::_rename) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    char *old_key;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `old key`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`old key` must be a string");
    else {
        Nan::Utf8String old_key_utf8 (info [0].As<String>());
        old_key = *old_key_utf8;
    }
    char *new_key;
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `new key`");
    else
    if (!info [1]->IsString ())
        return Nan::ThrowTypeError ("`new key` must be a string");
    else {
        Nan::Utf8String new_key_utf8 (info [1].As<String>());
        new_key = *new_key_utf8;
    }
    int result = zhash_rename (zhash->self, (const char *)old_key, (const char *)new_key);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhash::_size) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    size_t result = zhash_size (zhash->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhash::_dup) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    zhash_t *result = zhash_dup (zhash->self);
    Zhash *zhash_result = new Zhash (result);
    if (zhash_result) {
    //  Don't yet know how to return a new object
    //      zhash->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhash::_keys) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    zlist_t *result = zhash_keys (zhash->self);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhash::_cursor) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    char *result = (char *) zhash_cursor (zhash->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zhash::_pack) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    zframe_t *result = zhash_pack (zhash->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhash::_save) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zhash_save (zhash->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhash::_load) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zhash_load (zhash->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhash::_refresh) {
    Zhash *zhash = Nan::ObjectWrap::Unwrap <Zhash> (info.Holder ());
    int result = zhash_refresh (zhash->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

Nan::Persistent <Function> &Zhash::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zhashx::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zhashx").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "keys", _keys);
    Nan::SetPrototypeMethod (tpl, "values", _values);
    Nan::SetPrototypeMethod (tpl, "save", _save);
    Nan::SetPrototypeMethod (tpl, "load", _load);
    Nan::SetPrototypeMethod (tpl, "refresh", _refresh);
    Nan::SetPrototypeMethod (tpl, "pack", _pack);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "dupV2", _dup_v2);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zhashx").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zhashx::Zhashx (void) {
    self = zhashx_new ();
}

Zhashx::Zhashx (zhashx_t *self_) {
    self = self_;
}

Zhashx::~Zhashx () {
}

NAN_METHOD (Zhashx::New) {
    assert (info.IsConstructCall ());
    Zhashx *zhashx = new Zhashx ();
    if (zhashx) {
        zhashx->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zhashx::destroy) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zhashx_destroy (&zhashx->self);
}


NAN_METHOD (Zhashx::defined) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zhashx->self != NULL));
}

NAN_METHOD (Zhashx::_size) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    size_t result = zhashx_size (zhashx->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhashx::_keys) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zlistx_t *result = zhashx_keys (zhashx->self);
    Zlistx *zlistx_result = new Zlistx (result);
    if (zlistx_result) {
    //  Don't yet know how to return a new object
    //      zlistx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhashx::_values) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zlistx_t *result = zhashx_values (zhashx->self);
    Zlistx *zlistx_result = new Zlistx (result);
    if (zlistx_result) {
    //  Don't yet know how to return a new object
    //      zlistx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhashx::_save) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zhashx_save (zhashx->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhashx::_load) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    char *filename;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `filename`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`filename` must be a string");
    else {
        Nan::Utf8String filename_utf8 (info [0].As<String>());
        filename = *filename_utf8;
    }
    int result = zhashx_load (zhashx->self, (const char *)filename);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhashx::_refresh) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    int result = zhashx_refresh (zhashx->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zhashx::_pack) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zframe_t *result = zhashx_pack (zhashx->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhashx::_dup) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zhashx_t *result = zhashx_dup (zhashx->self);
    Zhashx *zhashx_result = new Zhashx (result);
    if (zhashx_result) {
    //  Don't yet know how to return a new object
    //      zhashx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zhashx::_dup_v2) {
    Zhashx *zhashx = Nan::ObjectWrap::Unwrap <Zhashx> (info.Holder ());
    zhashx_t *result = zhashx_dup_v2 (zhashx->self);
    Zhashx *zhashx_result = new Zhashx (result);
    if (zhashx_result) {
    //  Don't yet know how to return a new object
    //      zhashx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

Nan::Persistent <Function> &Zhashx::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Ziflist::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Ziflist").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "first", _first);
    Nan::SetPrototypeMethod (tpl, "next", _next);
    Nan::SetPrototypeMethod (tpl, "address", _address);
    Nan::SetPrototypeMethod (tpl, "broadcast", _broadcast);
    Nan::SetPrototypeMethod (tpl, "netmask", _netmask);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Ziflist").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Ziflist::Ziflist (void) {
    self = ziflist_new ();
}

Ziflist::Ziflist (ziflist_t *self_) {
    self = self_;
}

Ziflist::~Ziflist () {
}

NAN_METHOD (Ziflist::New) {
    assert (info.IsConstructCall ());
    Ziflist *ziflist = new Ziflist ();
    if (ziflist) {
        ziflist->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Ziflist::destroy) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    ziflist_destroy (&ziflist->self);
}


NAN_METHOD (Ziflist::defined) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (ziflist->self != NULL));
}

NAN_METHOD (Ziflist::_size) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    size_t result = ziflist_size (ziflist->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Ziflist::_first) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    char *result = (char *) ziflist_first (ziflist->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Ziflist::_next) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    char *result = (char *) ziflist_next (ziflist->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Ziflist::_address) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    char *result = (char *) ziflist_address (ziflist->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Ziflist::_broadcast) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    char *result = (char *) ziflist_broadcast (ziflist->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Ziflist::_netmask) {
    Ziflist *ziflist = Nan::ObjectWrap::Unwrap <Ziflist> (info.Holder ());
    char *result = (char *) ziflist_netmask (ziflist->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Ziflist::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zlist::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zlist").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "size", _size);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zlist").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zlist::Zlist (void) {
    self = zlist_new ();
}

Zlist::Zlist (zlist_t *self_) {
    self = self_;
}

Zlist::~Zlist () {
}

NAN_METHOD (Zlist::New) {
    assert (info.IsConstructCall ());
    Zlist *zlist = new Zlist ();
    if (zlist) {
        zlist->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zlist::destroy) {
    Zlist *zlist = Nan::ObjectWrap::Unwrap <Zlist> (info.Holder ());
    zlist_destroy (&zlist->self);
}


NAN_METHOD (Zlist::defined) {
    Zlist *zlist = Nan::ObjectWrap::Unwrap <Zlist> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zlist->self != NULL));
}

NAN_METHOD (Zlist::_dup) {
    Zlist *zlist = Nan::ObjectWrap::Unwrap <Zlist> (info.Holder ());
    zlist_t *result = zlist_dup (zlist->self);
    Zlist *zlist_result = new Zlist (result);
    if (zlist_result) {
    //  Don't yet know how to return a new object
    //      zlist->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zlist::_size) {
    Zlist *zlist = Nan::ObjectWrap::Unwrap <Zlist> (info.Holder ());
    size_t result = zlist_size (zlist->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

Nan::Persistent <Function> &Zlist::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zlistx::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zlistx").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zlistx").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zlistx::Zlistx (void) {
    self = zlistx_new ();
}

Zlistx::Zlistx (zlistx_t *self_) {
    self = self_;
}

Zlistx::~Zlistx () {
}

NAN_METHOD (Zlistx::New) {
    assert (info.IsConstructCall ());
    Zlistx *zlistx = new Zlistx ();
    if (zlistx) {
        zlistx->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zlistx::destroy) {
    Zlistx *zlistx = Nan::ObjectWrap::Unwrap <Zlistx> (info.Holder ());
    zlistx_destroy (&zlistx->self);
}


NAN_METHOD (Zlistx::defined) {
    Zlistx *zlistx = Nan::ObjectWrap::Unwrap <Zlistx> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zlistx->self != NULL));
}

NAN_METHOD (Zlistx::_size) {
    Zlistx *zlistx = Nan::ObjectWrap::Unwrap <Zlistx> (info.Holder ());
    size_t result = zlistx_size (zlistx->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zlistx::_dup) {
    Zlistx *zlistx = Nan::ObjectWrap::Unwrap <Zlistx> (info.Holder ());
    zlistx_t *result = zlistx_dup (zlistx->self);
    Zlistx *zlistx_result = new Zlistx (result);
    if (zlistx_result) {
    //  Don't yet know how to return a new object
    //      zlistx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

Nan::Persistent <Function> &Zlistx::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zloop::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zloop").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "timerEnd", _timer_end);
    Nan::SetPrototypeMethod (tpl, "start", _start);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zloop").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zloop::Zloop (void) {
    self = zloop_new ();
}

Zloop::Zloop (zloop_t *self_) {
    self = self_;
}

Zloop::~Zloop () {
}

NAN_METHOD (Zloop::New) {
    assert (info.IsConstructCall ());
    Zloop *zloop = new Zloop ();
    if (zloop) {
        zloop->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zloop::destroy) {
    Zloop *zloop = Nan::ObjectWrap::Unwrap <Zloop> (info.Holder ());
    zloop_destroy (&zloop->self);
}


NAN_METHOD (Zloop::defined) {
    Zloop *zloop = Nan::ObjectWrap::Unwrap <Zloop> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zloop->self != NULL));
}

NAN_METHOD (Zloop::_timer_end) {
    Zloop *zloop = Nan::ObjectWrap::Unwrap <Zloop> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `timer id`");
    else
    if (!info [0]->IsNumber ())
        return Nan::ThrowTypeError ("`timer id` must be a number");
    int timer_id = Nan::To<int>(info [0]).FromJust ();

    int result = zloop_timer_end (zloop->self, (int)timer_id);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zloop::_start) {
    Zloop *zloop = Nan::ObjectWrap::Unwrap <Zloop> (info.Holder ());
    int result = zloop_start (zloop->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

Nan::Persistent <Function> &Zloop::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zmsg::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zmsg").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "contentSize", _content_size);
    Nan::SetPrototypeMethod (tpl, "routingId", _routing_id);
    Nan::SetPrototypeMethod (tpl, "prepend", _prepend);
    Nan::SetPrototypeMethod (tpl, "append", _append);
    Nan::SetPrototypeMethod (tpl, "pop", _pop);
    Nan::SetPrototypeMethod (tpl, "pushstr", _pushstr);
    Nan::SetPrototypeMethod (tpl, "addstr", _addstr);
    Nan::SetPrototypeMethod (tpl, "pushstrf", _pushstrf);
    Nan::SetPrototypeMethod (tpl, "addstrf", _addstrf);
    Nan::SetPrototypeMethod (tpl, "popstr", _popstr);
    Nan::SetPrototypeMethod (tpl, "addmsg", _addmsg);
    Nan::SetPrototypeMethod (tpl, "popmsg", _popmsg);
    Nan::SetPrototypeMethod (tpl, "first", _first);
    Nan::SetPrototypeMethod (tpl, "next", _next);
    Nan::SetPrototypeMethod (tpl, "last", _last);
    Nan::SetPrototypeMethod (tpl, "encode", _encode);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);
    Nan::SetPrototypeMethod (tpl, "eq", _eq);
    Nan::SetPrototypeMethod (tpl, "signal", _signal);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zmsg").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zmsg::Zmsg (void) {
    self = zmsg_new ();
}

Zmsg::Zmsg (zmsg_t *self_) {
    self = self_;
}

Zmsg::~Zmsg () {
}

NAN_METHOD (Zmsg::New) {
    assert (info.IsConstructCall ());
    Zmsg *zmsg = new Zmsg ();
    if (zmsg) {
        zmsg->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zmsg::destroy) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zmsg_destroy (&zmsg->self);
}


NAN_METHOD (Zmsg::defined) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zmsg->self != NULL));
}

NAN_METHOD (Zmsg::_size) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    size_t result = zmsg_size (zmsg->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_content_size) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    size_t result = zmsg_content_size (zmsg->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_routing_id) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    uint32_t result = zmsg_routing_id (zmsg->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_prepend) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    Zframe *frame_p = Nan::ObjectWrap::Unwrap<Zframe>(info [0].As<Object>());
    int result = zmsg_prepend (zmsg->self, &frame_p->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_append) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    Zframe *frame_p = Nan::ObjectWrap::Unwrap<Zframe>(info [0].As<Object>());
    int result = zmsg_append (zmsg->self, &frame_p->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_pop) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zframe_t *result = zmsg_pop (zmsg->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_pushstr) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    char *string;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `string`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`string` must be a string");
    else {
        Nan::Utf8String string_utf8 (info [0].As<String>());
        string = *string_utf8;
    }
    int result = zmsg_pushstr (zmsg->self, (const char *)string);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_addstr) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    char *string;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `string`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`string` must be a string");
    else {
        Nan::Utf8String string_utf8 (info [0].As<String>());
        string = *string_utf8;
    }
    int result = zmsg_addstr (zmsg->self, (const char *)string);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_pushstrf) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zmsg_pushstrf (zmsg->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_addstrf) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zmsg_addstrf (zmsg->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_popstr) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    char *result = (char *) zmsg_popstr (zmsg->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zmsg::_addmsg) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    Zmsg *msg_p = Nan::ObjectWrap::Unwrap<Zmsg>(info [0].As<Object>());
    int result = zmsg_addmsg (zmsg->self, &msg_p->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zmsg::_popmsg) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zmsg_t *result = zmsg_popmsg (zmsg->self);
    Zmsg *zmsg_result = new Zmsg (result);
    if (zmsg_result) {
    //  Don't yet know how to return a new object
    //      zmsg->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_first) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zframe_t *result = zmsg_first (zmsg->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_next) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zframe_t *result = zmsg_next (zmsg->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_last) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zframe_t *result = zmsg_last (zmsg->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_encode) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zframe_t *result = zmsg_encode (zmsg->self);
    Zframe *zframe_result = new Zframe (result);
    if (zframe_result) {
    //  Don't yet know how to return a new object
    //      zframe->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_dup) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    zmsg_t *result = zmsg_dup (zmsg->self);
    Zmsg *zmsg_result = new Zmsg (result);
    if (zmsg_result) {
    //  Don't yet know how to return a new object
    //      zmsg->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Zmsg::_eq) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    Zmsg *other = Nan::ObjectWrap::Unwrap<Zmsg>(info [0].As<Object>());
    bool result = zmsg_eq (zmsg->self, other->self);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zmsg::_signal) {
    Zmsg *zmsg = Nan::ObjectWrap::Unwrap <Zmsg> (info.Holder ());
    int result = zmsg_signal (zmsg->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

Nan::Persistent <Function> &Zmsg::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zproc::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zproc").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "czmqVersion", _czmq_version);
    Nan::SetPrototypeMethod (tpl, "interrupted", _interrupted);
    Nan::SetPrototypeMethod (tpl, "hasCurve", _has_curve);
    Nan::SetPrototypeMethod (tpl, "hostname", _hostname);
    Nan::SetPrototypeMethod (tpl, "biface", _biface);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zproc").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zproc::Zproc () {
}

Zproc::~Zproc () {
}

NAN_METHOD (Zproc::New) {
    assert (info.IsConstructCall ());
    Zproc *zproc = new Zproc ();
    if (zproc) {
        zproc->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zproc::_czmq_version) {
    int result = zproc_czmq_version ();
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zproc::_interrupted) {
    bool result = zproc_interrupted ();
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zproc::_has_curve) {
    bool result = zproc_has_curve ();
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zproc::_hostname) {
    char *result = (char *) zproc_hostname ();
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zproc::_biface) {
    char *result = (char *) zproc_biface ();
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zproc::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zsock::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zsock").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "bind", _bind);
    Nan::SetPrototypeMethod (tpl, "endpoint", _endpoint);
    Nan::SetPrototypeMethod (tpl, "unbind", _unbind);
    Nan::SetPrototypeMethod (tpl, "connect", _connect);
    Nan::SetPrototypeMethod (tpl, "disconnect", _disconnect);
    Nan::SetPrototypeMethod (tpl, "attach", _attach);
    Nan::SetPrototypeMethod (tpl, "typeStr", _type_str);
    Nan::SetPrototypeMethod (tpl, "send", _send);
    Nan::SetPrototypeMethod (tpl, "recv", _recv);
    Nan::SetPrototypeMethod (tpl, "bsend", _bsend);
    Nan::SetPrototypeMethod (tpl, "brecv", _brecv);
    Nan::SetPrototypeMethod (tpl, "routingId", _routing_id);
    Nan::SetPrototypeMethod (tpl, "wait", _wait);
    Nan::SetPrototypeMethod (tpl, "join", _join);
    Nan::SetPrototypeMethod (tpl, "leave", _leave);
    Nan::SetPrototypeMethod (tpl, "heartbeatIvl", _heartbeat_ivl);
    Nan::SetPrototypeMethod (tpl, "heartbeatTtl", _heartbeat_ttl);
    Nan::SetPrototypeMethod (tpl, "heartbeatTimeout", _heartbeat_timeout);
    Nan::SetPrototypeMethod (tpl, "useFd", _use_fd);
    Nan::SetPrototypeMethod (tpl, "tos", _tos);
    Nan::SetPrototypeMethod (tpl, "zapDomain", _zap_domain);
    Nan::SetPrototypeMethod (tpl, "mechanism", _mechanism);
    Nan::SetPrototypeMethod (tpl, "plainServer", _plain_server);
    Nan::SetPrototypeMethod (tpl, "plainUsername", _plain_username);
    Nan::SetPrototypeMethod (tpl, "plainPassword", _plain_password);
    Nan::SetPrototypeMethod (tpl, "curveServer", _curve_server);
    Nan::SetPrototypeMethod (tpl, "curvePublickey", _curve_publickey);
    Nan::SetPrototypeMethod (tpl, "curveSecretkey", _curve_secretkey);
    Nan::SetPrototypeMethod (tpl, "curveServerkey", _curve_serverkey);
    Nan::SetPrototypeMethod (tpl, "gssapiServer", _gssapi_server);
    Nan::SetPrototypeMethod (tpl, "gssapiPlaintext", _gssapi_plaintext);
    Nan::SetPrototypeMethod (tpl, "gssapiPrincipal", _gssapi_principal);
    Nan::SetPrototypeMethod (tpl, "gssapiServicePrincipal", _gssapi_service_principal);
    Nan::SetPrototypeMethod (tpl, "ipv6", _ipv6);
    Nan::SetPrototypeMethod (tpl, "immediate", _immediate);
    Nan::SetPrototypeMethod (tpl, "ipv4only", _ipv4only);
    Nan::SetPrototypeMethod (tpl, "type", _type);
    Nan::SetPrototypeMethod (tpl, "sndhwm", _sndhwm);
    Nan::SetPrototypeMethod (tpl, "rcvhwm", _rcvhwm);
    Nan::SetPrototypeMethod (tpl, "affinity", _affinity);
    Nan::SetPrototypeMethod (tpl, "identity", _identity);
    Nan::SetPrototypeMethod (tpl, "rate", _rate);
    Nan::SetPrototypeMethod (tpl, "recoveryIvl", _recovery_ivl);
    Nan::SetPrototypeMethod (tpl, "sndbuf", _sndbuf);
    Nan::SetPrototypeMethod (tpl, "rcvbuf", _rcvbuf);
    Nan::SetPrototypeMethod (tpl, "linger", _linger);
    Nan::SetPrototypeMethod (tpl, "reconnectIvl", _reconnect_ivl);
    Nan::SetPrototypeMethod (tpl, "reconnectIvlMax", _reconnect_ivl_max);
    Nan::SetPrototypeMethod (tpl, "backlog", _backlog);
    Nan::SetPrototypeMethod (tpl, "maxmsgsize", _maxmsgsize);
    Nan::SetPrototypeMethod (tpl, "multicastHops", _multicast_hops);
    Nan::SetPrototypeMethod (tpl, "rcvtimeo", _rcvtimeo);
    Nan::SetPrototypeMethod (tpl, "sndtimeo", _sndtimeo);
    Nan::SetPrototypeMethod (tpl, "tcpKeepalive", _tcp_keepalive);
    Nan::SetPrototypeMethod (tpl, "tcpKeepaliveIdle", _tcp_keepalive_idle);
    Nan::SetPrototypeMethod (tpl, "tcpKeepaliveCnt", _tcp_keepalive_cnt);
    Nan::SetPrototypeMethod (tpl, "tcpKeepaliveIntvl", _tcp_keepalive_intvl);
    Nan::SetPrototypeMethod (tpl, "tcpAcceptFilter", _tcp_accept_filter);
    Nan::SetPrototypeMethod (tpl, "rcvmore", _rcvmore);
    Nan::SetPrototypeMethod (tpl, "events", _events);
    Nan::SetPrototypeMethod (tpl, "lastEndpoint", _last_endpoint);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zsock").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zsock::Zsock (int type) {
    self = zsock_new ((int)type);
}

Zsock::Zsock (zsock_t *self_) {
    self = self_;
}

Zsock::~Zsock () {
}

NAN_METHOD (Zsock::New) {
    assert (info.IsConstructCall ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `type`");
    else
    if (!info [0]->IsNumber ())
        return Nan::ThrowTypeError ("`type` must be a number");
    int type = Nan::To<int>(info [0]).FromJust ();

    Zsock *zsock = new Zsock ((int)type);
    if (zsock) {
        zsock->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zsock::destroy) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    zsock_destroy (&zsock->self);
}


NAN_METHOD (Zsock::defined) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zsock->self != NULL));
}

NAN_METHOD (Zsock::_bind) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zsock_bind (zsock->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_endpoint) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_endpoint (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_unbind) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zsock_unbind (zsock->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_connect) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zsock_connect (zsock->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_disconnect) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *format;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `format`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`format` must be a string");
    else {
        Nan::Utf8String format_utf8 (info [0].As<String>());
        format = *format_utf8;
    }
    int result = zsock_disconnect (zsock->self, "%s", format);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_attach) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *endpoints;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `endpoints`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`endpoints` must be a string");
    else {
        Nan::Utf8String endpoints_utf8 (info [0].As<String>());
        endpoints = *endpoints_utf8;
    }
    if (info [1]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `serverish`");
    else
    if (!info [1]->IsBoolean ())
        return Nan::ThrowTypeError ("`serverish` must be a number");
    bool serverish = Nan::To<bool>(info [1]).FromJust ();

    int result = zsock_attach (zsock->self, (const char *)endpoints, (bool)serverish);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_type_str) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_type_str (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_send) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *picture;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `picture`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`picture` must be a string");
    else {
        Nan::Utf8String picture_utf8 (info [0].As<String>());
        picture = *picture_utf8;
    }
    int result = zsock_send (zsock->self, (const char *)picture);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_recv) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *picture;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `picture`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`picture` must be a string");
    else {
        Nan::Utf8String picture_utf8 (info [0].As<String>());
        picture = *picture_utf8;
    }
    int result = zsock_recv (zsock->self, (const char *)picture);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_bsend) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *picture;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `picture`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`picture` must be a string");
    else {
        Nan::Utf8String picture_utf8 (info [0].As<String>());
        picture = *picture_utf8;
    }
    int result = zsock_bsend (zsock->self, (const char *)picture);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_brecv) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *picture;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `picture`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`picture` must be a string");
    else {
        Nan::Utf8String picture_utf8 (info [0].As<String>());
        picture = *picture_utf8;
    }
    int result = zsock_brecv (zsock->self, (const char *)picture);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_routing_id) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    uint32_t result = zsock_routing_id (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_wait) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_wait (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_join) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *group;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `group`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`group` must be a string");
    else {
        Nan::Utf8String group_utf8 (info [0].As<String>());
        group = *group_utf8;
    }
    int result = zsock_join (zsock->self, (const char *)group);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_leave) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *group;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `group`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`group` must be a string");
    else {
        Nan::Utf8String group_utf8 (info [0].As<String>());
        group = *group_utf8;
    }
    int result = zsock_leave (zsock->self, (const char *)group);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_heartbeat_ivl) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_heartbeat_ivl (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_heartbeat_ttl) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_heartbeat_ttl (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_heartbeat_timeout) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_heartbeat_timeout (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_use_fd) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_use_fd (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tos) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_tos (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_zap_domain) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_zap_domain (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_mechanism) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_mechanism (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_plain_server) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_plain_server (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_plain_username) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_plain_username (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_plain_password) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_plain_password (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_curve_server) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_curve_server (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_curve_publickey) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_curve_publickey (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_curve_secretkey) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_curve_secretkey (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_curve_serverkey) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_curve_serverkey (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_gssapi_server) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_gssapi_server (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_gssapi_plaintext) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_gssapi_plaintext (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_gssapi_principal) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_gssapi_principal (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_gssapi_service_principal) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_gssapi_service_principal (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_ipv6) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_ipv6 (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_immediate) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_immediate (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_ipv4only) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_ipv4only (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_type) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_type (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_sndhwm) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_sndhwm (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_rcvhwm) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_rcvhwm (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_affinity) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_affinity (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_identity) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_identity (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_rate) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_rate (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_recovery_ivl) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_recovery_ivl (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_sndbuf) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_sndbuf (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_rcvbuf) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_rcvbuf (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_linger) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_linger (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_reconnect_ivl) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_reconnect_ivl (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_reconnect_ivl_max) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_reconnect_ivl_max (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_backlog) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_backlog (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_maxmsgsize) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_maxmsgsize (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_multicast_hops) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_multicast_hops (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_rcvtimeo) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_rcvtimeo (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_sndtimeo) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_sndtimeo (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tcp_keepalive) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_tcp_keepalive (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tcp_keepalive_idle) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_tcp_keepalive_idle (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tcp_keepalive_cnt) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_tcp_keepalive_cnt (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tcp_keepalive_intvl) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_tcp_keepalive_intvl (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_tcp_accept_filter) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_tcp_accept_filter (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zsock::_rcvmore) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_rcvmore (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_events) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    int result = zsock_events (zsock->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zsock::_last_endpoint) {
    Zsock *zsock = Nan::ObjectWrap::Unwrap <Zsock> (info.Holder ());
    char *result = (char *) zsock_last_endpoint (zsock->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Zsock::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zstr::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zstr").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zstr").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zstr::Zstr () {
}

Zstr::~Zstr () {
}

NAN_METHOD (Zstr::New) {
    assert (info.IsConstructCall ());
    Zstr *zstr = new Zstr ();
    if (zstr) {
        zstr->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

Nan::Persistent <Function> &Zstr::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Ztrie::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Ztrie").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "removeRoute", _remove_route);
    Nan::SetPrototypeMethod (tpl, "matches", _matches);
    Nan::SetPrototypeMethod (tpl, "hitParameterCount", _hit_parameter_count);
    Nan::SetPrototypeMethod (tpl, "hitParameters", _hit_parameters);
    Nan::SetPrototypeMethod (tpl, "hitAsteriskMatch", _hit_asterisk_match);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Ztrie").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Ztrie::Ztrie (char delimiter) {
    self = ztrie_new ((char)delimiter);
}

Ztrie::Ztrie (ztrie_t *self_) {
    self = self_;
}

Ztrie::~Ztrie () {
}

NAN_METHOD (Ztrie::New) {
    assert (info.IsConstructCall ());
    char delimiter;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `delimiter`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`delimiter` must be a string");
    else {
        Nan::Utf8String delimiter_utf8 (info [0].As<String>());
        if (strlen (*delimiter_utf8) != 1)
            return Nan::ThrowTypeError ("`delimiter` must be a single character");
        delimiter = (*delimiter_utf8) [0];
    }
    Ztrie *ztrie = new Ztrie ((char)delimiter);
    if (ztrie) {
        ztrie->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Ztrie::destroy) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    ztrie_destroy (&ztrie->self);
}


NAN_METHOD (Ztrie::defined) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (ztrie->self != NULL));
}

NAN_METHOD (Ztrie::_remove_route) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    int result = ztrie_remove_route (ztrie->self, (const char *)path);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Ztrie::_matches) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    char *path;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `path`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`path` must be a string");
    else {
        Nan::Utf8String path_utf8 (info [0].As<String>());
        path = *path_utf8;
    }
    bool result = ztrie_matches (ztrie->self, (const char *)path);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Ztrie::_hit_parameter_count) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    size_t result = ztrie_hit_parameter_count (ztrie->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Ztrie::_hit_parameters) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    zhashx_t *result = ztrie_hit_parameters (ztrie->self);
    Zhashx *zhashx_result = new Zhashx (result);
    if (zhashx_result) {
    //  Don't yet know how to return a new object
    //      zhashx->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

NAN_METHOD (Ztrie::_hit_asterisk_match) {
    Ztrie *ztrie = Nan::ObjectWrap::Unwrap <Ztrie> (info.Holder ());
    char *result = (char *) ztrie_hit_asterisk_match (ztrie->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

Nan::Persistent <Function> &Ztrie::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


NAN_MODULE_INIT (Zuuid::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local <FunctionTemplate> tpl = Nan::New <FunctionTemplate> (New);
    tpl->SetClassName (Nan::New ("Zuuid").ToLocalChecked ());
    tpl->InstanceTemplate ()->SetInternalFieldCount (1);

    // Prototypes
    Nan::SetPrototypeMethod (tpl, "destroy", destroy);
    Nan::SetPrototypeMethod (tpl, "defined", defined);
    Nan::SetPrototypeMethod (tpl, "setStr", _set_str);
    Nan::SetPrototypeMethod (tpl, "data", _data);
    Nan::SetPrototypeMethod (tpl, "size", _size);
    Nan::SetPrototypeMethod (tpl, "str", _str);
    Nan::SetPrototypeMethod (tpl, "strCanonical", _str_canonical);
    Nan::SetPrototypeMethod (tpl, "eq", _eq);
    Nan::SetPrototypeMethod (tpl, "neq", _neq);
    Nan::SetPrototypeMethod (tpl, "dup", _dup);

    constructor ().Reset (Nan::GetFunction (tpl).ToLocalChecked ());
    Nan::Set (target, Nan::New ("Zuuid").ToLocalChecked (),
    Nan::GetFunction (tpl).ToLocalChecked ());
}

Zuuid::Zuuid (void) {
    self = zuuid_new ();
}

Zuuid::Zuuid (zuuid_t *self_) {
    self = self_;
}

Zuuid::~Zuuid () {
}

NAN_METHOD (Zuuid::New) {
    assert (info.IsConstructCall ());
    Zuuid *zuuid = new Zuuid ();
    if (zuuid) {
        zuuid->Wrap (info.This ());
        info.GetReturnValue ().Set (info.This ());
    }
}

NAN_METHOD (Zuuid::destroy) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    zuuid_destroy (&zuuid->self);
}


NAN_METHOD (Zuuid::defined) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    info.GetReturnValue ().Set (Nan::New (zuuid->self != NULL));
}

NAN_METHOD (Zuuid::_set_str) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    char *source;
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a `source`");
    else
    if (!info [0]->IsString ())
        return Nan::ThrowTypeError ("`source` must be a string");
    else {
        Nan::Utf8String source_utf8 (info [0].As<String>());
        source = *source_utf8;
    }
    int result = zuuid_set_str (zuuid->self, (const char *)source);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zuuid::_data) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    const char *result = (const char *) zuuid_data (zuuid->self);
    info.GetReturnValue().Set (Nan::CopyBuffer (result, zuuid_size (zuuid->self)).ToLocalChecked ());
}

NAN_METHOD (Zuuid::_size) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    size_t result = zuuid_size (zuuid->self);
    info.GetReturnValue ().Set (Nan::New<Number>(result));
}

NAN_METHOD (Zuuid::_str) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    char *result = (char *) zuuid_str (zuuid->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zuuid::_str_canonical) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    char *result = (char *) zuuid_str_canonical (zuuid->self);
    info.GetReturnValue ().Set (Nan::New (result).ToLocalChecked ());
}

NAN_METHOD (Zuuid::_eq) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *compare = (const byte *) node::Buffer::Data (buffer_node);
    bool result = zuuid_eq (zuuid->self, (const byte *)compare);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zuuid::_neq) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    if (info [0]->IsUndefined ())
        return Nan::ThrowTypeError ("method requires a argument to provide data");
    Local<Object> buffer_node = info [0].As<Object> ();
    const byte *compare = (const byte *) node::Buffer::Data (buffer_node);
    bool result = zuuid_neq (zuuid->self, (const byte *)compare);
    info.GetReturnValue ().Set (Nan::New<Boolean>(result));
}

NAN_METHOD (Zuuid::_dup) {
    Zuuid *zuuid = Nan::ObjectWrap::Unwrap <Zuuid> (info.Holder ());
    zuuid_t *result = zuuid_dup (zuuid->self);
    Zuuid *zuuid_result = new Zuuid (result);
    if (zuuid_result) {
    //  Don't yet know how to return a new object
    //      zuuid->Wrap (info.This ());
    //      info.GetReturnValue ().Set (info.This ());
        info.GetReturnValue ().Set (Nan::New<Boolean>(true));
    }
}

Nan::Persistent <Function> &Zuuid::constructor () {
    static Nan::Persistent <Function> my_constructor;
    return my_constructor;
}


extern "C" NAN_MODULE_INIT (czmq_initialize)
{
    Zarmour::Init (target);
    Zcert::Init (target);
    Zcertstore::Init (target);
    Zchunk::Init (target);
    Zclock::Init (target);
    Zconfig::Init (target);
    Zdigest::Init (target);
    Zdir::Init (target);
    ZdirPatch::Init (target);
    Zfile::Init (target);
    Zframe::Init (target);
    Zhash::Init (target);
    Zhashx::Init (target);
    Ziflist::Init (target);
    Zlist::Init (target);
    Zlistx::Init (target);
    Zloop::Init (target);
    Zmsg::Init (target);
    Zproc::Init (target);
    Zsock::Init (target);
    Zstr::Init (target);
    Ztrie::Init (target);
    Zuuid::Init (target);
}

NODE_MODULE (czmq, czmq_initialize)