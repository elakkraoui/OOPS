/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date July 1996

#ifndef eckit_Buffer_h
#define eckit_Buffer_h

#include "eckit/eckit.h"

#include "eckit/memory/NonCopyable.h"


namespace eckit {

//----------------------------------------------------------------------------------------------------------------------

// A simple class to implement buffers

class Buffer : private NonCopyable {

public: // methods

    Buffer(size_t size);
    Buffer(const std::string& s);
    Buffer(const char*, size_t size);

    /// Constructor that does not take ownership of the data
    Buffer(void*, size_t size, bool dummy);

    ~Buffer();

    operator char*()                 { return (char*)buffer_; }
    operator const char*() const     { return (char*)buffer_; }

    operator void*()                 { return buffer_; }
    operator const void*() const     { return buffer_; }

    size_t size() const		 { return size_; }

    void resize(size_t size);

    void swap(Buffer& rhs);

protected: // methods

    void create();
    void destroy();

    void copy(const std::string& s);
    void copy(const char*,size_t size);

private: // members

    void*  buffer_;
    size_t size_;

    bool   owned_;

};

//----------------------------------------------------------------------------------------------------------------------

} // namespace eckit

#endif
