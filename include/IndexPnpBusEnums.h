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


#ifndef INDEXPNPBUSENUMS_H_
#define INDEXPNPBUSENUMS_H_

enum class IndexPnpBusResponseCode : uint8_t {
  ok = 0x00,
  wrongUuid = 0x01,
  motorFault = 0x02,
  uninitializedFeeder = 0x03,
  unexpectedPayloadSize = 0x04,
  processing = 0xF0
};

enum class IndexPnpBusFunctionCode : uint8_t {
  getFeederId = 0x01,
  initializeFeeder = 0x02,
  getVersion = 0x03,
  moveFeedForward = 0x04,
  moveFeedBackward = 0x05,
  getFeederAddress = 0xA0,
};

#endif /* INDEXPNPBUSENUMS_H_ */