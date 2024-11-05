
function loadSidebar() {
    fetch(window.location.href.slice(0, window.location.href.indexOf("docs/")) + 'docs/sidebar.html')
        .then(response => response.text())
        .then(data => {
            document.getElementById('myUL').innerHTML = data;
        });
}
window.onload = loadSidebar;

document.getElementById("fa").href = window.location.origin;

function formatString(input) {
    // Reemplazar todos los guiones por espacios
    let formattedString = input.replace(/-/g, ' ');
    
    // Convertir la primera letra de cada palabra a mayúscula
    formattedString = formattedString.replace(/\b\w/g, function(char) {
        return char.toUpperCase();
    });
    
    return formattedString;
}

function penultimateIndexOf(str, searchValue) {
    // Encontrar la última coincidencia
    const lastIndex = str.lastIndexOf(searchValue);
    if (lastIndex === -1) return -1; // Si no hay coincidencias, retornar -1

    // Buscar todas las coincidencias anteriores a la última
    const beforeLast = str.substring(0, lastIndex);
    const penultimateIndex = beforeLast.lastIndexOf(searchValue);

    return penultimateIndex;
}

function getFolderName() {
    let url = window.location.href;
    if(penultimateIndexOf(url, "/") != -1) {
        return url.slice(penultimateIndexOf(url, "/") + 1, url.lastIndexOf("/"));
    }
    return -1;
}

document.title = "epi2 Docs - " + formatString(getFolderName());

function gotoweb(url) {
    let url2 = window.location.href;
    url2 = url2.slice(0, url2.indexOf("/docs/"));
    url2 += url;
    window.location.href = url2;
}

function showOne(id) {
    document.querySelectorAll("#container div").forEach(elem => elem.style.display = "none");
    document.getElementById(id).style.display = "";
}
var toggler = document.getElementsByClassName("caret");
var i;

for (i = 0; i < toggler.length; i++) {
  toggler[i].addEventListener("click", function() {
    this.parentElement.querySelector(".nested").classList.toggle("active");
    this.classList.toggle("caret-down");
  });
}


document.querySelectorAll("#container div").forEach(elem => {
    elem.innerHTML += "<span class='annimation' style='cursor: aliceblue;animation-name: cursor;animation-duration: 500ms;animation-iteration-count: infinite;animation-fill-mode: both;display:inline'>_</span>"
})