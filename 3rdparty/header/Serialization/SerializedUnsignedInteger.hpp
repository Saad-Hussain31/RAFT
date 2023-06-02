#ifndef SERIALIZATION_SERIALIZED_OBJECT_UNSIGNED_INTEGER_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_UNSIGNED_INTEGER_HPP

/**
 * @file SerializedUnsignedInteger.hpp
 *
 * This module declares the
 * Serialization::SerializedUnsignedInteger class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <stdint.h>
#include <string>

namespace Serialization {

    /**
     * This class represents an unsigned integer which is going to be
     * serialized into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class SerializedUnsignedInteger
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef uint64_t Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::UnsignedInteger;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedUnsignedInteger() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedUnsignedInteger(uint64_t initialValue);

        /**
         * This is the typecast to unsigned int operator for the class.
         *
         * @return
         *     The integer state of the object is returned.
         */
        operator uint64_t() const;

        // ISerializedObject
    public:
        virtual bool Serialize(
            SystemAbstractions::IFile* file,
            unsigned int serializationVersion = 0
        ) const override;
        virtual bool Deserialize(SystemAbstractions::IFile* file) override;
        virtual std::string Render() const override;
        virtual bool Parse(std::string rendering) override;
        virtual bool IsEqualTo(const ISerializedObject* other) const override;

        // Private properties
    private:
        /**
         * This is the unsigned integer state of the object.
         */
        uint64_t value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_UNSIGNED_INTEGER_HPP */
