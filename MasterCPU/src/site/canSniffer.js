// can_sniffer.js

var displayMode = "hex";

function changeDisplayMode() {
    displayMode = document.getElementById("display-mode").value;
    filterData();
}

function updateTable(id, rtr, ide, data) {
    var filterValue = document.getElementById("filter-input").value.toLowerCase();

    // Check if the row matches the filter before updating or adding
    if (id.toString().toLowerCase().includes(filterValue)) {
        var table = document.getElementById("data-table");
        var tbody = table.getElementsByTagName("tbody")[0];
        var rows = tbody.getElementsByTagName("tr");

        for (var i = 0; i < rows.length; i++) {
            var cells = rows[i].getElementsByTagName("td");
            if (cells[0].innerHTML === id.toString()) {
                cells[1].innerHTML = rtr;
                cells[2].innerHTML = ide;
                for (var j = 0; j < data.length; j++) {
                    var oldValue = cells[j + 3].innerHTML;
                    cells[j + 3].innerHTML = formatData([data[j]]);
                    if (oldValue !== cells[j + 3].innerHTML) {
                        cells[j + 3].classList.add("changed");
                        setTimeout(function(cell) {
                            cell.classList.remove("changed");
                        }, 1000, cells[j + 3]);
                    }
                }
                return;
            }
        }

        var newRow = tbody.insertRow();
        var cell1 = newRow.insertCell(0);
        var cell2 = newRow.insertCell(1);
        var cell3 = newRow.insertCell(2);
        cell1.innerHTML = id;
        cell2.innerHTML = rtr;
        cell3.innerHTML = ide;
        for (var j = 0; j < data.length; j++) {
            var cell = newRow.insertCell(j + 3);
            cell.innerHTML = formatData([data[j]]);
        }
    }
}

function byteArrayToString(bytes) {
    var hexString = Array.from(bytes, function(byte) {
        return ('0' + (byte & 0xFF).toString(16)).slice(-2);
    }).join(' ');

    var decString = bytes.join(' ');

    var binString = Array.from(bytes, function(byte) {
        return ('00000000' + byte.toString(2)).slice(-8);
    }).join(' ');

    return { hex: hexString, dec: decString, bin: binString };
}

function formatData(data) {
    var dataString = byteArrayToString(data);

    switch (displayMode) {
        case "hex":
            return dataString.hex;
        case "dec":
            return dataString.dec;
        case "bin":
            return dataString.bin;
        default:
            return dataString.hex;
    }
}

function filterData() {
    var filterValue = document.getElementById("filter-input").value.toLowerCase();
    var table = document.getElementById("data-table");
    var tbody = table.getElementsByTagName("tbody")[0];
    var rows = tbody.getElementsByTagName("tr");

    for (var i = 0; i < rows.length; i++) {
        var cells = rows[i].getElementsByTagName("td");
        var id = cells[0].innerHTML.toLowerCase();
        
        // Hide the row if it doesn't match the filter
        rows[i].style.display = id.includes(filterValue) ? "" : "none";
    }
}

function simulateI2CData() {
    var id = Math.floor(Math.random() * (120 - 100 + 1)) + 100; // Random ID in the range 100-120
    var rtr = 0;
    var ide = 1;
    var data = Array.from({ length: 8 }, () => Math.floor(Math.random() * 256)); // 8 random bytes

    updateTable(id, rtr, ide, data);
}

setInterval(simulateI2CData, 10); // Update every 10 milliseconds
