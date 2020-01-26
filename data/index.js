//Selectors
const randomId = document.querySelector('#random-id');
const newRandomIdButton = document.querySelector('#new-random-id-button');

const siteLabel = document.querySelector('#site-label');
const siteLabelButton = document.querySelector('#site-label-button');

const resetWifi = document.querySelector('#reset_wifi');
const startTest = document.querySelector('#start-test-button');
const resumeTest = document.querySelector('#resume-test-button');
//Selectors
document.addEventListener('WS_id', idHandler, false);
document.addEventListener('WS_index_init', indexInitHandler, false);
document.addEventListener('WS_site_label', siteLabelHandler, false);

function idHandler() {
  randomId.value = id_value;
}
function indexInitHandler() {
  randomId.value = id_value;
  siteLabel.value = site_label_value;
}
function siteLabelHandler() {
  siteLabel.value = site_label_value;
}



document.addEventListener("DOMContentLoaded", function(event) {

  startTest.addEventListener('click', function(e) {
    e.preventDefault;
    if(id_value === prev_id_value){
      alert("ERROR\nPlease generate new ID" );
    }
    else {
      startTest.href="video.html";
      return false;
    }
  });

  resumeTest.addEventListener('click', function(e) {
    e.preventDefault;
    if(id_value === prev_id_value){
      resumeTest.href="resume.html";
      return false;
    }
    else {
      alert("ERROR\nNo test in progress,\nPlease start new test");
    }
  });
  
  //Generate new random ID
  newRandomIdButton.addEventListener('click', sendStartData);

  siteLabelButton.addEventListener('click', function(e) {
    e.preventDefault;
    site_label_value = siteLabel.value;
    sendData(7)
  });

  //Reset WiFi configuration
  resetWifi.addEventListener('click', sendResetData);

  
  //Functions
});
