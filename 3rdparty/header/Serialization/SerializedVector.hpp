#ifndef SERIALIZATION_SERIALIZED_VECTOR_HPP
#define SERIALIZATION_SERIALIZED_VECTOR_HPP

/**
 * @file SerializedVector.hpp
 *
 * This module declares the Serialization::SerializedVector
 * class.
 *
 * Copyright (c) 2013-2018 by Richard Walters
 */

#include <Serialization/SerializedObject.hpp>
#include <string>
#include <vector>

namespace Serialization {

    /**
     * This class represents a vector of objects
     * which is going to be serialized into a string of bytes, or has
     * been deserialized from a string of bytes.
     */
    class SerializedVector
        : public ISerializedObject
    {
        // Custom types
    public:
        /**
         * This is the type of the value represented by the object.
         */
        typedef std::vector< SerializedObject > Type;

        // Public properties
    public:
        /**
         * This is the identifier of this type of serialized object.
         */
        static const SerializedObject::Type TypeId = SerializedObject::Type::Vector;

        // Public methods
    public:
        /**
         * This instance constructor initializes the object
         * to its default empty state.
         */
        SerializedVector() = default;

        /**
         * This instance constructor initializes the object
         * to the given value.
         *
         * @param[in] initialValue
         *     This is the initial value to set for the object.
         */
        SerializedVector(std::vector< SerializedObject > initialValue);

        /**
         * This is the typecast to std::vector operator for the class.
         *
         * @return
         *     The integer state of the object is returned.
         */
        operator std::vector< SerializedObject >() const;

        /**
         * This method returns the number of objects in the vector.
         *
         * @return
         *     The number of objects in the vector is returned.
         */
        size_t GetLength() const;

        /**
         * This is the const index operator.
         *
         * @param[in] index
         *     This is the index of the object to look up.
         *
         * @return
         *     A reference to the object at the given index in the
         *     vector is returned.
         */
        const SerializedObject& operator[](size_t index) const;

        /**
         * This is the index operator.
         *
         * @param[in] index
         *     This is the index of the object to look up.
         *
         * @return
         *     A reference to the object at the given index in the
         *     vector is returned.
         */
        SerializedObject& operator[](size_t index);

        /**
         * This method pushes all serialized objects in the vector
         * beginning at the given index forward one position, and
         * places the given serialized object at the hole opened
         * up at the given index.
         *
         * @param[in] value
         *     This is the serialized object to insert.
         *
         * @param[in] index
         *     This is the index at which to insert the object.
         */
        void Insert(SerializedObject value, size_t index);

        /**
         * This method puts the given serialized object into the
         * vector at the very end.
         *
         * @param[in] value
         *     This is the serialized object to put at the end
         *     of the vector.
         */
        void Append(SerializedObject value);

        /**
         * This method removes the serialized object at the given
         * index of the vector, pulling each object after it back
         * one position.
         *
         * @param[in] index
         *     This is the index of the serialized object
         *     to remove from the vector.
         */
        void Delete(size_t index);

        /**
         * This method searches for the given serialized object in
         * the vector, and if not found, adds the object to the vector.
         *
         * @param[in] value
         *     This is the serialized object to add to the vector if
         *     not already in the vector.
         */
        void Add(SerializedObject value);

        /**
         * This method searches for the given serialized object in
         * the vector, and if found, removes the object from the vector.
         *
         * @param[in] value
         *     This is the serialized object to remove from the vector,
         *     if found in the vector.
         */
        void Remove(SerializedObject value);

        /**
         * This method replaces the object at the given index in the
         * vector with a new object.
         *
         * @param[in] index
         *     This is the index of the object in the vector to replace.
         *
         * @param[in] value
         *     This is the value to replace in the given index of
         *     the vector.
         */
        void Replace(size_t index, SerializedObject value);

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
         * This is the vector state of the object.
         */
        std::vector< SerializedObject > value_;
    };

}

#endif /* SERIALIZATION_SERIALIZED_VECTOR_HPP */
