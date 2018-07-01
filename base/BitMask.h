#pragma once

#include "../common/types.h"

namespace Core {

    typedef UInt32 IntMask;
    typedef UInt64 LongMask;

    template <class T> 
    class BitMaskUtil {
    public:

        static T invertBitsForIndexMask(UInt16 index) {
            T maskValue = indexToMaskValue(index);
            return invertBits(maskValue);
        }

        static T invertBits(T a) {
            return ~a;
        }

        static T maskValueToIndex(T maskValue) {
            if (maskValue == 0) return 0;

            T index = (T)0x00000001;
            Int32 count = 0;
            while (!(maskValue & index) && count < sizeof(T) * 8) {
                index <<= 1;
                count++;
            }

            return index;
        }

        static T indexToMaskValue(UInt16 index) {
            return (T)0x00000001 << index;
        }

        static void setBit(T * target, UInt16 index) {
            T maskValue = indexToMaskValue(index);
            T uPtr = (T)*target;
            uPtr |= (T)maskValue;
            *target = uPtr;
        }

        static void clearBit(T * target, UInt16 index) {
            T uPtr = (T)*target;
            T mask = invertBitsForIndexMask(index);

            uPtr &= mask;
            *target = uPtr;
        }

        static void setBitForMask(T * target, T mask) {
            T uPtr = (T)*target;
            uPtr |= mask;
            *target = uPtr;
        }

        static void clearBitForMask(T * target, T mask) {
            T uPtr = (T)*target;
            T iMask = invertBits(mask);

            uPtr &= iMask;
            *target = uPtr;
        }

        static Bool isBitSet(T target, UInt16 index) {
            T mask = indexToMaskValue(index);
            return (target & mask) != 0;
        }

        static Bool isBitSetForMask(T target, T mask) {
            return (target & mask) != 0;
        }

        static T createT() {
            return 0;
        }

        static T mergeMasks(T a, T b) {
            return a | b;
        }

        static Bool haveAtLeastOneInCommon(T a, T b) {
            return (a & b) != 0;
        }

        static void clearMask(T * mask) {
            *mask = 0;
        }

        static void setAll(T * mask) {
            *mask = (T)~0;
        }

        static T createMask() {
            return (T)0;
        }
    };

    typedef BitMaskUtil<IntMask> IntMaskUtil;
    typedef BitMaskUtil<LongMask> LongMaskUtil;
}
