/**************************************************************************/
/*!
    @file     PIR.cpp
    @author   Jason Scheunemann <jason.scheunemann@gmail.com>

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Jason Scheunemann
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "PIR.h"

PIR::PIR(byte sensorPin) {
  this->sensorPin = sensorPin;
  this->motionTimeout = LONG_MAX;
}

void PIR::begin() {
  this->count = 0;
  this->lastCount = 0;
  this->lastUpdate = millis();
  this->motionDetected = false;

}

long PIR::read() {

  if (analogRead(this->sensorPin) > 300) {
    count++;
  }

  if (this->lastCount != this->count) {
    if (motionDetected == false) {
      cb_onMotion(*this);
      this->motionDetected = true;
    }

    this->lastUpdate = millis();
  }

  if (this->motionDetected == true) {
    if (millis() - this->lastUpdate > this->motionTimeout) {
      cb_onMotionTimeout(*this);
      this->motionDetected = false;
    }
  }

  this->lastCount = this->count;
}

void PIR::setMotionTimeout(int seconds) {
  this->motionTimeout = seconds * MILLIS_TO_SECONDS;
}

void PIR::motionHandler(motionEventHandler handler) {
  this->cb_onMotion = handler;
}

void PIR::motionTimeoutHandler(motionEventHandler handler) {
  this->cb_onMotionTimeout = handler;
}
