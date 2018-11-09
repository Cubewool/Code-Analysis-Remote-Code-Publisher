function myFunction(myDiv) {
    var x = document.getElementById(myDiv);
    if (x.style.display === 'none') {
        x.style.display = 'inline';
    } else {
        x.style.display = 'none';
    }
}