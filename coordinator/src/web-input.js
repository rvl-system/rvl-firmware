'use strict';

const express = require('express');
const state = require('./state');

module.exports = {
  init
};

let app;

function init(board, cb) {
  app = express();

  app.use(express.static(`${__dirname}/web-input-static`));

  app.post('/api/next-control', (req, res) => {
    state.nextControl();
    res.send('ok');
  });

  app.post('/api/control-up', (req, res) => {
    state.controlUp();
    res.send('ok');
  });

  app.post('/api/control-down', (req, res) => {
    state.controlDown();
    res.send('ok');
  });

  app.listen(3000, cb);
}
