/*
  MitsuProtocol.cpp - Mitsubishi Air Conditioner/Heat Pump protocol library
  Copyright (c) 2017 Jarrod Lamb.  All right reserved.
  Originally reverse engineered by Hadley Rich (http://nice.net.nz)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <string.h>
#include <stdlib_noniso.h>
#include "MitsuProtocol.h"

#ifdef DEBUG_ON
void MitsuProtocol::log (const char* msg){
    if (debugCb){
        debugCb (msg);
    }
}
void MitsuProtocol::setDebugCb(DEBUG_CB){
    this->debugCb = debugCb;
}
#endif
/* END DEBUG */

MitsuProtocol::MitsuProtocol() {
}

int MitsuProtocol::getTxSettingsPacket (uint8_t* buffer, settings_t settings){
    #ifdef DEBUG_CALLS
    log("MitsuProtocol::getTxSettingsPacket");
    #endif    
    
    // Zeroize the packet
    for (int i = 0; i < DATA_PACKET_LEN; i++){
        buffer[i] = 0;
    }
    
    buffer[HEADER_1_POS]      = HEADER_1;
    buffer[MSG_TYPE_POS]      = static_cast<uint8_t>(msgKind_t::txSettings);
    buffer[HEADER_3_POS]      = HEADER_3;
    buffer[HEADER_4_POS]      = HEADER_4;
    buffer[LENGTH_POS]        = DATA_PACKET_LEN - CHECKSUM_LEN - HEADER_LEN;
    buffer[DATA_KIND_POS]     = static_cast<uint8_t>(dataKind_t::settingsRequest);

    if (settings.powerValid){
        buffer[DATA_POWER_POS] = static_cast<uint8_t>(settings.power);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::power);
    }
    if (settings.modeValid){
        buffer[DATA_MODE_POS] = static_cast<uint8_t>(settings.mode);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::mode);
    }
    if (settings.tempDegCValid){
        buffer[DATA_TEMP_POS] = tempToByte(settings.tempDegC);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::temp);
    }
    if (settings.fanValid){
        buffer[DATA_FAN_POS] = static_cast<uint8_t>(settings.fan);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::fan);
    }
    if (settings.vaneValid){
        buffer[DATA_VANE_POS] = static_cast<uint8_t>(settings.vane);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::vane);
    }
    if (settings.wideVaneValid){
        buffer[DATA_WIDEVANE_POS] = static_cast<uint8_t>(settings.wideVane);
        buffer[DATA_CONTROL] += static_cast<uint8_t>(control_t::wideVane);
    }

    // Checksum
    buffer[DATA_CHECKSUM_POS] = MitsuProtocol::calculateChecksum(buffer, DATA_PACKET_LEN - 1);

    return DATA_PACKET_LEN;
}

int MitsuProtocol::getTxConnectPacket (uint8_t* buffer){
    #ifdef DEBUG_CALLS
    log("MitsuProtocol::getTxConnectPacket");
    #endif    
    
    // Zeroize the packet
    for (int i = 0; i < CONNECT_PACKET_LEN; i++){
        buffer[i] = 0;
    }    
    
    buffer[HEADER_1_POS]  = HEADER_1;
    buffer[MSG_TYPE_POS]  = static_cast<uint8_t>(msgKind_t::txConnect);
    buffer[HEADER_3_POS]  = HEADER_3;
    buffer[HEADER_4_POS]  = HEADER_4;
    buffer[LENGTH_POS]    = CONNECT_PACKET_LEN - CHECKSUM_LEN - HEADER_LEN;
    buffer[CONNECT_1_POS] = CONNECT_1;
    buffer[CONNECT_2_POS] = CONNECT_2;
    
    // Checksum
    buffer[CONNECT_CHECKSUM_POS] = MitsuProtocol::calculateChecksum(buffer, CONNECT_PACKET_LEN - 1);

    return CONNECT_PACKET_LEN;
}


int MitsuProtocol::getTxInfoPacket (uint8_t* buffer, info_t kind){
    #ifdef DEBUG_CALLS
    log("MitsuProtocol::getTxInfoPacket");
    #endif    
    
    // Zeroize the packet
    for (int i = 0; i < INFO_PACKET_LEN; i++){
        buffer[i] = 0;
    }    
    
    buffer[HEADER_1_POS]  = HEADER_1;
    buffer[MSG_TYPE_POS]  = static_cast<uint8_t>(msgKind_t::txInfoRequest);
    buffer[HEADER_3_POS]  = HEADER_3;
    buffer[HEADER_4_POS]  = HEADER_4;
    buffer[LENGTH_POS]    = INFO_PACKET_LEN - CHECKSUM_LEN - HEADER_LEN;    
    buffer[INFO_KIND]     = static_cast<uint8_t>(kind);
    // Checksum
    buffer[INFO_CHECKSUM_POS] = MitsuProtocol::calculateChecksum(buffer, INFO_PACKET_LEN - 1);

    return INFO_PACKET_LEN;
}


uint8_t MitsuProtocol::calculateChecksum(uint8_t* data, int len) {
    uint8_t sum = 0;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    return (0xfc - sum) & 0xff;
}

const char* MitsuProtocol::power_tToString (power_t power){
    switch (power){
        case power_t::powerOn:return "on";
        case power_t::powerOff:return "off";
    }
    return "undefined";
}
void MitsuProtocol::power_tFromString (const char* powerStr, power_t* power, bool success){
    success=true;
    if (strcmp(powerStr,"on")==0){*power=power_t::powerOn;}
    else if(strcmp(powerStr,"off")==0){*power=power_t::powerOff;}
    else{*power=power_t::powerOff;success=false;}
}

const char* MitsuProtocol::mode_tToString (mode_t mode){
    switch (mode){
        case mode_t::modeHeat:return "heat";
        case mode_t::modeDry:return "dry";
        case mode_t::modeFan:return "fan";
        case mode_t::modeCool:return "cool";
        case mode_t::modeAuto:return "auto";
    }
    return "undefined";
}
void MitsuProtocol::mode_tFromString (const char* modeStr, mode_t* mode, bool success){
    success=true;
    if(strcmp(modeStr,"heat")==0){ *mode=mode_t::modeHeat;}
    else if(strcmp(modeStr,"dry")==0){ *mode=mode_t::modeDry;}
    else if(strcmp(modeStr,"fan")==0){ *mode=mode_t::modeFan;}
    else if(strcmp(modeStr,"cool")==0){ *mode=mode_t::modeCool;}
    else if(strcmp(modeStr,"auto")==0){ *mode=mode_t::modeAuto;}
    else{*mode=mode_t::modeFan;success=false;}
}

const char* MitsuProtocol::fan_tToString (fan_t fan){
    switch (fan){
        case fan_t::fanAuto:return "auto";
        case fan_t::fanQuiet:return "quiet";
        case fan_t::fan1:return "1";
        case fan_t::fan2:return "2";
        case fan_t::fan3:return "3";
        case fan_t::fan4:return "4";
    }
    return "undefined";
}
void MitsuProtocol::fan_tFromString (const char* fanStr, fan_t* fan, bool success){
    success=true;

    if(strcmp(fanStr,"auto")==0){ *fan= fan_t::fanAuto;}
    else if(strcmp(fanStr,"quiet")==0){ *fan= fan_t::fanQuiet;}
    else if(strcmp(fanStr,"1")==0){ *fan= fan_t::fan1;}
    else if(strcmp(fanStr,"2")==0){ *fan= fan_t::fan2;}
    else if(strcmp(fanStr,"3")==0){ *fan=  fan_t::fan3;}
    else if(strcmp(fanStr,"4")==0){*fan= fan_t::fan4;}
    else{*fan=fan_t::fan1;success=false;}

}

const char* MitsuProtocol::vane_tToString (vane_t vane){

    switch (vane){
        case vane_t::vaneAuto:return "auto";
        case vane_t::vane1:return "1";
        case vane_t::vane2:return "2";
        case vane_t::vane3:return "3";
        case vane_t::vane4:return "4";
        case vane_t::vane5:return "5";
        case vane_t::vaneSwing:return "swing";
    }
    return "undefined";
}

void MitsuProtocol::vane_tFromString (const char* vaneStr, vane_t* vane, bool success){
    success=true;
        if(strcmp(vaneStr,"auto")==0){*vane= vane_t::vaneAuto;}
        else if(strcmp(vaneStr,"1")==0){*vane= vane_t::vane1;}
        else if(strcmp(vaneStr,"2")==0){*vane= vane_t::vane2;}
        else if(strcmp(vaneStr,"3")==0){*vane= vane_t::vane3;}
        else if(strcmp(vaneStr,"4")==0){*vane= vane_t::vane4;}
        else if(strcmp(vaneStr,"5")==0){*vane= vane_t::vane5;}
        else if(strcmp(vaneStr,"swing")==0){*vane= vane_t::vaneSwing;}
        else{*vane=vane_t::vane3;success=false;}
}

const char* MitsuProtocol::wideVane_tToString (wideVane_t wideVane){
    switch (wideVane){
        case wideVane_t::wideVaneFullLeft:return "full_left";
        case wideVane_t::wideVaneHalfLeft:return "half_left";
        case wideVane_t::wideVaneCenter:return "center";
        case wideVane_t::wideVaneHalfRight:return "half_right";
        case wideVane_t::wideVaneFullRight:return "full_right";
        case wideVane_t::wideVaneLeftAndRight:return "left_and_right";
        case wideVane_t::wideVaneSwing:return "swing";
    }
    return "undefined";
}
void MitsuProtocol::wideVane_tFromString (const char* wideVaneStr, wideVane_t* wideVane, bool success){
    success=true;
    if(strcmp(wideVaneStr,"full_left")==0){ *wideVane= wideVane_t::wideVaneFullLeft;}
    else if(strcmp(wideVaneStr, "half_left")==0){ *wideVane= wideVane_t::wideVaneHalfLeft;}
    else if(strcmp(wideVaneStr, "center")==0){ *wideVane= wideVane_t::wideVaneCenter;}
    else if(strcmp(wideVaneStr, "half_right")==0){ *wideVane= wideVane_t::wideVaneHalfRight;}
    else if(strcmp(wideVaneStr, "full_right")==0){ *wideVane= wideVane_t::wideVaneFullRight;}
    else if(strcmp(wideVaneStr, "left_and_right")==0){ *wideVane= wideVane_t::wideVaneLeftAndRight;}
    else if(strcmp(wideVaneStr, "swing")==0){ *wideVane= wideVane_t::wideVaneSwing;}
    else{*wideVane=wideVane_t::wideVaneCenter;success=false;}
}

/* packetBuilder */

MitsuProtocol::packetBuilder::packetBuilder(MitsuProtocol* parent) {
    this->parent = parent;
    cursor = 0;
    for (int i = 0; i < MAX_SIZE; i++){
        buffer[i] = 0;
    }
}

int MitsuProtocol::packetBuilder::addByte(uint8_t b){
    #ifdef DEBUG_BYTES
    char dmsg[16];
    strcpy (dmsg,"Rx: 0x");
    char dbuf[8];
    strcat(dmsg, itoa(b,dbuf,16));
    parent->log(dmsg);    
    #endif
      
    // Too many bytes, reset
    if (cursor >= MAX_SIZE){
        #ifdef DEBUG_BYTES
        parent->log("MitsuProtocol::packetBuilder.addByte: cursor reset");        
        #endif
        reset();
    }

    if ((cursor == 0 && b == HEADER_1) || cursor > 0){
        #ifdef DEBUG_BYTES
        if (cursor==0){
            parent->log("MitsuProtocol::packetBuilder.addByte: packet start");
        }
        #endif
        
        buffer[cursor] = b;
        cursor++;
        return 0; // OK, bytes accepted
    }else{
        return 1; // OK, but ignored - waiting for packet start
    }
}

bool MitsuProtocol::packetBuilder::complete(){
    int expectedLength = MitsuProtocol::HEADER_LEN + buffer[MitsuProtocol::LENGTH_POS] + MitsuProtocol::CHECKSUM_LEN;
    return (cursor >= expectedLength - 1);
}

bool MitsuProtocol::packetBuilder::valid(){
    if (!complete()){
        return false;
    }
    bool headerValid = (buffer[0] == MitsuProtocol::HEADER_1 &&
                        buffer[2] == MitsuProtocol::HEADER_3 &&
                        buffer[3] == MitsuProtocol::HEADER_4);
                        
    int checksumPos = MitsuProtocol::HEADER_LEN + buffer[MitsuProtocol::LENGTH_POS];
    int numuint8_tsInMsg = MitsuProtocol::HEADER_LEN + buffer[MitsuProtocol::LENGTH_POS];
    bool checksumValid = (buffer[checksumPos] == calculateChecksum (buffer,numuint8_tsInMsg));

    return (headerValid && checksumValid);
}

MitsuProtocol::msg_t MitsuProtocol::packetBuilder::getData(){
    #ifdef DEBUG_CALLS
    parent->log("packetBuilder.getData()");
    #endif
    
    #ifdef DEBUG_PACKETS
    char dmsg[256];
    strcpy (dmsg,"Rx Pkt: [");
    for(int i = 0; i < 22; i++) {
        strcat(dmsg,"0x");
        char dbuf[8];
        if (buffer[i]<=0x0f){strcat(dmsg,"0");}
        strcat(dmsg, itoa(buffer[i],dbuf,16));
        if (i==4){strcat(dmsg,"]");};
        strcat(dmsg," ");
    }
    parent->log(dmsg);
    #endif

        
    MitsuProtocol::msg_t msg;
    
    msg.msgKindValid = false;
    
    if (!valid()) {
        #ifdef DEBUG_CALLS
        parent->log("packetBuilder.getData: invalid");
        #endif
        return msg;
    } //TBD
    
    switch(static_cast<msgKind_t>(buffer[MSG_TYPE_POS])){
        case msgKind_t::txConnect:
        case msgKind_t::txSettings:
        case msgKind_t::txInfoRequest:
            break; // We don't care about these right now
            
        case msgKind_t::rxCurrentSettings:
            #ifdef DEBUG_CALLS
            parent->log("packetBuilder.getData: rxCurrentSettings");
            #endif            
            msg.kind = MitsuProtocol::msgKind_t::rxCurrentSettings;
            msg.msgKindValid = true;
            msg.data.rxCurrentSettingsData.kind = static_cast<info_t>(buffer[DATA_KIND_POS]);
            
            switch (msg.data.rxCurrentSettingsData.kind){
                case settings:
                    msg.data.rxCurrentSettingsData.data.settings.power = static_cast<power_t>(buffer[DATA_POWER_POS]);
                    msg.data.rxCurrentSettingsData.data.settings.powerValid = true;
                    msg.data.rxCurrentSettingsData.data.settings.mode = static_cast<mode_t>(buffer[DATA_MODE_POS]);
                    msg.data.rxCurrentSettingsData.data.settings.modeValid = true;
                    msg.data.rxCurrentSettingsData.data.settings.tempDegC = byteToTemp(buffer[DATA_TEMP_POS]);    
                    msg.data.rxCurrentSettingsData.data.settings.tempDegCValid = true;
                    msg.data.rxCurrentSettingsData.data.settings.fan = static_cast<fan_t>(buffer[DATA_FAN_POS]);
                    msg.data.rxCurrentSettingsData.data.settings.fanValid = true;
                    msg.data.rxCurrentSettingsData.data.settings.vane = static_cast<vane_t>(buffer[DATA_VANE_POS]);
                    msg.data.rxCurrentSettingsData.data.settings.vaneValid = true;
                    msg.data.rxCurrentSettingsData.data.settings.wideVane = static_cast<wideVane_t>(buffer[DATA_WIDEVANE_POS]);
                    msg.data.rxCurrentSettingsData.data.settings.wideVaneValid = true;
                    break;
                case roomTemp:
                    msg.data.rxCurrentSettingsData.data.roomTemp.roomTemp = byteToRoomTemp(buffer[DATA_ROOM_TEMP_POS]);
                    msg.data.rxCurrentSettingsData.data.roomTemp.tempSens1Raw = byteToTempRaw(buffer[DATA_TEMP_SENS1_RAW]);
                    msg.data.rxCurrentSettingsData.data.roomTemp.tempSens2Raw = byteToTempRaw(buffer[DATA_TEMP_SENS2_RAW]);
                    break;
                default:
                    #ifdef DEBUG_CALLS
                    char dmsg[128];
                    char dbuf[8];
                    strcpy(dmsg,"packetBuilder.getData: unrecognised rx settings kind:");
                    strcat(dmsg,itoa(buffer[MSG_TYPE_POS],dbuf,16));
                    parent->log(dmsg);
                    #endif    
                    break;
            }
            break;
        case msgKind_t::rxStatusOk:
            #ifdef DEBUG_CALLS
            char dmsg[128];
            char dbuf[8];
            strcpy(dmsg,"packetBuilder.getData: rxStatusOk");
            strcat(dmsg,itoa(buffer[MSG_TYPE_POS],dbuf,16));
            parent->log(dmsg);            
            #endif        
            msg.kind = MitsuProtocol::msgKind_t::rxStatusOk;
            msg.msgKindValid = true;
            break;
        case msgKind_t::rxStatusNok:
            #ifdef DEBUG_CALLS
            char dmsg2[128];
            char dbuf2[8];
            strcpy(dmsg2,"packetBuilder.getData: rxStatusNok");
            strcat(dmsg2,itoa(buffer[MSG_TYPE_POS],dbuf2,16));
            parent->log(dmsg2);            
            #endif
                 
            msg.kind = MitsuProtocol::msgKind_t::rxStatusNok;
            msg.msgKindValid = true;
            break;
        default:       
            #ifdef DEBUG_CALLS
            char dmsg3[128];
            char dbuf3[8];
            strcpy(dmsg3,"packetBuilder.getData: unrecognised message type:");
            strcat(dmsg3,itoa(buffer[MSG_TYPE_POS],dbuf3,16));
            parent->log(dmsg3);            
            #endif
            break; // Unrecognised message
    }
    return msg;
}

void MitsuProtocol::packetBuilder::reset(){
    #ifdef DEBUG_CALLS
    parent->log("packetBuilder.getData: reset()");
    #endif
    cursor = 0;
    for (int i; i < MAX_SIZE; i++){
        buffer[i] = 0;
    }
}
