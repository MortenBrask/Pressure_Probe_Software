//Selectors
const downloadTest = document.querySelector('#dropTest');
const uploadTest = document.querySelector('#upload');

//Selectors
document.addEventListener('WS_set', setHandler, false);

function setHandler() {
    window.location.href = "downloadtest.html";
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