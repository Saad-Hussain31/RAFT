#ifndef SERIALIZATION_SERIALIZED_OBJECT_INTEGER_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_INTEGER_HPP

/**
 * @file SerializedInteger.hpp
 *
 * This module declares the Serialization::SerializedInteger
 * class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <string>

namespace Serialization {

    /**
     * This class represents an integer which is going to be serialized
     * into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class SerializedInteger
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef int Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::Integer;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedInteger() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedInteger(int initialValue);

        /**
         * This is the typecast to int operator for the class.
         *
         * @return
         *     The integer state of the object is returned.
         */
        operator int() const;

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
         * This is the integer state of the object.
         */
        int value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_INTEGER_HPP */
