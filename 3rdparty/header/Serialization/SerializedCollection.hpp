#ifndef SERIALIZATION_SERIALIZED_OBJECT_COLLECTION_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_COLLECTION_HPP

/**
 * @file SerializedCollection.hpp
 *
 * This module declares the Serialization::SerializedCollection
 * class.
 *
 * Copyright (c) 2015-2018 by Richard Walters
 */

#include "SerializedObject.hpp"

#include <map>
#include <stddef.h>
#include <string>
#include <SystemAbstractions/DiagnosticsSender.hpp>
#include <SystemAbstractions/IFile.hpp>

namespace Serialization {

    /**
     * This represents a collection of serialized objects keyed by name.
     */
    class SerializedCollection
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef std::map< std::string, SerializedObject > Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::Collection;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedCollection() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedCollection(std::map< std::string, SerializedObject > initialValue);

        /**
         * This is the typecast to map operator for the class.
         *
         * @return
         *     The map state of the object is returned.
         */
        operator std::map< std::string, SerializedObject >() const;

        /**
         * This method returns an indication of whether or not the
         * collection contains an object with the given name.
         *
         * @param[in] name
         *     This is the name of the serialized object to look up.
         *
         * @return
         *     An indication of whether or not the collection has
         *     an object with the given name is returned.
         */
        bool HasObject(const std::string& name) const;

        /**
         * This method returns the serialized object with the given name,
         * wrapped by SerializedObject.  An empty object is returned
         * if no such object exists in the collection.
         *
         * @param[in] name
         *     This is the name of the serialized object to look up.
         *
         * @return
         *     A wrapper referencing the serialized object with
         *     the given name in the collection is returned.
         *     If no serialized object has the given name, the
         *     wrapper will be empty.
         */
        SerializedObject GetObjectWrapper(const std::string& name) const;

        /**
         * This method returns the serialized object with the given name, nullptr if
         * it does not exist.
         *
         * @param[in] name
         *     This is the name of the serialized object to look up.
         *
         * @return
         *     A pointer to the serialized object with the given name is returned.
         *
         * @retval nullptr
         *     This is returned if no serialized object has the given name.
         */
        template< class SerializedObjectType > SerializedObjectType* GetObject(const std::string& name) const {
            SerializedObject wrapper(GetObjectWrapper(name));
            SerializedObjectType* object = wrapper;
            return object;
        }

        /**
         * This method returns the serialized object with the given name, creating
         * it if it does not exist.
         *
         * @param[in] name
         *     This is the name of the serialized object to look up.
         *
         * @param[in] defaultValue
         *     This is the value to set for the serialized object if the serialized object
         *     does not already exist.
         *
         * @return
         *     A pointer to the serialized object with the given name is returned.
         *     If no serialized object has the given name, one is created with
         *     the given default value.
         */
        template< class SerializedObjectType > SerializedObjectType* GetObject(
            const std::string& name,
            typename SerializedObjectType::Type defaultValue
        ) {
            SerializedObject wrapper(GetObjectWrapper(name));
            SerializedObjectType* object = wrapper;
            if (object == nullptr) {
                object = new SerializedObjectType(defaultValue);
                wrapper = SerializedObject(object, true);
                collection_[name] = wrapper;
            }
            return object;
        }

        /**
         * This method returns the value of the serialized object with the given name,
         * or the given default value if no serialized object has the given name.
         *
         * @param[in] name
         *     This is the name of the serialized object to look up.
         *
         * @param[in] defaultValue
         *     This is the value to return if there is no serialized object with
         *     the given name.
         *
         * @return
         *     The value of the serialized object with the given name is returned.
         *     If no serialized object has the given name, the given default value
         *     is returned instead.
         */
        template< class SerializedObjectType > typename SerializedObjectType::Type GetObjectValue(
            const std::string& name,
            typename SerializedObjectType::Type defaultValue
        ) const {
            auto wrapper(collection_.find(name));
            if (wrapper == collection_.end()) {
                return defaultValue;
            }
            const SerializedObjectType* object = wrapper->second;
            if (object == nullptr) {
                return defaultValue;
            }
            return *object;
        }

        /**
         * This method creates a serialized object with the given name and value,
         * replacing any previous serialized object with the same name.
         *
         * @param[in] name
         *     This is the name of the serialized object to create.
         *
         * @param[in] value
         *     This is the value of the serialized object to create.
         */
        void SetObject(const std::string& name, SerializedObject value);

        /**
         * This method creates a serialized object with the given name and value,
         * replacing any previous serialized object with the same name.
         *
         * @param[in] name
         *     This is the name of the serialized object to create.
         *
         * @param[in] value
         *     This is the value of the serialized object to create.
         */
        template< class SerializedObjectType > void SetObjectValue(const std::string& name, typename SerializedObjectType::Type value) {
            collection_[name] = SerializedObject(new SerializedObjectType(value), true);
        }

        /**
         * This method removes the serialized object in the collection
         * that has the given name.
         *
         * @param[in] name
         *     This is the name of the serialized object to remove.
         */
        void RemoveObject(const std::string& name);

        /**
         * This method returns the name and a wrapper for the first
         * serialized object that is in the collection.
         *
         * @param[out]
         *     The name of the first serialized object is stored here.
         *
         * @param[out] value
         *     A wrapper for the first serialized object is stored here.
         *
         * @return
         *     An indication of whether or not an object was returned
         *     is returned.  No object is returned if the collection is
         *     empty.
         */
        bool GetFirstObject(std::string& name, SerializedObject& value) const;

        /**
         * This method returns the name and a wrapper for the serialized
         * object that is next in the collection after the one with
         * the given name.
         *
         * @param[in,out]
         *     On input, this is the name of the serialized object that
         *     comes before the serialized object to be returned.
         *     On output, the name of the next serialized object is
         *     stored here.
         *
         * @param[out] value
         *     A wrapper for the next serialized object is stored here.
         *
         * @return
         *     An indication of whether or not an object was returned
         *     is returned.  No object is returned if the object with
         *     the given name is last in the collection.
         */
        bool GetNextObject(std::string& name, SerializedObject& value) const;

        /**
         * This method returns the number of objects in the collection.
         *
         * @return
         *     The number of objects in the collection is returned.
         */
        size_t GetSize() const;

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
         * This stores the collection of serialized objects, keyed
         * by name.
         */
        std::map< std::string, SerializedObject > collection_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_COLLECTION_HPP */
