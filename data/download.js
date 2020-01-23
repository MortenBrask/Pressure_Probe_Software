//Selectors
const downloadTest = document.querySelector('#dropTest');
const uploadTest = document.querySelector('#upload');
const uploadStatusIcon = document.querySelector('#uploadStatusIcon');
const uploadStatusText = document.querySelector('#uploadStatusText');
//Selectors

document.addEventListener('WS_set', setHandler, false);

document.addEventListener('WS_success', successHandler, false);

document.addEventListener('WS_error', errorHandler, false);

function setHandler() {
    window.location.href = "downloadtest.html";
}

function successHandler() {
    uploadStatusIcon.className = "fas fa-save";
    uploadStatusIcon.style = "color: green";
    uploadStatusText.value = "Upload Complete";
    uploadStatusText.className = "form-control text-light bg-success text-center text-wrap"
}

function errorHandler() {
    uploadStatusIcon.className = "far fa-save";
    uploadStatusIcon.style = "color: red";
    uploadStatusText.value = "Upload Failed";
    uploadStatusText.className = "form-control text-light bg-danger text-center text-wrap"
}

document.addEventListener("DOMContentLoaded", function(event) {

    downloadTest.addEventListener('click', function(e) {
        e.preventDefault;
        sendStartData();
    });

    uploadTest.addEventListener('click', function(e) {
        e.preventDefault;
        sendSetData();
    });
  //Functions
});
