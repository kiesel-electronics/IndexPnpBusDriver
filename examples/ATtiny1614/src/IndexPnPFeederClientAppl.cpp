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


 #include "IndexPnPFeederClientAppl.h"


IndexPnPFeederClientAppl::IndexPnPFeederClientAppl() {
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::initializeFeeder(uint8_t* uuid_in) {
  // init the feeder
  return IndexPnpBusResponseCode::ok;
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::moveFeederForward(uint8_t distance) {
  return IndexPnpBusResponseCode::ok;
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::moveFeederBackward(uint8_t distance) {
  return IndexPnpBusResponseCode::ok;
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::setUuid(uint8_t* uuid_in) {
  return IndexPnpBusResponseCode::ok;
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::setParam(uint8_t paramAddr, uint8_t value) {
  return IndexPnpBusResponseCode::ok;
}

IndexPnpBusResponseCode IndexPnPFeederClientAppl::getParam(uint8_t paramAddr, uint8_t *txData) {
  return IndexPnpBusResponseCode::ok;
}

// led triggers
void IndexPnPFeederClientAppl::txLedTrigger() {
}

void IndexPnPFeederClientAppl::rxLedTrigger() {
}