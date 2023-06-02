#ifndef SERIALIZATION_SERIALIZED_OBJECT_HPP
#define SERIALIZATION_SERIALIZED_OBJECT_HPP

/**
 * @file SerializedObject.hpp
 *
 * This module declares the Serialization::SerializedObject
 * class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <map>
#include <memory>
#include <Serialization/ISerializedObject.hpp>
#include <stdint.h>
#include <string>

namespace Serialization {

    /**
     * This class represents a wrapper for a concrete serialized object.
     * It holds (and may own) a pointer to the concrete serialized object
     * and is aware of the concrete serialized object type.
     *
     * All copies of the same SerializedObject share the same concrete
     * serialized object, and destroying the last SerializedObject object
     * destroys the concrete serialized object (if it is owned).
     * Reassigning a SerializedObject is the same as destroying it and
     * then recreating it as a copy of another SerializedObject.
     */
    class SerializedObject
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This identifies the concrete type of a serialized object.
         */
        enum class Type {
            Empty,
            Boolean,
            Integer,
            UnsignedInteger,
            Decimal,
            String,
            Vector,
            Collection,
            IpAddress,
            IntegerVector,
            UnsignedIntegerVector,
        };

        // Lifecycle management
    public:
        ~SerializedObject();
        SerializedObject(const SerializedObject& other);
        SerializedObject(SerializedObject&& other);
        SerializedObject& operator=(const SerializedObject& other);
        SerializedObject& operator=(SerializedObject&& other);

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state where there is no concrete
         * serialized object.
         */
        SerializedObject();

        /**
         * This instance constructor initializes the object
         * with a concrete serialized object implementation of a
         * specific type.
         *
         * @param[in] impl
         *     This is the implementation of the serialized object
         *     to hold onto.
         */
        template< class SerializedObjectType > SerializedObject(
            const std::shared_ptr< SerializedObjectType > impl
        )
            : type_(SerializedObjectType::TypeId)
            , impl_(impl)
        {
            if (impl_ == nullptr) {
                type_ = Type::Empty;
            }
        }

        /**
         * This instance constructor initializes the object
         * with a concrete serialized object implementation of a
         * specific type.
         *
         * @param[in] impl
         *     This is the implementation of the serialized object
         *     to hold onto.
         *
         * @param[in] ownIt
         *     This flag indicates whether or not the SerializedObject
         *     should own the serialized object implementation, and
         *     destroy it when the last SerializedObject referencing
         *     it is destroyed.
         */
        template< class SerializedObjectType > SerializedObject(
            SerializedObjectType* impl,
            bool ownIt = false
        )
            : type_(SerializedObjectType::TypeId)
        {
            if (ownIt) {
                impl_ = std::shared_ptr< ISerializedObject >(impl);
            } else {
                impl_ = std::shared_ptr< ISerializedObject >(impl, [](ISerializedObject*){});
            }
            if (impl_ == nullptr) {
                type_ = Type::Empty;
            }
        }

        /**
         * This is typecast to constant object type implementation
         * pointer operator.
         *
         * @return
         *     If the concrete serialized object is of the same type
         *     as the template argument, a pointer to the implementation
         *     is returned.  Otherwise, nullptr is returned.
         */
        template< class SerializedObjectType > operator const SerializedObjectType*() const {
            if (
                (impl_ == nullptr)
                || (type_ != SerializedObjectType::TypeId)
            ) {
                return nullptr;
            }
            return static_cast< SerializedObjectType* >(impl_.get());
        }

        /**
         * This is typecast to object type implementation
         * pointer operator.
         *
         * @return
         *     If the concrete serialized object is of the same type
         *     as the template argument, a pointer to the implementation
         *     is returned.  Otherwise, nullptr is returned.
         */
        template< class SerializedObjectType > operator SerializedObjectType*() {
            if (
                (impl_ == nullptr)
                || (type_ != SerializedObjectType::TypeId)
            ) {
                return nullptr;
            }
            return static_cast< SerializedObjectType* >(impl_.get());
        }

        /**
         * This is typecast to object type implementation
         * shared pointer operator.
         *
         * @return
         *     If the concrete serialized object is of the same type
         *     as the template argument, a shared pointer to the
         *     implementation is returned.  Otherwise, nullptr
         *     is returned.
         */
        template< class SerializedObjectType > operator std::shared_ptr< SerializedObjectType >() const {
            if (
                (impl_ == nullptr)
                || (type_ != SerializedObjectType::TypeId)
            ) {
                return nullptr;
            }
            return std::static_pointer_cast< SerializedObjectType >(impl_);
        }

        /**
         * This is the positive comparison operator.
         *
         * @param[in] other
         *     This is the other object to which to compare this object.
         */
        bool operator==(const SerializedObject& other) const;

        /**
         * This is the negative comparison operator.
         *
         * @param[in] other
         *     This is the other object to which to compare this object.
         */
        bool operator!=(const SerializedObject& other) const;

        /**
         * This method returns the type of the serialized object.
         *
         * @return
         *     The type of the serialized object is returned.
         */
        Type GetType() const;

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
         * This identifies the actual type of the serialized object.
         */
        Type type_ = Type::Empty;

        /**
         * This is the serialized object implementation.
         */
        std::shared_ptr< ISerializedObject > impl_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_OBJECT_HPP */
