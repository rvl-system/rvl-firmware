const FADE = 0;
const PULSE = 1;

module.exports = {
  messageTypes: {
    SET_PRESET: 0,
    SET_VALUE: 1,
    SET_BRIGHTNESS: 2
  },
  preset: {
    FADE,
    PULSE
  },
  control: {
    NONE: 0,
    BRIGHTNESS: 1,
    PRESET: 2,
    VALUE1: 3,
    VALUE2: 4,
    VALUE3: 5
  },
  value: {
    [FADE]: {
      RATE: 0
    },
    [PULSE]: {
      RATE: 0,
      HUE: 1,
      SATURATION: 2
    }
  },
  defaultValue: {
    [FADE]: {
      RATE: 20
    },
    [PULSE]: {
      RATE: 20,
      HUE: 0,
      SATURATION: 100
    }
  },
  getCodeName(type, code) {
    const keys = Object.keys(module.exports[type]);
    for (const key of keys) {
      if (module.exports[type][key] === code) {
        return key;
      }
    }
  }
};
