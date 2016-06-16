'use strict';

var Clay = require('pebble-clay');
var clayConfig = require('./dynamic-config-example');
var customFn = require('./dynamic-fn-example');
var clay = new Clay(clayConfig, customFn);
