#ifndef SERIALIZATION_I_SERIALIZED_OBJECT_HPP
#define SERIALIZATION_I_SERIALIZED_OBJECT_HPP

/**
 * @file ISerializedObject.hpp
 *
 * This module declares the Serialization::ISerializedObject
 * interface.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <string>
#include <SystemAbstractions/IFile.hpp>

namespace Serialization {

    /**
     * This is the interface to something which is going to be serialized
     * into a string of bytes, or has been deserialized from a
     * string of bytes.
     */
    class ISerializedObject {
        // Lifecycle Management
    public:
        virtual ~ISerializedObject() = default;
        ISerializedObject(const ISerializedObject& other) = default;
        ISerializedObject(ISerializedObject&& other) = default;
        ISerializedObject& operator=(const ISerializedObject& other) = default;
        ISerializedObject& operator=(ISerializedObject&& other) = default;

        // Methods
    public:
        /**
         * This is the default constructor.
         */
        ISerializedObject() = default;

        /**
         * This method serializes the object into a string of bytes,
         * which are written to the given file.
         *
         * @param[in] file
         *     This is the file to which, starting at the current
         *     position in the file, the serialized representation of the
         *     state of the object will be written.
         *
         * @param[in] serializationVersion
         *     This is the version of the serialization in which to
         *     encode the object.  If zero (the default), the newest
         *     version is used.
         *
         * @return
         *     An indicator of whether or not the method was
         *     successful is returned.
         */
        virtual bool Serialize(
            SystemAbstractions::IFile* file,
            unsigned int serializationVersion = 0
        ) const = 0;

        /**
         * This method deserializes the object from a string of bytes,
         * which are read from the given file.
         *
         * @param[in] file
         *     This is the file containing, starting at the current
         *     position in the file, the serialized representation of the
         *     state to assign to the object.
         *
         * @return
         *     An indicator of whether or not the method was
         *     successful is returned.
         */
        virtual bool Deserialize(SystemAbstractions::IFile* file) = 0;

        /**
         * This method renders the object into a human-readable
         * string that makes the type and value evident.
         *
         * @return
         *     A human-readable string revealing the type and
         *     value of the object is returned.
         */
        virtual std::string Render() const = 0;

        /**
         * This method parses the given human-readable string to
         * obtain the object's value.
         *
         * @param[in] rendering
         *     This is the string from which to parse the object.
         *
         * @return
         *     An indicator of whether or not the method was
         *     successful is returned.
         */
        virtual bool Parse(std::string rendering) = 0;

        /**
         * This method compares the object to another object provided
         * by base interface to determine whether or not they have
         * equivalent states.
         *
         * @param[in] other
         *     This is the other object with which to compare.
         *
         * @return
         *     An indication of whether or not the two objects are
         *     equivalent is returned.
         */
        virtual bool IsEqualTo(const ISerializedObject* other) const = 0;
    };

}

#endif /* SERIALIZATION_I_SERIALIZED_OBJECT_HPP */
