const hamburgerMenu = document.querySelector('.hamburger-menu');
const header = document.querySelector('header');
const navbarLinks = document.querySelector('.navbar__links');

hamburgerMenu.addEventListener('click', function() {
    header.classList.toggle('mobile__nav');

    // Show navbar links after delay when opening mobile nav
    if (navbarLinks.classList.contains('mobile__nav')) {
        navbarLinks.classList.toggle('mobile__nav');
    } else {
        setTimeout(function() {
            navbarLinks.classList.toggle('mobile__nav');
        }, 400)
    }
});