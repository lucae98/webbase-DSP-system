
// -------------------------------------
// Globale Variablen
// -------------------------------------
const esp32Host = 'esp32.local';



// -------------------------------------
// Helper Functions
// -------------------------------------

// Checks if a given string is a valid number (accepts dot or comma as decimal separator)

// Funktion zur Validierung (Zahl mit optional Komma oder Punkt)
function isValidNumber(str) {
    return /^-?\d+(?:[.,]\d+)?$/.test(str.trim());
}

function checkGainValue(str) {
    const num = parseFloat(str);

    return num >= -30 && num <= 30;
}


// Replaces comma with dot and trims whitespace

function normalizeNumber(str) {

    return str.trim().replace(',', '.');
}

// Sends a POST request to the given URL path with provided data --> (Keypair)

function sendPostRequest(path, data, outputElementID) {
    fetch(path, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: new URLSearchParams(data)
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Serverantwort war nicht OK: ' + response.statusText);
            }
            return response.text();
        })
        .then(data => {
            console.log(`Antwort vom Server (${path}):`, data);
            document.getElementById(outputElementID).textContent = "Empfangen: " + data;

        })
        .catch(err => {
            console.error(`Fehler (${path}):`, err);
            document.getElementById(outputElementID).textContent = "Fehler: " + err.message;
        });

}

function handleChannelLevel(value, textElementId, signalLEDId) {

    const textElement = document.getElementById(textElementId);
    const LED = document.getElementById(signalLEDId);
    let number;

    if (value === "inf") {
        number = Infinity;
    } else if (value === "-inf") {
        number = -Infinity;
    } else {
        number = parseFloat(value);
    }

    // Textausgabe und LED für den Zahlenwert
    if (Number.isNaN(number)) {
        textElement.textContent = "NaN";
        updateSignalLed(LED, number);
        return;
    }
    else if (!isFinite(number)) {
        textElement.textContent = " -∞ dBFS";
        updateSignalLed(LED, number);
        return;
    }
    else if (isFinite(number)) {
        textElement.textContent = number.toFixed(2) + " dBFS";
        updateSignalLed(LED, number);
        return;
    } else {
        textElement.textContent = "undef";
        updateSignalLed(LED, NaN);

    }
}

// Change LED Color
function updateSignalLed(signalLed, level) {

    if (!Number.isFinite(level) || Number.isNaN(level)) {
        signalLed.classList.remove('green', 'yellow', 'red');
        signalLed.classList.add('black');
        return;
    }

    signalLed.classList.remove('green', 'yellow', 'red', 'black');

    if (level <= -70) {
        //LED bleibt grau
    } else if ((level >= -40) && (level < -10)) {
        signalLed.classList.add('green');
    } else if ((level >= -10) && (level < -4)) {
        signalLed.classList.add('yellow');
    } else if (level >= -4) {
        signalLed.classList.add('red');
    }
}


// -------------------------------------
// Event Handlers
// -------------------------------------

// Send pre-gain value
document.getElementById('gainPreSend').addEventListener('click', function () {
    const value = document.getElementById('gainPreInput').value;
    if (!isValidNumber(value)) {
        document.getElementById('outputGainPre').textContent = 'No valid Number (0,5/-3)';
        return;
    }
    const normalized = normalizeNumber(value);

    if (!checkGainValue(normalized)) {
        document.getElementById('outputGainPre').textContent = 'Only Values between -30dB and 30 dB';
        return;
    }
    //Wertepaar erstellen
    sendPostRequest('/preGain', { preGain: normalized }, 'outputGainPre');
    //Eingabefeld leeren
    document.getElementById('gainPreInput').value = '';
});

// Send post-gain value
document.getElementById('gainPostSend').addEventListener('click', function () {
    const value = document.getElementById('gainPostInput').value;
    if (!isValidNumber(value)) {
        document.getElementById('outputGainPost').textContent = 'No valid Number (0,5/-3)';
        return;
    }
    const normalized = normalizeNumber(value);

    if (!checkGainValue(normalized)) {
        document.getElementById('outputGainPost').textContent = 'Only Values between -30dB and 30 dB';
        return;
    }

    //Wertepaar erstellen
    sendPostRequest('/postGain', { postGain: normalized }, 'outputGainPost');
    //Eingabefeld leeren
    document.getElementById('gainPostInput').value = '';
});



// Send Filter 1 Parameters mit Type

document.getElementById('filter1send').addEventListener('click', function () {

    const valueFreq1 = document.getElementById('freq1').value;
    const valueGain1 = document.getElementById('gain1').value;
    const valueQ1 = document.getElementById('q1').value;
    const valueType1 = document.getElementById('typeFilter1').value;
    //Check eingabevalues
    const arrayValues = [valueFreq1, valueGain1, valueQ1];

    if (arrayValues.some(val => !isValidNumber(val))) {
        document.getElementById('outputFilter1').textContent = 'No valid Number (0,5/-3)';
        return;
    }

    if (arrayValues[0] < 10 || arrayValues[0] > 24000 || arrayValues[1] < -45 || arrayValues[1] > 45 || arrayValues[2] <= 0) {
        document.getElementById('outputFilter1').textContent = 'Frequency range 10 to 24000, Gain range from -45 to 45 and Q greater than 0';
        return;
    }

    const arrayNomalizedValues = arrayValues.map(normalizeNumber);

    sendPostRequest('/filter1', {

        freq1: arrayNomalizedValues[0],
        gain1: arrayNomalizedValues[1],
        q1: arrayNomalizedValues[2],
        type1: valueType1
    }, 'outputFilter1');

    //Felder Leeren
    document.getElementById('freq1').value = '';
    document.getElementById('gain1').value = '';
    document.getElementById('q1').value = '';

    const bypassButton = document.getElementById('bypassFilter1')

    bypassButton.classList.remove('active');
    bypassButton.textContent = "Bypass";

})


// Handle Filter 1 byPass Toggle

document.getElementById('bypassFilter1').addEventListener('click', function () {
    const button = this;

    // Toggle: Ist der Button bereits "active"?
    const isBypassed = button.classList.toggle('active'); // toggle fügt/entfernt "active"-Klasse
    button.textContent = isBypassed ? "Bypassed" : "Bypass";

    // 1 = Bypass aktiv (active-Klasse gesetzt), 0 = Bypass aus
    const bypassValue = isBypassed ? 1 : 0;

    // Debugging
    // console.log("Bypass Filter 1:", bypassValue);

    sendPostRequest('/bypassFilter1', { bypass1: bypassValue }, 'outputFilter1');

});



// Send Filter 2 Parameters mit Type

document.getElementById('filter2send').addEventListener('click', function () {

    const valueFreq2 = document.getElementById('freq2').value;
    const valueGain2 = document.getElementById('gain2').value;
    const valueQ2 = document.getElementById('q2').value;
    const valueType2 = document.getElementById('typeFilter2').value;
    //Check eingabevalues
    const arrayValues = [valueFreq2, valueGain2, valueQ2];

    if (arrayValues.some(val => !isValidNumber(val))) {
        document.getElementById('outputFilter2').textContent = 'No valid Number (0,5/-3)';
        return;
    }

    if (arrayValues[0] < 10 || arrayValues[0] > 24000 || arrayValues[1] < -45 || arrayValues[1] > 45 || arrayValues[2] <= 0) {
        document.getElementById('outputFilter2').textContent = 'Frequency range 10 to 24000, Gain range from -45 to 45 and Q greater than 0';
        return;
    }

    const arrayNomalizedValues = arrayValues.map(normalizeNumber);

    sendPostRequest('/filter2', {

        freq2: arrayNomalizedValues[0],
        gain2: arrayNomalizedValues[1],
        q2: arrayNomalizedValues[2],
        type2: valueType2
    }, 'outputFilter2');

    //Felder Leeren
    document.getElementById('freq2').value = '';
    document.getElementById('gain2').value = '';
    document.getElementById('q2').value = '';

    const bypassButton = document.getElementById('bypassFilter2')

    bypassButton.classList.remove('active');
    bypassButton.textContent = "Bypass";

})


// Handle Filter 2 byPass Toggle

document.getElementById('bypassFilter2').addEventListener('click', function () {
    const button = this;

    // Toggle: Ist der Button bereits "active"?
    const isBypassed = button.classList.toggle('active'); // toggle fügt/entfernt "active"-Klasse
    button.textContent = isBypassed ? "Bypassed" : "Bypass";

    // 1 = Bypass aktiv (active-Klasse gesetzt), 0 = Bypass aus
    const bypassValue = isBypassed ? 1 : 0;

    // Debugging
    // console.log("Bypass Filter 2:", bypassValue);

    sendPostRequest('/bypassFilter2', { bypass2: bypassValue }, 'outputFilter2');

});



// Send Filter 3 Parameters mit Type

document.getElementById('filter3send').addEventListener('click', function () {

    const valueFreq3 = document.getElementById('freq3').value;
    const valueGain3 = document.getElementById('gain3').value;
    const valueQ3 = document.getElementById('q3').value;
    const valueType3 = document.getElementById('typeFilter3').value;
    //Check eingabevalues
    const arrayValues = [valueFreq3, valueGain3, valueQ3];

    if (arrayValues.some(val => !isValidNumber(val))) {
        document.getElementById('outputFilter3').textContent = 'No valid Number (0,5/-3)';
        return;
    }

    if (arrayValues[0] < 10 || arrayValues[0] > 24000 || arrayValues[1] < -45 || arrayValues[1] > 45 || arrayValues[2] <= 0) {
        document.getElementById('outputFilter3').textContent = 'Frequency range 10 to 24000, Gain range from -45 to 45 and Q greater than 0';
        return;
    }


    const arrayNomalizedValues = arrayValues.map(normalizeNumber);

    sendPostRequest('/filter3', {

        freq3: arrayNomalizedValues[0],
        gain3: arrayNomalizedValues[1],
        q3: arrayNomalizedValues[2],
        type3: valueType3
    }, 'outputFilter3');

    //Felder Leeren
    document.getElementById('freq3').value = '';
    document.getElementById('gain3').value = '';
    document.getElementById('q3').value = '';

    const bypassButton = document.getElementById('bypassFilter3')

    bypassButton.classList.remove('active');
    bypassButton.textContent = "Bypass";

})


// Handle Filter 3 byPass Toggle

document.getElementById('bypassFilter3').addEventListener('click', function () {
    const button = this;

    // Toggle: Ist der Button bereits "active"?
    const isBypassed = button.classList.toggle('active'); // toggle fügt/entfernt "active"-Klasse
    button.textContent = isBypassed ? "Bypassed" : "Bypass";

    // 1 = Bypass aktiv (active-Klasse gesetzt), 0 = Bypass aus
    const bypassValue = isBypassed ? 1 : 0;

    // Debugging
    // console.log("Bypass Filter 3:", bypassValue);

    sendPostRequest('/bypassFilter3', { bypass3: bypassValue }, 'outputFilter3');

});



// Send Filter 4 Parameters mit Type

document.getElementById('filter4send').addEventListener('click', function () {

    const valueFreq4 = document.getElementById('freq4').value;
    const valueGain4 = document.getElementById('gain4').value;
    const valueQ4 = document.getElementById('q4').value;
    const valueType4 = document.getElementById('typeFilter4').value;
    //Check eingabevalues
    const arrayValues = [valueFreq4, valueGain4, valueQ4];

    if (arrayValues.some(val => !isValidNumber(val))) {
        document.getElementById('outputFilter4').textContent = 'No valid Number (0,5/-3)';
        return;
    }

    if (arrayValues[0] < 10 || arrayValues[0] > 24000 || arrayValues[1] < -45 || arrayValues[1] > 45 || arrayValues[2] <= 0) {
        document.getElementById('outputFilter4').textContent = 'Frequency range 10 to 24000, Gain range from -45 to 45 and Q greater than 0';
        return;
    }

    const arrayNomalizedValues = arrayValues.map(normalizeNumber);

    sendPostRequest('/filter4', {

        freq4: arrayNomalizedValues[0],
        gain4: arrayNomalizedValues[1],
        q4: arrayNomalizedValues[2],
        type4: valueType4
    }, 'outputFilter4');

    //Felder Leeren
    document.getElementById('freq4').value = '';
    document.getElementById('gain4').value = '';
    document.getElementById('q4').value = '';

    const bypassButton = document.getElementById('bypassFilter4')

    bypassButton.classList.remove('active');
    bypassButton.textContent = "Bypass";

})


// Handle Filter 4 byPass Toggle

document.getElementById('bypassFilter4').addEventListener('click', function () {
    const button = this;

    // Toggle: Ist der Button bereits "active"?
    const isBypassed = button.classList.toggle('active'); // toggle fügt/entfernt "active"-Klasse
    button.textContent = isBypassed ? "Bypassed" : "Bypass";

    // 1 = Bypass aktiv (active-Klasse gesetzt), 0 = Bypass aus
    const bypassValue = isBypassed ? 1 : 0;

    // Debugging
    // console.log("Bypass Filter 1:", bypassValue);

    sendPostRequest('/bypassFilter4', { bypass4: bypassValue }, 'outputFilter4');

});



// Handle Mute Toggle

document.getElementById('muteCheckbox').addEventListener('click', function () {
    const value = document.getElementById('muteCheckbox').checked ? 0 : 1; // 0 for checked, 1 for uncheck oder lieber anderherum ? 1 : 0;
    sendPostRequest('/mute', { mute: value }, 'outputMute');
});






// Handel Signal detection 
// 
// mit Selbstaufrufender funktionm
(() => {
    const signalLedL = document.getElementById('signalLedL');
    const signalLedR = document.getElementById('signalLedR');
    const reconnectInterval = 2000; // in ms

    let socket;

    function connectWebSocket() {
        socket = new WebSocket(`ws://${esp32Host}:81/`);

        socket.onopen = () => {
            console.log('WebSocket connected');
            document.getElementById('outputSignalLeds').textContent = "WebSocket connected";
        };

        socket.onclose = () => {
            console.warn('WebSocket disconnected');
            document.getElementById('outputSignalLeds').textContent = "WebSocket disconnected";

            // Reconnect nach einer Pause
            setTimeout(connectWebSocket, reconnectInterval);
        };

        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
            document.getElementById('outputSignalLeds').textContent = "Fehler: " + error.message;

            // Verbindung explizit schließen → triggert onclose → reconnect
            socket.close();
        };

        socket.onmessage = (event) => {
            const message = event.data;

            if (message === "Connected") {
                document.getElementById('outputSignalLeds').textContent = "ESP: " + message;
                return;
            }

            try {
                const data = JSON.parse(message);
                handleChannelLevel(data.Left, 'channelSignalLedL', 'signalLedL');
                handleChannelLevel(data.Right, 'channelSignalLedR', 'signalLedR');
            } catch (err) {
                document.getElementById('outputSignalLeds').textContent = "Ungültige Nachricht vom ESP: " + message;
                console.error("Fehler beim Parsen:", err);
                console.log("Raw Message: ", event.data);
            }
        };
    }

    // WebSocket-Verbindung starten
    connectWebSocket();
})();


