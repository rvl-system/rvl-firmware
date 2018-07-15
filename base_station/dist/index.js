"use strict";
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
Object.defineProperty(exports, "__esModule", { value: true });
const os_1 = require("os");
const dgram_1 = require("dgram");
const codes_1 = require("./codes");
const clockSyncSignature = ['C'.charCodeAt(0), 'L'.charCodeAt(0), 'K'.charCodeAt(0), 'S'.charCodeAt(0)];
const addresses = os_1.networkInterfaces().wlan0.map((entry) => entry.address);
function run() {
    const startTime = Date.now();
    const socket = dgram_1.createSocket('udp4');
    socket.on('error', (err) => {
        console.error(`server error:\n${err.stack}`);
        socket.close();
    });
    socket.on('message', (msg, rinfo) => {
        if (addresses.indexOf(rinfo.address) === -1) {
            console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
        }
    });
    socket.on('listening', () => {
        console.log(`Server listening`);
        socket.setBroadcast(true);
        let seq = 0;
        setInterval(() => {
            const clockTime = Date.now() - startTime;
            console.log(`Pinging clock time ${clockTime}`);
            const msg = new Uint8Array(14);
            const view = new DataView(msg.buffer);
            // Signature: 4 bytes = "CLKS"
            for (let i = 0; i < 4; i++) {
                view.setUint8(i, clockSyncSignature[i]);
            }
            view.setUint8(4, 1); // Version: 1 byte = 1
            view.setUint8(5, 1); // Type: 1 byte = 1:reference, 2:response
            view.setUint16(6, ++seq); // Sequence: 2 bytes = always incrementing
            view.setUint32(8, clockTime); // Clock: 4 bytes = running clock, relative to app start
            view.setUint16(12, 0); // ClientID: 2 bytes = 0 in this case because this is not an LED device/client
            socket.send(msg, codes_1.SERVER_PORT, codes_1.GATEWAY);
        }, codes_1.CLOCK_SYNC_INTERVAL);
    });
    socket.bind(codes_1.SERVER_PORT, codes_1.SERVER_IP);
}
exports.run = run;
//# sourceMappingURL=index.js.map