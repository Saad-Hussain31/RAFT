#ifndef SERIALIZATION_SERIALIZED_OBJECT_BOOLEAN_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_BOOLEAN_HPP

/**
 * @file SerializedBoolean.hpp
 *
 * This module declares the Serialization::SerializedBoolean
 * class.
 *
 * Copyright (c) 2014-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <string>

namespace Serialization {

    /**
     * This class represents a boolean value which is going to be
     * serialized into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class SerializedBoolean
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef bool Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::Boolean;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedBoolean() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedBoolean(bool initialValue);

        /**
         * This is the typecast to bool operator for the class.
         *
         * @return
         *     The bool state of the object is returned.
         */
        operator bool() const;

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
         * This is the boolean value state of the object.
         */
        bool value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_BOOLEAN_HPP */
