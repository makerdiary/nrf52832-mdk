/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

DigitalOut  led1(LED1, 1);
InterruptIn button(BLE_BUTTON_PIN_NAME);
uint8_t cnt;

// Change your device name below
const char DEVICE_NAME[] = "GAPButton";

/* We can arbiturarily choose the GAPButton service UUID to be 0xAA00
 * as long as it does not overlap with the UUIDs defined here:
 * https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx */
#define GAPButtonUUID 0xAA00
const uint16_t uuid16_list[] = {GAPButtonUUID};

static EventQueue eventQueue(/* event count */ 16 * EVENTS_EVENT_SIZE);

void print_error(ble_error_t error, const char* msg)
{
    printf("%s: ", msg);
    switch(error) {
        case BLE_ERROR_NONE:
            printf("BLE_ERROR_NONE: No error");
            break;
        case BLE_ERROR_BUFFER_OVERFLOW:
            printf("BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been aborted");
            break;
        case BLE_ERROR_NOT_IMPLEMENTED:
            printf("BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the target HW");
            break;
        case BLE_ERROR_PARAM_OUT_OF_RANGE:
            printf("BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range");
            break;
        case BLE_ERROR_INVALID_PARAM:
            printf("BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid");
            break;
        case BLE_STACK_BUSY:
            printf("BLE_STACK_BUSY: The stack is busy");
            break;
        case BLE_ERROR_INVALID_STATE:
            printf("BLE_ERROR_INVALID_STATE: Invalid state");
            break;
        case BLE_ERROR_NO_MEM:
            printf("BLE_ERROR_NO_MEM: Out of Memory");
            break;
        case BLE_ERROR_OPERATION_NOT_PERMITTED:
            printf("BLE_ERROR_OPERATION_NOT_PERMITTED");
            break;
        case BLE_ERROR_INITIALIZATION_INCOMPLETE:
            printf("BLE_ERROR_INITIALIZATION_INCOMPLETE");
            break;
        case BLE_ERROR_ALREADY_INITIALIZED:
            printf("BLE_ERROR_ALREADY_INITIALIZED");
            break;
        case BLE_ERROR_UNSPECIFIED:
            printf("BLE_ERROR_UNSPECIFIED: Unknown error");
            break;
        case BLE_ERROR_INTERNAL_STACK_FAILURE:
            printf("BLE_ERROR_INTERNAL_STACK_FAILURE: internal stack faillure");
            break;
    }
    printf("\r\n");
}

void updatePayload(void)
{
    // Update the count in the SERVICE_DATA field of the advertising payload
    uint8_t service_data[3];
    service_data[0] = GAPButtonUUID & 0xff;
    service_data[1] = GAPButtonUUID >> 8;
    service_data[2] = cnt; // Put the button click count in the third byte
    ble_error_t err = BLE::Instance().gap().updateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, (uint8_t *)service_data, sizeof(service_data));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Updating payload failed");
    }
}

void buttonPressedCallback(void)
{
    ++cnt;

    // Calling BLE api in interrupt context may cause race conditions
    // Using mbed-events to schedule calls to BLE api for safety
    eventQueue.call(updatePayload);
}

void blinkCallback(void)
{
    led1 = !led1;
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

void bleInitComplete(BLE::InitializationCompleteCallbackContext *context)
{
    BLE&        ble = context->ble;
    ble_error_t err = context->error;

    if (err != BLE_ERROR_NONE) {
        print_error(err, "BLE initialisation failed");
        return;
    }

    // Set up the advertising flags. Note: not all combination of flags are valid
    // BREDR_NOT_SUPPORTED: Device does not support Basic Rate or Enchanced Data Rate, It is Low Energy only.
    // LE_GENERAL_DISCOVERABLE: Peripheral device is discoverable at any moment
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting GAP flags failed");
        return;
    }

    // Put the device name in the advertising payload
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting device name failed");
        return;
    }

    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting service UUID failed");
        return;
    }

    // The Service Data data type consists of a service UUID with the data associated with that service.
    // We will encode the number of button clicks in the Service Data field
    // First two bytes of SERVICE_DATA field should contain the UUID of the service
    uint8_t service_data[3];
    service_data[0] = GAPButtonUUID & 0xff;
    service_data[1] = GAPButtonUUID >> 8;
    service_data[2] = cnt; // Put the button click count in the third byte
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, (uint8_t *)service_data, sizeof(service_data));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting service data failed");
        return;
    }

    // It is not connectable as we are just boardcasting
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);

    // Send out the advertising payload every 1000ms
    ble.gap().setAdvertisingInterval(1000);

    err = ble.gap().startAdvertising();
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Start advertising failed");
        return;
    }

    printMacAddress();
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    cnt = 0;

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble_error_t err = ble.init(bleInitComplete);
    if (err != BLE_ERROR_NONE) {
        print_error(err, "BLE initialisation failed");
        return 0;
    }

    // Blink LED every 500 ms to indicate system aliveness
    eventQueue.call_every(500, blinkCallback);

    // Register function to be called when button is released
    button.rise(buttonPressedCallback);

    eventQueue.dispatch_forever();

    return 0;
}
