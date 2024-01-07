// script.js

function loadPage(page) {
    var mainContent = document.getElementById("main-content");

    switch (page) {
        case 'home':
            mainContent.innerHTML = '<h2>Home Page</h2><p>This is the home page content.</p>';
            break;
        case 'canSniffer':
            mainContent.innerHTML = '<iframe src="canSniffer.html" width="100%" height="100%" frameborder="0"></iframe>';
            break;
        case 'settings':
            mainContent.innerHTML = '<iframe src="settings.html" width="100%" height="100%" frameborder="0"></iframe>';
            break; 
        // Dodaj inne przypadki dla innych zakładek
        default:
            mainContent.innerHTML = '<h2>Page Not Found</h2><p>The requested page was not found.</p>';
    }
}

// Funkcja do ładowania skryptów (nie zmieniła się)
function loadScript(script) {
    var existingScript = document.querySelector("script[src='" + script + "']");

    if (!existingScript) {
        var scriptElement = document.createElement('script');
        scriptElement.src = script;
        document.head.appendChild(scriptElement);
    }
}
