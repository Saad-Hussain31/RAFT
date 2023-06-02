#ifndef SERIALIZATION_SERIALIZED_OBJECT_STRING_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_STRING_HPP

/**
 * @file SerializedString.hpp
 *
 * This module declares the Serialization::SerializedString
 * class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <string>

namespace Serialization {

    /**
     * This class represents a string which is going to be serialized
     * into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class SerializedString
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef std::string Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::String;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedString() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedString(std::string initialValue);

        /**
         * This is the typecast to std::string operator for the class.
         *
         * @return
         *     The string state of the object is returned.
         */
        operator std::string() const;

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
         * This is the string state of the object.
         */
        std::string value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_STRING_HPP */
