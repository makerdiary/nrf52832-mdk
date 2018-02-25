/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"

DigitalOut alivenessLED(LED1, 1);
static DiscoveredCharacteristic ledCharacteristic;
static bool triggerLedCharacteristic;
static const char PEER_NAME[] = "LED";

static EventQueue eventQueue(/* event count */ 16 * EVENTS_EVENT_SIZE);

void periodicCallback(void) {
    alivenessLED = !alivenessLED; /* Do blinky on LED1 while we're waiting for BLE events */
}

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {
    // parse the advertising payload, looking for data type COMPLETE_LOCAL_NAME
    // The advertising payload is a collection of key/value records where
    // byte 0: length of the record excluding this byte
    // byte 1: The key, it is the type of the data
    // byte [2..N] The value. N is equal to byte0 - 1
    for (uint8_t i = 0; i < params->advertisingDataLen; ++i) {

        const uint8_t record_length = params->advertisingData[i];
        if (record_length == 0) {
            continue;
        }
        const uint8_t type = params->advertisingData[i + 1];
        const uint8_t* value = params->advertisingData + i + 2;
        const uint8_t value_length = record_length - 1;

        if(type == GapAdvertisingData::COMPLETE_LOCAL_NAME) {
            if ((value_length == sizeof(PEER_NAME)) && (memcmp(value, PEER_NAME, value_length) == 0)) {
                printf(
                    "adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u\r\n",
                    params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2],
                    params->peerAddr[1], params->peerAddr[0], params->rssi, params->isScanResponse, params->type
                );
                BLE::Instance().gap().connect(params->peerAddr, Gap::ADDR_TYPE_RANDOM_STATIC, NULL, NULL);
                break;
            }
        }
        i += record_length;
    }
}

void serviceDiscoveryCallback(const DiscoveredService *service) {
    if (service->getUUID().shortOrLong() == UUID::UUID_TYPE_SHORT) {
        printf("S UUID-%x attrs[%u %u]\r\n", service->getUUID().getShortUUID(), service->getStartHandle(), service->getEndHandle());
    } else {
        printf("S UUID-");
        const uint8_t *longUUIDBytes = service->getUUID().getBaseUUID();
        for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
            printf("%02x", longUUIDBytes[i]);
        }
        printf(" attrs[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
    }
}

void updateLedCharacteristic(void) {
    if (!BLE::Instance().gattClient().isServiceDiscoveryActive()) {
        ledCharacteristic.read();
    }
}

void characteristicDiscoveryCallback(const DiscoveredCharacteristic *characteristicP) {
    printf("  C UUID-%x valueAttr[%u] props[%x]\r\n", characteristicP->getUUID().getShortUUID(), characteristicP->getValueHandle(), (uint8_t)characteristicP->getProperties().broadcast());
    if (characteristicP->getUUID().getShortUUID() == 0xa001) { /* !ALERT! Alter this filter to suit your device. */
        ledCharacteristic        = *characteristicP;
        triggerLedCharacteristic = true;
    }
}

void discoveryTerminationCallback(Gap::Handle_t connectionHandle) {
    printf("terminated SD for handle %u\r\n", connectionHandle);
    if (triggerLedCharacteristic) {
        triggerLedCharacteristic = false;
        eventQueue.call(updateLedCharacteristic);
    }
}

void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
    if (params->role == Gap::CENTRAL) {
        BLE &ble = BLE::Instance();
        ble.gattClient().onServiceDiscoveryTermination(discoveryTerminationCallback);
        ble.gattClient().launchServiceDiscovery(params->handle, serviceDiscoveryCallback, characteristicDiscoveryCallback, 0xa000, 0xa001);
    }
}

void triggerToggledWrite(const GattReadCallbackParams *response) {
    if (response->handle == ledCharacteristic.getValueHandle()) {
        printf("triggerToggledWrite: handle %u, offset %u, len %u\r\n", response->handle, response->offset, response->len);
        for (unsigned index = 0; index < response->len; index++) {
            printf("%c[%02x]", response->data[index], response->data[index]);
        }
        printf("\r\n");

        uint8_t toggledValue = response->data[0] ^ 0x1;
        ledCharacteristic.write(1, &toggledValue);
    }
}

void triggerRead(const GattWriteCallbackParams *response) {
    if (response->handle == ledCharacteristic.getValueHandle()) {
        ledCharacteristic.read();
    }
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *) {
    printf("disconnected\r\n");
    /* Start scanning and try to connect again */
    BLE::Instance().gap().startScan(advertisementCallback);
}

void onBleInitError(BLE &ble, ble_error_t error)
{
   /* Initialization error handling should go here */
}

void printMacAddress()
{
    /* Print out device MAC address to the console*/
    Gap::AddressType_t addr_type;
    Gap::Address_t address;
    BLE::Instance().gap().getAddress(&addr_type, address);
    printf("DEVICE MAC ADDRESS: ");
    for (int i = 5; i >= 1; i--){
        printf("%02x:", address[i]);
    }
    printf("%02x\r\n", address[0]);
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);
    ble.gap().onConnection(connectionCallback);

    ble.gattClient().onDataRead(triggerToggledWrite);
    ble.gattClient().onDataWrite(triggerRead);

    // scan interval: 400ms and scan window: 400ms.
    // Every 400ms the device will scan for 400ms
    // This means that the device will scan continuously.
    ble.gap().setScanParams(400, 400);
    ble.gap().startScan(advertisementCallback);

    printMacAddress();
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    triggerLedCharacteristic = false;
    eventQueue.call_every(500, periodicCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
