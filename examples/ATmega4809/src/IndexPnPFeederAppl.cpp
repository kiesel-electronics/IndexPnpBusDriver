/*
 * IndexPnPFeederAppl.cpp
 *
 * Created: 25.02.2021 01:00:48
 *  Author: Friedrich
 */ 


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