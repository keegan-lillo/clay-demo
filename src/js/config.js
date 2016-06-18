'use strict';
/* eslint-disable quotes */

module.exports = [
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "size": 1,
        "defaultValue": "Clay Demo 😁 ♥ 這兩個字是"
      },
      {
        "type": "color",
        "messageKey": "BACKGROUND_COLOR",
        "defaultValue": "000055",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "TEXT_COLOR",
        "defaultValue": "ffffff",
        "label": "Text Color"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  },
  {
    "type": "heading",
    "id": "main-heading",
    "defaultValue": "Clay Examples",
    "size": 1
  },
  {
    "type": "text",
    "defaultValue": "The options below do not actually affect the watchface. " +
                    "They are examples of the components available in Clay."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "This is a section"
      },
      {
        "type": "input",
        "defaultValue": "",
        "label": "Input Field",
        "description": "This is a description for the input component. " +
                       "You can add <strong>html</strong> in here too.",
        "attributes": {
          "placeholder": "Placeholder set with attributes"
        }
      },
      {
        "type": "slider",
        "defaultValue": 15,
        "label": "Slider",
        description: "This is the description for the slider",
        "min": 10,
        "max": 20,
        "step": 0.25
      },
      {
        "type": "toggle",
        "label": "This is a Toggle",
        "defaultValue": false
      },
      {
        "type": "color",
        "defaultValue": "FF0000",
        "label": "Standard Color Picker",
        "sunlight": false
      },
      {
        "type": "color",
        "defaultValue": "00FF00",
        "label": "Sunny Color Picker",
        "sunlight": true
      },
      {
        type: 'button',
        id: 'testButton',
        primary: false,
        defaultValue: 'Generic Button',
        description: 'This is a generic button. ' +
                     'You can listen for standard events like "click"'
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "More Settings"
      },
      {
        "type": "radiogroup",
        "label": "Radio Group",
        "options": [
          { "label": "Test thing", "value": "one" },
          { "label": "Another thing", "value": "two" },
          { "label": "Final thing with <em>html</em>", "value": "three" }
        ]
      },
      {
        "type": "select",
        "defaultValue": "grape",
        "label": "Select Menu",
        "options": [
          { "label": "", "value": "" },
          { "label": "Berry", "value": "berry" },
          { "label": "This Option is Selected", "value": "grape" },
          { "label": "Banana", "value": "banana" }
        ],
        "attributes": {
          "required": "required"
        }
      }
    ]
  }
];
