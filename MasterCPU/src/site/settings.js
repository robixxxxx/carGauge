// function addGauge() {
//     var gaugeContainer = document.getElementById('gauge-container');
//     var newGauge = document.createElement('div');
//     newGauge.className = 'gauge';
//     newGauge.innerHTML = 'New Gauge: <span class="gauge-value">0</span>';
    
//     // Dodanie przycisku usuwania
//     var removeButton = document.createElement('span');
//     removeButton.className = 'remove-button';
//     removeButton.innerHTML = 'Remove';
//     removeButton.onclick = function() {
//         gaugeContainer.removeChild(newGauge);
//         removeSavedGauge(newGauge);
//     };
//     newGauge.appendChild(removeButton);
    
//     gaugeContainer.appendChild(newGauge);

//     // Zapisanie parametrów zegara
//     var gaugeName = document.getElementById('gauge-name').value;
//     var frameNumber = document.getElementById('frame-number').value;
//     var byteNumber = document.getElementById('byte-number').value;
//     var valueSize = document.getElementById('value-size').value;
//     var byte1Number = document.getElementById('byte1-number').value;
//     var byte2Number = document.getElementById('byte2-number').value;
//     var criticalValue = document.getElementById('critical-value').value;
//     var unit = document.getElementById('unit').value;

//     // Wyświetlenie zapisanych parametrów
//     var savedGaugesContainer = document.getElementById('saved-gauges');
//     var gaugeDetails = document.createElement('div');
//     gaugeDetails.className = 'gauge-details';
//     gaugeDetails.innerHTML = `<strong>${gaugeName}</strong>: Frame ${frameNumber}, Byte ${byteNumber}, ${valueSize}-byte value`;
//     if (valueSize === '2') {
//         gaugeDetails.innerHTML += `, Bytes ${byte1Number} and ${byte2Number}`;
//     }
//     gaugeDetails.innerHTML += `, Critical Value: ${criticalValue}, Unit: ${unit}`;
    
//     // Dodanie przycisku usuwania
//     var removeButtonSaved = document.createElement('span');
//     removeButtonSaved.className = 'remove-button';
//     removeButtonSaved.innerHTML = 'Remove';
//     removeButtonSaved.onclick = function() {
//         savedGaugesContainer.removeChild(gaugeDetails);
//         removeGauge(newGauge);
//     };
//     gaugeDetails.appendChild(removeButtonSaved);
    
//     savedGaugesContainer.appendChild(gaugeDetails);
// }

// function removeSavedGauge(gauge) {
//     var savedGaugesContainer = document.getElementById('saved-gauges');
//     var gaugeDetailsList = savedGaugesContainer.getElementsByClassName('gauge-details');

//     for (var i = 0; i < gaugeDetailsList.length; i++) {
//         if (gaugeDetailsList[i].contains(gauge)) {
//             savedGaugesContainer.removeChild(gaugeDetailsList[i]);
//             break;
//         }
//     }
// }

// function removeGauge(gauge) {
//     var gaugeContainer = document.getElementById('gauge-container');
//     gaugeContainer.removeChild(gauge);
// }

// document.getElementById('digital').addEventListener('change', function() {
//     document.getElementById('digital-gauge').style.display = 'block';
//     document.getElementById('analog-gauge').style.display = 'none';
// });

// document.getElementById('analog').addEventListener('change', function() {
//     document.getElementById('digital-gauge').style.display = 'none';
//     document.getElementById('analog-gauge').style.display = 'block';
// });

// function updateGauges() {
//     var gaugeValues = document.getElementsByClassName('gauge-value');
//     for (var i = 0; i < gaugeValues.length; i++) {
//         gaugeValues[i].innerText = Math.floor(Math.random() * (parseInt(document.getElementById('max-value').value) - parseInt(document.getElementById('min-value').value) + 1)) + parseInt(document.getElementById('min-value').value);
//     }
// }

// setInterval(updateGauges, 1000);

// settings.js

document.addEventListener('DOMContentLoaded', function () {
    document.getElementById('digital').addEventListener('change', function () {
        document.getElementById('digital-image').style.display = 'block';
        document.getElementById('analog-image').style.display = 'none';
    });

    document.getElementById('analog').addEventListener('change', function () {
        document.getElementById('digital-image').style.display = 'none';
        document.getElementById('analog-image').style.display = 'block';
    });

    setInterval(updateGauges, 1000);
});

function addGauge() {
    var gaugeContainer = document.getElementById('saved-gauges');

    // Sprawdzenie, czy przekroczono limit wskaźników (5 wskaźników)
    if (gaugeContainer.children.length >= 5) {
        alert('Przekroczono limit wskaźników (maksymalnie 5).');
        return;
    }

    var newGauge = document.createElement('div');
    newGauge.className = 'gauge-details';

    // Pobranie wartości typu wskaźnika
    var gaugeType = document.querySelector('input[name="gaugeType"]:checked').value;

    // Ustawienie obrazka dla typu wskaźnika
    var gaugeImage = (gaugeType === 'digital') ? 'digital_gauge_image.jpg' : 'analog_gauge_image.jpg';
    newGauge.innerHTML = `<img src="${gaugeImage}" alt="${gaugeType} Gauge Image">`;

    // Dodanie informacji o wskaźniku
    newGauge.innerHTML += `<strong>${document.getElementById('gauge-name').value}</strong>: Frame ${document.getElementById('frame-number').value}, Byte ${document.getElementById('byte-number').value}, ${document.getElementById('value-size').value}-byte value`;

    if (document.getElementById('value-size').value === '2') {
        newGauge.innerHTML += `, Bytes ${document.getElementById('byte1-number').value} and ${document.getElementById('byte2-number').value}`;
    }

    newGauge.innerHTML += `, Critical Value: ${document.getElementById('critical-value').value}, Unit: ${document.getElementById('unit').value}`;

    // Dodanie przycisku usuwania
    var removeButtonSaved = document.createElement('span');
    removeButtonSaved.className = 'remove-button';
    removeButtonSaved.innerHTML = 'Remove';
    removeButtonSaved.onclick = function () {
        removeSavedGauge(newGauge);
    };
    newGauge.appendChild(removeButtonSaved);

    gaugeContainer.appendChild(newGauge);
}

function removeSavedGauge(gauge) {
    var savedGaugesContainer = document.getElementById('saved-gauges');
    savedGaugesContainer.removeChild(gauge);
}

function updateGauges() {
    var gaugeValues = document.getElementsByClassName('gauge-value');
    for (var i = 0; i < gaugeValues.length; i++) {
        gaugeValues[i].innerText = Math.floor(Math.random() * (parseInt(document.getElementById('max-value').value) - parseInt(document.getElementById('min-value').value) + 1)) + parseInt(document.getElementById('min-value').value);
    }
}
