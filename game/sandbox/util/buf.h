/**
 ByteBuffer
 ByteBuffer.h
 Copyright 2011 - 2013 Ramsey Kant

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Modfied 2015 by Ashley Davis (SgtCoDFish)
 */

#pragma once

// Default number of uint8_ts to allocate in the backing buffer if no size is provided
#define BB_DEFAULT_SIZE 4096

// If defined, utility functions within the class are enabled
#define BB_UTILITY

// If defined, places the class into a namespace called bb
//#define BB_USE_NS

#include <cstdint>
#include <cstring>

#include <vector>

#include <glm/glm.hpp>

#ifdef BB_UTILITY
#include <iostream>
#include <cstdio>
#endif

#ifdef BB_USE_NS
namespace bb {
#endif

    class ByteBuffer {
    public:
        ByteBuffer(size_t size = BB_DEFAULT_SIZE);
        ByteBuffer(uint8_t* arr, size_t size);
        ~ByteBuffer() = default;

        size_t bytesRemaining(); // Number of uint8_ts from the current read position till the end of the buffer
        void clear(); // Clear our the vector and reset read and write positions
        std::unique_ptr<ByteBuffer> clone(); // Return a new instance of a ByteBuffer with the exact same contents and the same state (rpos, wpos)
        //ByteBuffer compact(); // TODO?
        bool equals(ByteBuffer* other); // Compare if the contents are equivalent
        void resize(size_t newSize);
        size_t size(); // Size of internal vector

        // Basic Searching (Linear)
        template<typename T> int32_t find(T key, size_t start = 0) {
            int32_t ret = -1;
            size_t len = buf.size();
            for (size_t i = start; i < len; i++) {
                T data = read<T>(i);
                // Wasn't actually found, bounds of buffer were exceeded
                if ((key != 0) && (data == 0))
                    break;

                // Key was found in array
                if (data == key) {
                    ret = (int32_t) i;
                    break;
                }
            }
            return ret;
        }

        // Replacement
        void replace(uint8_t key, uint8_t rep, size_t start = 0, bool firstOccuranceOnly = false);

        // Read

        uint8_t peek() const; // Relative peek. Reads and returns the next uint8_t in the buffer from the current position but does not increment the read position
        uint8_t getByte() const; // Relative get method. Reads the uint8_t at the buffers current position then increments the position
        uint8_t getByteAt(size_t index) const; // Absolute get method. Read uint8_t at index
        void getBytes(uint8_t* buf, size_t len) const; // Absolute read into array buf of length len
        char getChar() const; // Relative
        char getChar(size_t index) const; // Absolute
        double getDouble() const;
        double getDouble(size_t index) const;
        float getFloat() const;
        float getFloat(size_t index) const;
        uint32_t getUInt() const;
        uint32_t getUInt(size_t index) const;
        int32_t getSInt() const;
        int32_t getSInt(size_t index) const;
        uint64_t getLong() const;
        uint64_t getLong(size_t index) const;
        uint16_t getShort() const;
        uint16_t getShort(size_t index) const;

        // Write

//        template<typename T>
//        void put(const T& value);

        void putBuf(ByteBuffer* src); // Relative write of the entire contents of another ByteBuffer (src)
        void putByte(uint8_t b); // Relative write
        void putByteAt(uint8_t b, size_t index); // Absolute write at index
        void putBytes(uint8_t* b, size_t len); // Relative write
        void putBytes(uint8_t* b, size_t len, size_t index); // Absolute write starting at index
        void putChar(char value); // Relative
        void putChar(char value, size_t index); // Absolute
        void putDouble(double value);
        void putDouble(double value, size_t index);
        void putFloat(float value);
        void putFloat(float value, size_t index);
        void putUInt(uint32_t value);
        void putUInt(uint32_t value, size_t index);
        void putSInt(int32_t value);
        void putSInt(int32_t value, size_t index);
        void putLong(uint64_t value);
        void putLong(uint64_t value, size_t index);
        void putShort(uint16_t value);
        void putShort(uint16_t value, size_t index);

        void putVec3(const glm::vec3 &value);
        void putIVec3(const glm::ivec3 &value);
        void putU8Vec3(const glm::u8vec3 &value);

        // Buffer Position Accessors & Mutators

        void setReadPos(size_t r) {
            rpos = r;
        }

        size_t getReadPos() const {
            return rpos;
        }

        void setWritePos(size_t w) {
            wpos = w;
        }

        size_t getWritePos() const {
            return wpos;
        }

        const std::vector<uint8_t>& data() const {
            return buf;
        }

        // Utility Functions
#ifdef BB_UTILITY
        void setName(std::string n);
        std::string getName();
        void printInfo();
        void printAH();
        void printAscii();
        void printHex();
        void printPosition();
#endif

    private:
        size_t wpos;
        mutable size_t rpos;
        std::vector<uint8_t> buf;

#ifdef BB_UTILITY
        std::string name;
#endif

        template<typename T> T read() const {
            T data = read<T>(rpos);
            rpos += sizeof(T);
            return data;
        }

        template<typename T> T read(size_t index) const {
            if (index + sizeof(T) <= buf.size())
                return *((T*) &buf[index]);
            return 0;
        }

        template<typename T> void append(T data) {
            size_t s = sizeof(data);

            if (size() < (wpos + s))
                buf.resize(wpos + s);
            memcpy(&buf[wpos], (uint8_t*) &data, s);
            //printf("writing %c to %i\n", (uint8_t)data, wpos);

            wpos += s;
        }

        template<typename T> void insert(T data, size_t index) {
            if ((index + sizeof(data)) > size()) {
                buf.resize(size() + (index + sizeof(data)));
            }

            memcpy(&buf[index], (uint8_t*) &data, sizeof(data));
            wpos = index + sizeof(data);
        }
    };

#ifdef BB_USE_NS
}
#endif