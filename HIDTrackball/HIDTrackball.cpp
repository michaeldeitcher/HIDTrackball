//
//  HIDTrackball.cpp
//  HIDTrackball
//
//  Created by Michael Deitcher on 1/10/16.
//  Copyright © 2016 StudioSplotch. All rights reserved.
//

#include "HIDTrackball.pch"
#include "hidapi.h"

#define MAX_STR 255
#define VENDOR_ID 0x1241
#define PRODUCT_ID 0x1122
hid_device *handles[8];
int numDevices;
int lastTrackballHandleIndexRetrieved;
bool initialized = false;
mouse_report mouse_report[8];

void InitializeHIDTrackball(){
    if (initialized)
        return;
    initialized = true;
    numDevices = 0;
    lastTrackballHandleIndexRetrieved = -1;

    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(VENDOR_ID, PRODUCT_ID);
    cur_dev = devs;
    while (cur_dev) {
        handles[numDevices] = hid_open_path(cur_dev->path);
        numDevices += 1;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
}

void ShutdownHIDTrackball(){
    initialized = false;
}

void ReleaseTrackball(int trackball){
    hid_close(handles[trackball]);
}

bool IsTrackballConnected(int trackball){
    return trackball > -1 && numDevices > trackball;
}

int GetNextConnectedTrackball(){
    if( lastTrackballHandleIndexRetrieved + 1 < numDevices ){
        lastTrackballHandleIndexRetrieved += 1;
        return lastTrackballHandleIndexRetrieved;
    } else
        return -1;
}

int GetTrackballButtonPressed(int trackball){
    if(!IsTrackballConnected(trackball))
        return -1;
    int ret = hid_read_mouse_update(handles[trackball], &mouse_report[trackball]);
    if (ret == -1)
        return -1;
    return 0;
}

int GetTrackballDeltaX(int trackball){
    return mouse_report[trackball].x;
}

int GetTrackballDeltaY(int trackball){
    return mouse_report[trackball].y;
}
