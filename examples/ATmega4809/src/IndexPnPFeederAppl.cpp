 /******************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2021 Friedrich Kiesel
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *******************************************************************************/


 #include "IndexPnPFeederAppl.h"


   IndexPnpBusResponseCode IndexPnPFeederAppl::getFeederId(uint8_t (&uuid_out)[12]) {
    uuid_out[ 0] = 0x55;
    uuid_out[ 1] = 0x55;
    uuid_out[ 2] = 0x55;
    uuid_out[ 3] = 0x55;
    uuid_out[ 4] = 0x55;
    uuid_out[ 5] = 0x55;
    uuid_out[ 6] = 0x55;
    uuid_out[ 7] = 0x55;
    uuid_out[ 8] = 0x55;
    uuid_out[ 9] = 0x55;
    uuid_out[10] = 0x55;
    uuid_out[11] = 0x55;
    return IndexPnpBusResponseCode::ok;
   }

   IndexPnpBusResponseCode IndexPnPFeederAppl::initializeFeeder(uint8_t (&uuid_in)[12]) {
     return IndexPnpBusResponseCode::ok;
   }

   IndexPnpBusResponseCode IndexPnPFeederAppl::getFeederVersion(uint8_t (&version_in)[4]) {
    version_in[0] = 0x01;
    version_in[1] = 0x00;
    version_in[2] = 0x00;
    version_in[3] = 0x00;
    return IndexPnpBusResponseCode::ok;
   }

   IndexPnpBusResponseCode IndexPnPFeederAppl::moveFeederForward(uint8_t distance) {
     return IndexPnpBusResponseCode::ok;
   }

   IndexPnpBusResponseCode IndexPnPFeederAppl::moveFeederBackward(uint8_t distance) {
     return IndexPnpBusResponseCode::ok;
   }

   IndexPnpBusResponseCode IndexPnPFeederAppl::getFeederAddress(uint8_t (&uuid_in)[12]) {
     return IndexPnpBusResponseCode::wrongUuid;
   }