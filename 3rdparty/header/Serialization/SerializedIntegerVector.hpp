#ifndef SERIALIZATION_SERIALIZED_INTEGER_VECTOR_HPP
#define SERIALIZATION_SERIALIZED_INTEGER_VECTOR_HPP

/**
 * @file SerializedIntegerVector.hpp
 *
 * This module declares the Serialization::SerializedIntegerVector
 * class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <string>
#include <vector>

namespace Serialization {

    /**
     * This class represents a vector of integers which is going to be
     * serialized into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class SerializedIntegerVector
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef std::vector< int > Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::IntegerVector;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedIntegerVector() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedIntegerVector(std::vector< int > initialValue);

        /**
         * This is the typecast to const int vector reference operator
         * for the class, providing access to the wrapped value.
         *
         * @return
         *     A reference to the wrapped value is returned.
         */
        operator const std::vector< int >&() const;

        /**
         * This is the typecast to int vector reference operator
         * for the class, providing access to the wrapped value.
         *
         * @return
         *     A reference to the wrapped value is returned.
         */
        operator std::vector< int >&();

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
         * This is the vector to be serialized/deserialized.
         */
        std::vector< int > value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_INTEGER_VECTOR_HPP */
