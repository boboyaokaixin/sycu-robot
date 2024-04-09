
"use strict";

let AnalogRead = require('./AnalogRead.js')
let DigitalSetDirection = require('./DigitalSetDirection.js')
let ServoRead = require('./ServoRead.js')
let ServoWrite = require('./ServoWrite.js')
let AnalogWrite = require('./AnalogWrite.js')
let DigitalWrite = require('./DigitalWrite.js')
let DigitalRead = require('./DigitalRead.js')

module.exports = {
  AnalogRead: AnalogRead,
  DigitalSetDirection: DigitalSetDirection,
  ServoRead: ServoRead,
  ServoWrite: ServoWrite,
  AnalogWrite: AnalogWrite,
  DigitalWrite: DigitalWrite,
  DigitalRead: DigitalRead,
};
