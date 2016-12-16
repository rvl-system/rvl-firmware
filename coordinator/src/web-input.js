/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

This file is part of Raver Lights.

Raver Lights is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Raver Lights is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.
*/

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
