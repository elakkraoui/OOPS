/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Value.h
/// @author Manuel Fuentes
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date Jun 97

#ifndef eckit_Value_h
#define eckit_Value_h

#include "eckit/value/Content.h"
#include "eckit/types/Date.h"
#include "eckit/types/DateTime.h"

#include "eckit/filesystem/PathName.h"
#include "eckit/io/Length.h"
#include "eckit/io/Offset.h"

namespace eckit {

//----------------------------------------------------------------------------------------------------------------------

/// Known issues
/// ============
///
/// NOTE: Objectives included not breaking eckit (including the testsuite). Therefore the failing unit tests are
///       commented with ///.
///
/// 1. Value(const Time&) and Value(const DateTime&) are unimplemented, even though exposed in the header.
///
/// 2. Length(val) or (Length)val fail due to an ambiguity in the Length constructor, which doesn't know whether to
///    cast via Length or long long. The explicit ::as<Length>() cast doesn't exist. Same with the Date() constructor.
///
///    The definition of operator= differs from the constructors, so that it is possible to do:
///
///    Length len = val;
///
/// 3. Value(bool) happily converts to a double, even explicitly with ::as<double>(). See BoolContent.cc/h
///
/// 4. Length() and Offset() are supposed to define a well-defined algebra (which avoids errors such as adding two
///    offsets). Within a Value they are stored as raw long long types, and no checking is performed when casting. Any
///    numeric value (including negative values) may be used silently as Length/Offsets.
///
/// 5. unsigned long long values are stored in signed long longs, resulting in integer overflows at (potentially) half
///    the anticipated value.
///
/// 6. Cannot initialise (copy) one ValueList with another using the implicit casts. ValueList is a std::vector, and
///    there are two ambiguous constructors with appropriate overloads provided by Value
///
///    std::vector<Value>( std::vector<Value>& rhs );
///    std::vector<Value>( size_type n );
///
///    Either:
///
///      - Assign using operator=
///      - Explicitly cast using ::as<ValueList>()
///
/// 7. Operators ==, >, <, >=, <= act on the memory address of the allocated internal Content object, and have nothing
///    to do with the content of the value. The compare() method should be used instead.
///
///    Value(true) != Value(true)
///
/// 8. Value(bool)::compare() has inverted sense to the other compare methods
///
///    - It will return 0 if the Values are different
///    - It will return -1 if the values are true
///    - It will return 1 if the values are false
///
/// 9. On conversion falure Value(std::string> --> long long, zero is returned rather than an exception being thrown.
///
/// 10. On conversion failure Value(std::string) --> double, a BadParameter exception is thrown, rather than a
///     BadConversion exception as for all other conversion failures (except the integer case noted above).
///
/// 11. Comparison operators for Value(Date), Value(Time), Value(DateTime) have the wrong sign in MapContent.h:66-68
///
/// 12. Equality testing for ValueMaps gives unintuitive results, as the maps contain copies of Values, and these will
///     have differing memory addresses (see (7)).
///
/// 13. operator[] discards constness of ValueMaps:
///
///         ValueMap vm;
///         const Value cv(vm);
///         cv[10];
///
///         Log::info() << cv; // This gives {10 => (nil)}
///
/// 14. Indexing ValueMaps with Value(bool) doesn't work. I suspect this is due to point (8).
///
/// 15. Indexing Value(ValueList) with std::string, or Value(std::string) spuriously returns element zero, rather than
///     hitting an assertion or throwing another exception. This is a result of Value(std::string) silently casting any
///     string to zero if used as an integer.
///
///     ValueList vl;
///     vl.push_back(123);
///     Value val(vl);
///
///     Log::info() << val["hello"]; // This prints "123".
///
/// 16. Similarly, Value(ValueList)[Value(bool)] returns element zero or one depending on the content type.
///
/// 17. Value(ValueList)::contains() should probably return false, rather than an exception, if indexed with bools,
///     floats, ...
///
/// 18. Although they are otherwise freely interconvertible, the arithmetic operators don't work between Number and
///     Double types.
///
/// 19. Although the mod() function is implemented on NumberContent, the corresponding modNumber() routine is not, and
///     so BadOperator is returned incorrectly.
///
///     More generally, the modulus functionality does not seem to be implemented for ANY of the Value() types.
///
/// 20. Subtraction operators for Value(Date) have a sign error. A later date minus a newer date should be positive.
///
/// 21. compare() function for Value(Date()) is buggy. Currently:
///
///     date1 == date2 --> 1
///     date1 <  date2 --> -1
///     date1 >  date2 --> 0
///
/// 22. The Value::head() and Value::tail() members make an entire copy of the contained ValueList before selecting
///     and returning the head/tail element (which is copied). This will involve head allocations (and then
///     deallocations during cleanup) for every contained element.
///
///       -- This seems crazy.
///       -- Accessing the first element of a list shouldn't be O[N] in cpu and memory...
///
/// 23. The head() and tail() methods are clearly intended to be used on Value(ValueList) types, but if called on any
///     other Value(X), it will create a single element list, and then return the first (and only) element.
///
/// 24. The tail() method returns the entire list, excluding the first element (or an empty Value(), not an empty list,
///     if there are no remaining elements).
///
///     -- This is inconsistent with the naming, and the behaviour of head()
///     -- It is closer to the car/cdr or first/rest functionality in lisp
///     -- It should certainly at least return an empty list, not a nil value

//----------------------------------------------------------------------------------------------------------------------

class Length;
class PathName;
class JSON;

class Value {
public:

// -- Contructors

    Value();

    Value(bool);
    Value(int);
    Value(long);
    Value(long long);
    Value(unsigned int);
    Value(unsigned long);
    Value(unsigned long long);
    Value(double);

    Value(const std::string&);
    Value(const char*);

    Value(const Length&);
    Value(const Date&);
    Value(const Time&);
    Value(const DateTime&);
    Value(const PathName&);

    Value(Stream&);
    Value(const ValueList&);
    Value(const ValueMap&);

// -- Copy

	Value(const Value&);
	Value& operator=(const Value&);

// -- Destructor

	~Value();

// -- Operators

    /// Explicitly cast value to the given type. For list of supported types, see the definitions of the
    /// member function value() in eckit/value/Content.h
    template< typename T > T as() const { T r; content_->value(r); return r; }

    operator short() const              { long long l;  content_->value(l); return l; }
    operator unsigned short() const     { long long l;  content_->value(l); return l; }

    operator int() const                { long long l;  content_->value(l); return l; }
    operator unsigned int() const       { long long l;  content_->value(l); return l; }

    operator long() const               { long long l;  content_->value(l); return l; }
    operator unsigned long() const      { long long l;  content_->value(l); return l; }

    operator long long() const          { long long l;  content_->value(l); return l; }
    operator unsigned long long() const { long long l;  content_->value(l); return l; }

    operator double() const             { double d;     content_->value(d); return d; }
    operator bool() const               { bool d;       content_->value(d); return d; }

    operator std::string() const  { std::string s; content_->value(s); return s; }
    operator PathName() const     { std::string s; content_->value(s); return s; }

    operator Date() const      { Date d; content_->value(d); return d; }
    operator Time() const      { Time t; content_->value(t); return t; }
    operator DateTime() const  { DateTime d; content_->value(d); return d; }

    operator Length() const    { long long l;  content_->value(l); return l; }
    operator Offset() const    { long long l;  content_->value(l); return l; }

    operator ValueList() const;
    operator ValueMap() const;

    bool operator<(const Value& v) const  { return *content_ < *(v.content_); }
    bool operator==(const Value& v) const { return *content_ == *(v.content_); }

    bool operator>(const Value& v) const  { return v < *this;     }
    bool operator!=(const Value& v) const { return !(*this == v); }

    bool operator>=(const Value& v) const  { return !(*this < v);     }
    bool operator<=(const Value& v) const  { return !(v < *this);     }


    Value operator+(const Value&) const;
    Value& operator+=(const Value&);

    Value operator-() const;

    Value operator-(const Value&) const;
    Value& operator-=(const Value&);

    Value operator*(const Value&) const;
    Value& operator*=(const Value&);

    Value operator/(const Value&) const;
    Value& operator/=(const Value&);

    Value operator%(const Value&) const;
    Value& operator%=(const Value&);

    const Value& operator[](const char*) const;
    const Value& operator[](const std::string&) const;
    const Value& operator[](const Value&) const;
    const Value& operator[](int) const;

    Value& operator[](const char*);
    Value& operator[](const std::string&);
    Value& operator[](const Value&);
    Value& operator[](int);

    Value keys() const;
    size_t size() const;

public:
    bool contains(const char*) const;
    bool contains(const std::string&) const;
    bool contains(const Value&) const;
    bool contains(int) const;

    Value& element(const std::string&);

    // -- Methods

    int      compare(const Value& v) const { return content_->compare(*(v.content_)); }

    bool     isNil()      const { return content_->isNil(); }
    bool     isNumber()   const { return content_->isNumber(); }
    bool     isBool()     const { return content_->isBool(); }
    bool     isDouble()   const { return content_->isDouble(); }
    bool     isString()   const { return content_->isString(); }
    bool     isList()     const { return content_->isList(); }
    bool     isMap()      const { return content_->isMap(); }
    bool     isDate()     const { return content_->isDate(); }
    bool     isTime()     const { return content_->isTime(); }
    bool     isDateTime() const { return content_->isDateTime(); }

    Value	 tail() const;
    Value	 head() const;

    Value    clone() const;
    bool     shared() const; // Ensure that value is not shared

// -- Class Methods

    static Value makeList();
    static Value makeList(const Value&);
    static Value makeList(const ValueList&);

    static Value makeMap();
    static Value makeMap(const ValueMap&);

protected:

	Value(Content*);

private: // members

	Content* content_;

private: // methods

    void json(JSON& s) const        { s << *content_; }
    void print(std::ostream& s) const    { s << *content_; }
    void encode(Stream& s) const    { s << *content_; }

    friend JSON& operator<<(JSON& s, const Value& v) { v.json(s);  return s; }
	friend std::ostream& operator<<(std::ostream& s, const Value& v) { v.print(s);  return s; }
	friend Stream&  operator<<(Stream&  s, const Value& v) { v.encode(s); return s; }

	friend class Content;

};

//----------------------------------------------------------------------------------------------------------------------

template < typename T >
Value makeVectorValue( const std::vector<T>& v )
{
    ValueList r;
    r.reserve(v.size());
    for( size_t i = 0; i < v.size(); ++i )
        r.push_back( Value(v[i]) );
    return Value::makeList(r);
}

template < typename T >
Value makeVectorValue( const std::list<T>& l )
{
    ValueList r;
    r.reserve(l.size());
    for(typename std::list<T>::const_iterator j = l.begin(); j != l.end(); ++j) {
        r.push_back( Value( *j ) );
    }
    return Value::makeList(r);
}

//----------------------------------------------------------------------------------------------------------------------

template <> struct VectorPrintSelector<Value> { typedef VectorPrintSimple selector; };

} // namespace eckit

#endif